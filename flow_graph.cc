#include "flow_graph.h"
#include "intermediate_code.h"

std::vector<live_interval_t> flowGraph::node::computeLiveness() const {
	const int NO_USE = -1;
	const int n = operations.size();
	std::unordered_map<variable_t,int> next_use; // negative for no next use
	std::unordered_map<variable_t,int> last_use; // --
	for( variable_t x : out ) {
		next_use[x] = n; // variables live on output have one past block as next use
		last_use[x] = n;
	}
	int line = n-1;
	std::vector<live_interval_t> intervals;
	for( auto itr = operations.rbegin(); itr != operations.rend(); ++itr ) {
		for( variable_t x : itr->getReadVariables() ) {
			if( next_use.count(x) == 0 ) {
				next_use[x] = NO_USE;
				last_use[x] = NO_USE;
			}
			next_use[x] = line;
			if( last_use[x] == NO_USE )
				last_use[x] = line;
		}
		for( variable_t x : itr->getWriteOnlyVariables() ) {
			if( last_use.count(x) and last_use[x] != NO_USE )
				intervals.push_back( live_interval_t{ line + instruction_offset, last_use[x] + instruction_offset, x } );
			next_use[x] = NO_USE;
			last_use[x] = NO_USE;
		}
		--line;
	}
	for( auto p : last_use )
		if( p.second != NO_USE )
			intervals.push_back( live_interval_t{ line + instruction_offset, p.second + instruction_offset, p.first } );
	return intervals;
}

void flowGraph::computeLiveness() {
	basic_block_t s = basic_blocks.size();
	for( basic_block_t i = 1; i < s; ++i ) {
		auto& b = basic_blocks.at( i );
		b.use.clear();
		b.def.clear();
		b.in.clear();
		b.out.clear();
		for( const iop_t op : b.operations ) {
			for( variable_t x : op.getReadVariables() )
				if( b.def.count( x ) == 0 )
					b.use.insert( x );
			for( variable_t x : op.getWriteOnlyVariables() )
				if( b.use.count( x ) == 0 )
					b.def.insert( x );
		}
	}
	bool change = true;
	while( change ) {
		change = false;
		for( basic_block_t i = 1; i < s; ++i ) {
			auto& b = basic_blocks.at( i );
			std::set<variable_t> nin = b.use;
			for( variable_t x : b.out )
				if( b.def.count( x ) == 0 )
					nin.insert( x );
			std::set<variable_t> nout;
			for( basic_block_t j = 0; j < 2 and b.children[j] != NO_BASIC_BLOCK; ++j )
				nout.insert( basic_blocks.at( b.children[j] ).in.begin(), basic_blocks.at( b.children[j] ).in.end() );
			change |= ( ( nin != b.in ) or ( nout != b.out ) );
			b.in = std::move( nin );
			b.out = std::move( nout );
		}
	}
}

std::vector<live_interval_t> flowGraph::naiveLiveIntervals() const {
	size_t s = basic_blocks.size();
	std::unordered_map<variable_t,live_interval_t> intervals; 
	for( size_t i = 1; i < s; ++i ) {
		const auto& b = basic_blocks.at( i );
		for( auto s : b.computeLiveness() ) {
			if( intervals.count( s.variable ) == 0 )
				intervals[s.variable] = s;
			else {
				intervals[s.variable].begin = std::min( intervals[s.variable].begin, s.begin );
				intervals[s.variable].end = std::max( intervals[s.variable].end, s.end );
			}
		}
	}
	std::vector<live_interval_t> ret;
	for( auto p : intervals )
		ret.push_back( p.second );
	return ret;
}

flowGraph::flowGraph( const intermediateCode::function& f ) {
	const auto& o = f.operations;
	size_t s = o.size();
	node n = { ENTRY_BASIC_BLOCK, { NO_BASIC_BLOCK, NO_BASIC_BLOCK }, 0 };
	basic_blocks.push_back( n );
	basic_blocks.back().operations.push_back( iop_t{ iop_t::id_t::IOP_NOP } );
	for( size_t i = 0; i < s; ++i ) {
		++n.id;
		basic_blocks.push_back( n );
		basic_blocks.back().instruction_offset = i;
		if( o.at(i).id == iop_t::id_t::IOP_LABEL ) {
			labeled_blocks[o.at(i).label] = n.id;
			++i;
		}
		basic_blocks.back().children[0] = n.id+1;
		for( ; i < s; ++i ) {
			if( o.at(i).id == iop_t::id_t::IOP_LABEL ) {
				--i;
				break;
			}
			basic_blocks.back().operations.push_back( o.at(i) );
			if( o.at(i).id >= iop_t::id_t::IOP_JUMP and o.at(i).id <= iop_t::id_t::IOP_FJ )
				break;
		}
	}
	if( basic_blocks.back().operations.size() == 0 )
		basic_blocks.back().operations.push_back( iop_t{ iop_t::id_t::IOP_NOP } );
	for( auto& b : basic_blocks ) {
		auto& op = b.operations.back();
		if( op.id == iop_t::id_t::IOP_JUMP )
			b.children[0] = labeled_blocks[ op.label ];
		else if( op.id > iop_t::id_t::IOP_JUMP and op.id <= iop_t::id_t::IOP_FJ )
			b.children[1] = labeled_blocks[ op.label ];
	}
	const auto& op = basic_blocks.back().operations.back();
	if( op.id < iop_t::id_t::IOP_JUMP or op.id > iop_t::id_t::IOP_FJ )
		basic_blocks.back().children[0] = 0;
	computeLiveness();
}

std::ostream& operator<<( std::ostream& os, const live_interval_t& i ) {
	return os << symtab->getName( scptab->getVariableSymbol( i.variable ) ) << " [" << i.begin << "," << i.end << "]"; 
}

std::ostream& operator<<( std::ostream& os, const flowGraph& fg ) {
	size_t s = fg.basic_blocks.size();
	for( size_t i = 1; i < s; ++i ) {
		const auto& b = fg.basic_blocks.at(i);
		os << "BLOCK " << i << " (to " << b.children[0] << "," << b.children[1] << "):\n";
		os << "USE: ";
		for( variable_t x : b.use )
			os << symtab->getName( scptab->getVariableSymbol( x ) ) << " ";
		os << "\nDEF: ";
		for( variable_t x : b.def )
			os << symtab->getName( scptab->getVariableSymbol( x ) ) << " ";
		os << "\nLIVE IN : ";
		for( variable_t x : b.in )
			os << symtab->getName( scptab->getVariableSymbol( x ) ) << " ";
		os << "\nLIVE OUT: ";
		for( variable_t x : b.out )
			os << symtab->getName( scptab->getVariableSymbol( x ) ) << " ";
		os << "\n";
		size_t j = b.instruction_offset;
		for( iop_t op : b.operations )
			os << "  " << std::setw(2) << (j++) << ": " << op << std::endl;
		{// temp
			for( auto i : b.computeLiveness() )
				os << i << std::endl;
		}
	}
	return os;
}