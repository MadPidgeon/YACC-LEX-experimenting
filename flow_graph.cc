#include "flow_graph.h"
#include "intermediate_code.h"

// ***********************************************
// * NOP
// ***********************************************

void flowGraph::clearNOP() {
	// todo: better complexity
	for( auto& b : basic_blocks ) {
		for( int i = b.operations.size()-1; i >= 0; --i ) {
			if( b.operations.at( i ).id == iop_t::id_t::IOP_NOP ) {
				b.operations.erase( b.operations.begin() + i );
			}
		}
	}
}

void flowGraph::removeMoveIdempotent() {
	for( auto& b : basic_blocks )
		for( auto& o : b.operations )
			if( o.id == iop_t::id_t::IOP_INT_MOV and o.parameterIsVariable(1) and o.getParameterVariable(0) == o.getParameterVariable(1) )
				o.id = iop_t::id_t::IOP_NOP;
}

// ***********************************************
// * Ad Hoc
// ***********************************************

void flowGraph::removeWriteOnlyMemory() {
	std::set<variable_t> read;
	for( auto& b : basic_blocks ) {
		for( auto& o : b.operations ) {
			auto r = o.getReadVariables();
			read.insert( r.begin(), r.end() );
		}
	}
	std::cout << "Safe variables:";
	for( variable_t v : read )
		std::cout << " " << symtab->getName( scptab->getVariableSymbol( v ) );
	std::cout << std::endl;
	for( auto& b : basic_blocks ) {
		for( auto& o : b.operations ) {
			auto r = o.getWrittenVariables();
			// assumption: all instructions writing to a variable have no side effects (like jmp has)
			if( r.size() > 0 ) {
				bool pointless = true;
				for( variable_t v : r )
					if( read.count( v ) )
						pointless = false;
				if( pointless )
					o.id = iop_t::id_t::IOP_NOP;
			}
		}
	}
}

// ***********************************************
// * Cyclic Equivalence
// ***********************************************

class checkForCycle {
	std::set<variable_t> visited;
	const std::map<variable_t,std::set<variable_t>>& graph;
	variable_t start;
public:
	checkForCycle( const std::map<variable_t,std::set<variable_t>>& g, variable_t s ) : graph( g ), start( s ) {
	}
	bool visit( std::vector<variable_t>& path, variable_t pos ) {
		if( visited.count( pos ) )
			return false;
		visited.insert( pos );
		if( graph.count( pos ) == 0 or graph.at( pos ).size() == 0 )
			return false;
		path.push_back( pos );
		for( variable_t next : graph.at( pos ) )
			if( next == start or visit( path, next ) )
				return true;
		path.pop_back();
		return false;
	}
	operator std::vector<variable_t>() {
		std::vector<variable_t> r;
		visit( r, start );
		return r;
	}
};

void flowGraph::resolveCyclicEquivalence( flowGraph::node* n, int i, const std::set<variable_t>& eq, variable_t target ) {
	std::vector<iop_t>& ops = n->operations;
	if( i == -1 )
		i = ops.size()-1;
	while( i >= 0 ) {
		iop_t& op = ops.at(i);
		if( op.id == iop_t::id_t::IOP_INT_MOV and op.parameterIsVariable(1) and op.getParameterVariable(1) == target and eq.count( op.getParameterVariable(0) ) ) {
			op.setParameterVariable( 0, target );
			return;
		} else 
			for( int p = 0; p < 3; ++p ) 
				if( op.parameterIsVariable( p ) )
					op.setParameterVariable( p, target );
		--i;
	}
	for( basic_block_t b : n->parents )
		resolveCyclicEquivalence( &basic_blocks.at( b ), -1, eq, target );
}

int flowGraph::node::cyclicEquivalence( flowGraph* fg ) {
	std::map<variable_t,std::set<variable_t>> equivalence = equal_in;
	int i = 0;
	for( const auto& op : operations ) {
		if( op.id == iop_t::id_t::IOP_INT_MOV and op.parameterIsVariable( 1 ) ) {
			variable_t a = op.getParameterVariable( 1 );
			variable_t b = op.getParameterVariable( 0 );
			if( a != b ) {
				equivalence[a].insert(b);
				if( equivalence[b].size() > 0 ) {
					std::cout << "Possible cyclic equivalence detected" << std::endl;
					std::vector<variable_t> cycle = checkForCycle( equivalence, a );
					if( cycle.size() > 1 ) {
						std::cout << "Cycle detected!:";
						for( variable_t v : cycle )
							std::cout << " " << symtab->getName( scptab->getVariableSymbol( v ) );
						std::cout << std::endl;
						std::set<variable_t> eq( cycle.begin(), cycle.end() );
						fg->resolveCyclicEquivalence( this, i, eq, b );
						return 2;
					}
				}
			}
		} else {
			for( variable_t v : op.getWrittenVariables() )
				equivalence[v].clear();
		}
		i++;
	}
	if( equivalence == equal_out )
		return 0;
	equal_out = std::move( equivalence );
	return 1;
}

void flowGraph::cyclicEquivalence() {
	bool change = true;
	while( change ) {
		change = false;
		for( basic_block_t b = 1; b < basic_blocks.size(); ++b ) {
			// compute ingoing constants
			auto& in = basic_blocks.at( b ).equal_in;
			if( basic_blocks.at( b ).parents.size() == 1 )
				in = basic_blocks.at( basic_blocks.at( b ).parents.back() ).equal_out;
			else if( basic_blocks.at( b ).parents.size() > 1 ) {
				const auto& checker = basic_blocks.at( basic_blocks.at( b ).parents.back() ).equal_out;
				for( auto itr = checker.begin(); itr != checker.end(); ++itr ) {
					variable_t v = itr->first;
					for( variable_t w : itr->second ) {
						bool contains = true;
						for( basic_block_t c = 1; c < basic_blocks.size(); ++c ) {
							if( basic_blocks.at( basic_blocks.at( b ).parents.at( 0 ) ).equal_out[v].count(w) == 0 ) {
								contains = false;
								break;
							} 
						}
						if( contains )
							in[v].insert( w );
					}
				}
			}
			int r = basic_blocks.at( b ).cyclicEquivalence( this );
			change |= r;
		}
	}
}

// ***********************************************
// * Constant Propagation
// ***********************************************

flowGraph::constant_data::constant_data() {
	type = ERROR_TYPE;
}

flowGraph::constant_data::constant_data( int64_t x ) {
	type = INT_TYPE;
	data.integer = x;
}

flowGraph::constant_data::constant_data( double x ) {
	type = FLT_TYPE;
	data.floating = x;
}

bool flowGraph::constant_data::operator==( const flowGraph::constant_data& other ) const {
	if( type != other.type )
		return false;
	if( type == INT_TYPE )
		return data.integer == other.data.integer;
	if( type == FLT_TYPE )
		return data.floating == other.data.floating;
	return false;
}

bool flowGraph::constant_data::operator!=( const flowGraph::constant_data& other ) const {
	return not operator==( other );
}

bool flowGraph::node::constantPropagation() {
	bool change = false;
	std::unordered_map<variable_t,constant_data> constants = const_in;
	for( auto& op : operations ) {
		// substitute in known constants
		for( int i = 1; i < 3; ++i ) {
			if( op.parameterIsVariable( i ) and op.parameterIsRead( i ) and not op.parameterIsWritten( i ) ) {
				auto itr = constants.find( op.getParameterVariable( i ) );
				if( itr != constants.end() ) 
					op.setParameterInteger( i, itr->second.data.integer );
			}
		}
		// aquire constants
		if( op.id == iop_t::id_t::IOP_INT_MOV and not op.parameterIsVariable( 1 ) ) {
			auto itr = constants.find( op.getParameterVariable( 0 ) );
			if( itr == constants.end() or itr->second.data.integer != op.getParameterInteger( 1 ) )
				constants[ op.getParameterVariable(0) ] = constant_data( op.getParameterInteger( 1 ) );
		}
		// lose constants
		if( op.id != iop_t::id_t::IOP_INT_MOV )
			for( int i = 0; i < 3; ++i )
				if( op.parameterIsWritten( i ) )
					constants.erase( op.getParameterVariable( i ) );
	}
	if( const_out == constants )
		return false;
	const_out = std::move( constants );
	return true;
}

void flowGraph::constantPropagation() {
	bool change = true;
	while( change ) {
		change = false;
		for( basic_block_t b = 1; b < basic_blocks.size(); ++b ) {
			// compute ingoing constants
			if( basic_blocks.at( b ).parents.size() > 0 ) {
				std::unordered_map<variable_t,constant_data> header;
				for( const auto& p : basic_blocks.at( basic_blocks.at( b ).parents.at( 0 ) ).const_out ) {
					bool equal = true;
					for( basic_block_t c : basic_blocks.at( b ).parents ) {
						auto itr = basic_blocks.at(c).const_out.find( p.first );
						if( itr == basic_blocks.at(c).const_out.end() or itr->second != p.second ) {
							equal = false;
							break;
						}
					}
					if( equal )
						header[p.first] = p.second;
				}
				if( header != basic_blocks.at(b).const_in ) {
					basic_blocks.at(b).const_in = std::move( header );
					change = true;
				}
			}
			change |= basic_blocks.at( b ).constantPropagation();
		}
	}
}

// ***********************************************
// * Lifetime Analysis
// ***********************************************

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

// ***********************************************
// * Construction
// ***********************************************

flowGraph::flowGraph( const intermediateCode::function& f ) {
	const auto& o = f.operations;
	size_t s = o.size();
	original_function = &f;
	node n = { ENTRY_BASIC_BLOCK, { NO_BASIC_BLOCK, NO_BASIC_BLOCK }, 0 };
	basic_blocks.push_back( n );
	// split into blocks
	for( size_t i = 0; i < s; ++i ) {
		++n.id;
		basic_blocks.push_back( n );
		basic_blocks.back().instruction_offset = i;
		if( o.at(i).id == iop_t::id_t::IOP_LABEL ) {
			labeled_blocks[o.at(i).label] = n.id;
			basic_blocks.back().label = o.at(i).label;
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
	// set children
	for( auto& b : basic_blocks ) {
		if( b.operations.size() == 0 )
			continue;
		auto& op = b.operations.back();
		if( op.id == iop_t::id_t::IOP_JUMP )
			b.children[0] = labeled_blocks[ op.label ];
		else if( op.id > iop_t::id_t::IOP_JUMP and op.id <= iop_t::id_t::IOP_FJ )
			b.children[1] = labeled_blocks[ op.label ];
	}
	// correct for last block
	if( basic_blocks.back().operations.size() == 0 )
		basic_blocks.back().children[0] = 0;
	else {
		const auto& op = basic_blocks.back().operations.back();
		if( op.id < iop_t::id_t::IOP_JUMP or op.id > iop_t::id_t::IOP_FJ )
			basic_blocks.back().children[0] = 0;
	}
	// set parents
	for( basic_block_t i = 0; i < basic_blocks.size(); ++i ) {
		for( int j = 0; j < 2; ++j ) {
			int c = basic_blocks.at(i).children[j];
			if( c != 0 )
				basic_blocks.at(c).parents.push_back(i);
		}
	}
	// to be removed
	computeLiveness();
}

intermediateCode::function flowGraph::generateFunction() const {
	std::vector<iop_t> ret;
	for( basic_block_t b = 1; b < basic_blocks.size(); ++b ) {
		if( basic_blocks.at(b).label != ERROR_LABEL )
			ret.push_back( iop_t{ iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, basic_blocks.at(b).label } );
		ret.insert( ret.end(), basic_blocks.at(b).operations.cbegin(), basic_blocks.at(b).operations.cend() );
	}
	return intermediateCode::function{ original_function->symbol, original_function->id, original_function->label, original_function->parent, ret };
}

void flowGraph::optimize() {
	constantPropagation();
	cyclicEquivalence();
	//std::cout << (*this) << std::endl;
	removeWriteOnlyMemory();
	// std::cout << (*this) << std::endl;
	removeMoveIdempotent();
	clearNOP();
	// std::cout << (*this) << std::endl;
}

// ***********************************************
// * I/O
// ***********************************************

std::ostream& operator<<( std::ostream& os, const live_interval_t& i ) {
	return os << symtab->getName( scptab->getVariableSymbol( i.variable ) ) << " [" << i.begin << "," << i.end << "]"; 
}

std::ostream& operator<<( std::ostream& os, const flowGraph& fg ) {
	size_t s = fg.basic_blocks.size();
	for( size_t i = 1; i < s; ++i ) {
		const auto& b = fg.basic_blocks.at(i);
		os << "BLOCK " << i << "(" << b.label << ") [to " << b.children[0] << " " << b.children[1] << "; from";
		for( basic_block_t x : b.parents )
			os << " " << x;
		os << "]:\n";
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
		os << "\nCONST IN: ";
		for( const auto& p : b.const_in )
			os << symtab->getName( scptab->getVariableSymbol( p.first ) ) << " ";
		os << "\nCONST OUT: ";
		for( const auto& p : b.const_out )
			os << symtab->getName( scptab->getVariableSymbol( p.first ) ) << " ";
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