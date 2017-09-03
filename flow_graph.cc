#include <algorithm>
#include <limits>

#include "flow_graph.h"
#include "intermediate_code.h"

// ***********************************************
// * Block Order Operations
// ***********************************************

void flowGraph::node::setJumpsToChildren( const flowGraph* fg ) {
	if( direct_child != NO_BASIC_BLOCK ) {
		assert( operations.size() > 0 and operations.back().id == iop_t::id_t::IOP_JUMP );
		operations.back().label = fg->basic_blocks.at( direct_child ).label;
	}
	if( jump_child != NO_BASIC_BLOCK ) {
		assert( operations.size() > 1 and operations.back().id == iop_t::id_t::IOP_JUMP and operations.at(operations.size()-2).isJump() );
		operations.at( operations.size()-2 ).label = fg->basic_blocks.at( jump_child ).label;
	}
}

void flowGraph::expandBlocks() {
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization expandBlocks..." << std::endl;
	#endif
	// assign every block a label
	for( basic_block_t i = 1; i < basic_blocks.size(); ++i ) {
		if( basic_blocks.at(i).label == ERROR_LABEL ) {
			label_t l = original_function->parent->newLabel();
			basic_blocks.at(i).label = l;
			labeled_blocks[l] = i;
		}
	}
	// end every block with a jump
	for( basic_block_t i = 1; i < basic_blocks.size(); ++i ) {
		auto& b = basic_blocks.at(i);
		if( b.direct_child != NO_BASIC_BLOCK and ( b.operations.size() == 0 or b.operations.back().id != iop_t::id_t::IOP_JUMP ) )
			b.operations.push_back( iop_t{ iop_t::id_t::IOP_JUMP, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, basic_blocks.at( b.direct_child ).label } );
	}
	// correct jump labels
	for( basic_block_t i = 1; i < basic_blocks.size(); ++i )
		basic_blocks.at(i).setJumpsToChildren( this );
}

void flowGraph::contractBlocks() {
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization contractBlocks..." << std::endl;
	#endif
	// determine order
	std::set<basic_block_t> visited;
	std::vector<basic_block_t> order = {0};
	std::vector<basic_block_t> end_of_program_blocks;
	for( basic_block_t start = 1; start < basic_blocks.size(); ++start ) {
		if( visited.count( start ) )
			continue;
		basic_block_t i = start;
		visited.insert( start );
		order.push_back( start );
		while( basic_blocks.at( i ).direct_child != NO_BASIC_BLOCK and visited.count( basic_blocks.at( i ).direct_child ) == 0 ) {
			i = basic_blocks.at( i ).direct_child;
			visited.insert( i );
			order.push_back( i );
		}
		if( basic_blocks.at( order.back() ).direct_child == NO_BASIC_BLOCK ) {
			end_of_program_blocks.push_back( order.back() );
			order.pop_back();
		}
	}
	order.insert( order.end(), end_of_program_blocks.begin(), end_of_program_blocks.end() );

	// reorder
	std::vector<node> old = std::move( basic_blocks );
	basic_blocks.resize( 0 );
	for( int i : order )
		basic_blocks.push_back( std::move( old.at( i ) ) );
	std::vector<basic_block_t> inverse_order( order.size() );
	for( basic_block_t i = 0; i < order.size(); ++i )
		inverse_order.at( order.at( i ) ) = i;
	for( auto& b : basic_blocks ) {
		for( basic_block_t& p : b.parents )
			p = inverse_order.at( p );
		b.direct_child = inverse_order.at( b.direct_child );
		b.jump_child = inverse_order.at( b.jump_child );
	}

	// remove jumps
	for( basic_block_t i = 1; i < basic_blocks.size(); ++i ) {
		if( basic_blocks.at( i ).direct_child == i+1 ) {
			assert( basic_blocks.at( i ).operations.back().id == iop_t::id_t::IOP_JUMP );
			basic_blocks.at(i).operations.pop_back();
		}
	}

	// remove useless labels
	std::set<label_t> used_labels;
	for( auto& b : basic_blocks )
		for( auto& o : b.operations )
			if( o.usesLabel() )
				used_labels.insert( o.label );
	for( auto& b : basic_blocks )
		if( used_labels.count( b.label ) == 0 )
			b.label = ERROR_LABEL;

	// reinsert labels
	for( basic_block_t i = 1; i < basic_blocks.size(); ++i )
		if( basic_blocks.at(i).label != ERROR_LABEL )
			basic_blocks.at(i).operations.insert( basic_blocks.at(i).operations.begin(), iop_t{ iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, basic_blocks.at(i).label } );

	// renumber instructions
	size_t n = 0;
	for( auto& b : basic_blocks ) {
		b.instruction_offset = n;
		n += b.operations.size() + 1; 
		// the plus one is here for the live intervals, 
		// the skipped instruction is treated as one-past-the end of the block
	}
}

void flowGraph::splitBranch( basic_block_t i1, basic_block_t parent ) {
	// add clone
	basic_block_t i2 = basic_blocks.size();
	basic_blocks.push_back( basic_blocks.at( i1 ) );
	auto& b2 = basic_blocks.at( i2 );
	b2.parents = { parent };

	// remove parent from target
	auto& b = basic_blocks.at( i1 );
	assert( b.parents.size() > 1 );
	auto itr = std::find( b.parents.begin(), b.parents.end(), parent );
	assert( itr != b.parents.end() );
	b.parents.erase( itr );

	// set clone as child
	auto& c = basic_blocks.at( parent );
	if( c.direct_child == i1 )
		c.direct_child = i2;
	if( c.jump_child == i1 )
		c.jump_child = i2;
	c.setJumpsToChildren( this ); // requires expended graph
}

void flowGraph::walkGraph( basic_block_t i, std::set<basic_block_t>& visited ) {
	auto& b = basic_blocks.at( i );
	if( b.direct_child != NO_BASIC_BLOCK and not visited.count( b.direct_child ) ) {
		visited.insert( b.direct_child );
		walkGraph( b.direct_child, visited );
	} 
	if( b.jump_child != NO_BASIC_BLOCK and not visited.count( b.jump_child ) ) {
		visited.insert( b.jump_child );
		walkGraph( b.jump_child, visited );
	} 
}

void flowGraph::removeDeadCode() {
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization removeDeadCode..." << std::endl;
	#endif
	// find dead blocks
	std::set<basic_block_t> reached_blocks = {1};
	walkGraph( 1, reached_blocks );
	for( int i : reached_blocks )
		opt_out << i << " ";
	opt_out << std::endl;
	// delete blocks
	for( int i = basic_blocks.size()-1; i > 0; --i )
		if( reached_blocks.count( i ) == 0 )
			basic_blocks.erase( basic_blocks.begin()+i );
	// relabel blocks
	for( int i = 1; i < basic_blocks.size(); ++i )
		labeled_blocks[basic_blocks.at(i).label] = i;
	// set correct children
	for( int i = 1; i < basic_blocks.size(); ++i ) {
		auto& b = basic_blocks.at(i);
		if( b.direct_child != NO_BASIC_BLOCK )
			b.direct_child = labeled_blocks[ b.operations.back().label ];
		if( b.jump_child != NO_BASIC_BLOCK )
			b.jump_child = labeled_blocks[ b.operations.at( b.operations.size()-2 ).label ];
		b.parents.clear();
	}
	// set parents
	for( int i = 1; i < basic_blocks.size(); ++i ) {
		auto& b = basic_blocks.at( i );
		if( b.direct_child != NO_BASIC_BLOCK )
			basic_blocks.at( b.direct_child ).parents.push_back( i );
		if( b.jump_child != NO_BASIC_BLOCK )
			basic_blocks.at( b.jump_child ).parents.push_back( i );
	}
}

// ***********************************************
// * NOP
// ***********************************************

void flowGraph::clearNOP() {
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization clearNOP..." << std::endl;
	#endif
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
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization removeMoveIdempotent..." << std::endl;
	#endif
	for( auto& b : basic_blocks )
		for( auto& o : b.operations )
			if( o.id == iop_t::id_t::IOP_INT_MOV and o.parameterIsVariable(1) and o.getParameterVariable(0) == o.getParameterVariable(1) )
				o.id = iop_t::id_t::IOP_NOP;
}

// ***********************************************
// * Ad Hoc
// ***********************************************

void flowGraph::flipConstantCompares() {
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization flipConstantCompares..." << std::endl;
	#endif
	for( auto& b : basic_blocks )
		for( auto& o : b.operations )
			o.legalizeCompare();
}

void flowGraph::removeWriteOnlyMemory() {
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization removeWriteOnlyMemory..." << std::endl;
	#endif
	std::set<variable_t> read;
	for( auto& b : basic_blocks ) {
		for( auto& o : b.operations ) {
			auto r = o.getReadVariables();
			read.insert( r.begin(), r.end() );
		}
	}
	opt_out << "Safe variables:";
	for( variable_t v : read )
		opt_out << " " << symtab->getName( scptab->getVariableSymbol( v ) );
	opt_out << std::endl;
	for( auto& b : basic_blocks ) {
		for( auto& o : b.operations ) {
			if( o.hasSideEffects() )
				continue;
			auto w = o.getWrittenVariables();
			if( w.size() > 0 ) {
				bool pointless = true;
				for( variable_t v : w )
					if( read.count( v ) )
						pointless = false;
				if( pointless )
					o.id = iop_t::id_t::IOP_NOP;
			}
		}
	}
}

void flowGraph::removeUselessBooleans() {
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization removeUselessBooleans..." << std::endl;
	#endif
	/*for( auto& b : basic_blocks ) {
		std::map<variable_t,size_t> comparisons;
		for( size_t i = 0; i < b.operations.size(); ++i ) {
			auto& o = b.operations.at(i);
			if( o.id == iop_t::id_t::IOP_INT_EQ or o.id == iop_t::id_t::IOP_INT_NEQ )
				comparisons[o.r] = i;
			else if( o.id == iop_t::id_t::IOP_JT or o.id == iop_t::id_t::IOP_JF ) {
				if( o.parameterIsVariable( 1 ) ) {
					bool nequals = o.id == iop_t::id_t::IOP_JF;
					if( comparisons.count( o.a ) ) {
						auto& p = b.operations.at( comparisons[o.a] );
						nequals ^= p.id == iop_t::id_t::IOP_INT_NEQ;
						if( nequals )
							o.id = iop_t::id_t::IOP_JN;
						else
							o.id = iop_t::id_t::IOP_JE;
						o.b = p.b;
						o.c_b = p.c_b;
						o.a = p.a;
					}
				} else { // constant optimization

				}				
			} else for( variable_t v : o.getWrittenVariables() ) {
				comparisons.erase( v );
			}
		}
	}*/
	for( basic_block_t i = 1; i < basic_blocks.size(); ++i )
		basic_blocks.at(i).upgradeJump();
}

void flowGraph::node::upgradeJump() {
	if( jump_child == NO_BASIC_BLOCK )
		return;
	auto& jump = operations.at( operations.size()-2 );
	if( jump.id != iop_t::id_t::IOP_JT and jump.id != iop_t::id_t::IOP_JF )
		return;
	variable_t conditional_variable = jump.a;
	if( conditional_variable == ERROR_VARIABLE )
		return;
	for( int i = operations.size()-3; i >= 0; --i ) {
		auto& o = operations.at(i);
		if( o.id >= iop_t::id_t::IOP_INT_EQ and o.id <= iop_t::id_t::IOP_INT_GE and o.r == conditional_variable ) {
			bool inverse = ( jump.id == iop_t::id_t::IOP_JF );
			jump.id = iop_t::id_t( iop_t::id_t::IOP_JE + o.id - iop_t::id_t::IOP_INT_EQ );
			jump.a = o.a;
			jump.c_a = o.c_a;
			jump.b = o.b;
			jump.c_b = o.c_b;
			if( inverse )
				jump.negateCompare();
			return;
		} else
			for( variable_t x : o.getWrittenVariables() )
				if( x == conditional_variable )
					return;
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

void flowGraph::node::localCyclicEquivalence() {
	bool change = true;
	while( change ) { // repeat cyclic equivalence removal until stable
		change = false;
		std::map<variable_t,std::set<variable_t>> graph;
		for( int i = 0; i < operations.size(); ++i ) {
 			auto& op = operations.at( i );
			variable_t from = op.getParameterVariable( 1 );
			variable_t to = op.getParameterVariable( 0 );
			bool clear = false;
			// add equivalences
			if( op.id == iop_t::id_t::IOP_INT_MOV and from != ERROR_VARIABLE ) {
				// add edge to graph if not a self-loop
				if( from == to )
					continue;
				graph[from].insert( to );
				if( graph[to].size() > 0 ) {
					// edge added to vertex with path going out of it
					std::vector<variable_t> cycle = checkForCycle( graph, to );
					if( cycle.size() > 0 ) {
						// the path results in a cycle
						/*opt_out << "Cycle detected!:";
						for( variable_t v : cycle )
							opt_out << " " << symtab->getName( scptab->getVariableSymbol( v ) );
						opt_out << std::endl;
						std::cin.get();*/

						int stage = cycle.size()-2;
						std::swap( op.r, op.a );
						variable_t master = to;
						int j = i-1;
						for( ; j >= 0 and stage >= 0; --j ) {
							auto& op = operations.at( j );
							if( op.id == iop_t::id_t::IOP_INT_MOV and op.a == cycle[stage] and op.r == cycle[stage+1] ) {
								// found next assignment
								op.r = op.a;
								op.a = master;
								stage--;
							} else {
								// substitute in master variable
								for( int p = 0; p < 3; ++p )
									if( op.parameterIsVariable( p ) )
										op.setParameterVariable( p, master );
							}
						}
						operations.at( j+1 ).id = iop_t::id_t::IOP_NOP;
						assert( stage == -1 );
						change = true;
					} else {
						// the path does not result in a cycle
						clear = true;
					}
				}
			} else if( op.id == iop_t::id_t::IOP_FUNCTION ) {
				// function might affect any non-temporary
				graph.clear();
			} else	
				clear = true;
			// remove equivalences
			if( clear )
				for( variable_t v : op.getWrittenVariables() )
					graph[v].clear();
			if( change )
				break;
		}
	}
}

void flowGraph::cyclicEquivalence() {
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization cyclicEquivalence..." << std::endl;
	#endif
	for( int i = 1; i < basic_blocks.size(); ++i )
		basic_blocks.at(i).localCyclicEquivalence();
}

/*
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
		if( op.id == iop_t::id_t::IOP_INT_MOV and op.parameterIsVariable( 1 ) and equivalence[ op.getParameterVariable(0) ].size() == 0 ) {
			variable_t a = op.getParameterVariable( 1 );
			variable_t b = op.getParameterVariable( 0 );
			if( a != b ) {
				equivalence[a].insert(b);
				if( equivalence[b].size() > 0 ) {
					opt_out << "Possible cyclic equivalence detected" << std::endl;
					std::vector<variable_t> cycle = checkForCycle( equivalence, a );
					if( cycle.size() > 1 ) {
						opt_out << "Cycle detected!:";
						for( variable_t v : cycle )
							opt_out << " " << symtab->getName( scptab->getVariableSymbol( v ) );
						opt_out << std::endl;
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
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization cyclicEquivalence..." << std::endl;
	#endif
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
}*/

// ***********************************************
// * Constant Propagation
// ***********************************************

bool offset_variable_t::operator<( offset_variable_t other ) const {
	if( variable != other.variable )
		return variable < other.variable;
	return offset < other.offset;
}

bool offset_variable_t::operator==( offset_variable_t other ) const {
	return variable == other.variable and offset == other.offset;
}

offset_variable_t::offset_variable_t( variable_t v, size_t o ) 
		: variable( v ), offset( o ) {
}

offset_variable_t offset_variable_t::highest_lower_bound( variable_t v ) {
	return offset_variable_t( v, std::numeric_limits<decltype( offset )>::min() );
}

offset_variable_t offset_variable_t::lowest_upper_bound( variable_t v ) {
	return offset_variable_t( v, std::numeric_limits<decltype( offset )>::max() );
}

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
	std::map<offset_variable_t,constant_data> constants = const_in;
	for( auto& op : operations ) {
		// substitute in known constants
		if( op.id == iop_t::id_t::IOP_INT_TUP_LOAD ) {
			if( op.parameterIsVariable( 2 ) ) {
				auto itr = constants.find( op.getParameterVariable( 2 ) );
				if( itr != constants.end() ) 
					op.setParameterInteger( 2, itr->second.data.integer );
			}
			if( not op.parameterIsVariable( 2 ) ) {
				offset_variable_t s( op.getParameterVariable( 1 ), op.getParameterInteger( 2 ) );
				auto itr = constants.find( s );
				if( itr != constants.end() ) {
					op.id = iop_t::id_t::IOP_INT_MOV;
					op.setParameterInteger( 1, itr->second.data.integer );
				}
			}
		} else if( op.id == iop_t::id_t::IOP_FLT_TUP_LOAD ) {
			// nop
		} else for( int i = 1; i < 3; ++i ) {
			if( op.isFloating() ) {
				if( op.id == iop_t::id_t::IOP_FLT_MOV and op.parameterIsVariable( i ) ) {
					auto itr = constants.find( op.getParameterVariable( i ) );
					if( itr != constants.end() ) 
						op.setParameterFloating( i, itr->second.data.floating );
				}
			} else {
				if( op.parameterIsVariable( i ) and op.parameterIsRead( i ) and not op.parameterIsWritten( i ) ) {
					auto itr = constants.find( op.getParameterVariable( i ) );
					if( itr != constants.end() ) 
						op.setParameterInteger( i, itr->second.data.integer );
				}
			}
		}
		
		// reduce strength
		op.reduceStrength();
		if( op.usesResultParameter() and constants.count( op.r ) and constants.at( op.r ).type == INT_TYPE )
			op.reduceStrengthEq( constants.at( op.r ).data.integer );
		// aquire constants
		if( op.id == iop_t::id_t::IOP_INT_MOV and not op.parameterIsVariable( 1 ) ) {
			auto itr = constants.find( op.getParameterVariable( 0 ) );
			if( itr == constants.end() or itr->second.data.integer != op.getParameterInteger( 1 ) )
				constants[ op.getParameterVariable(0) ] = constant_data( op.getParameterInteger( 1 ) );
		} else if( op.id == iop_t::id_t::IOP_FLT_MOV and not op.parameterIsVariable( 1 ) ) {
			auto itr = constants.find( op.getParameterVariable( 0 ) );
			if( itr == constants.end() or itr->second.data.floating != op.getParameterFloating( 1 ) )
				constants[ op.getParameterVariable(0) ] = constant_data( op.getParameterFloating( 1 ) );
		} else if( op.id == iop_t::id_t::IOP_INT_TUP_STORE ) {
			if( ( not op.parameterIsVariable( 1 ) ) and ( not op.parameterIsVariable( 2 ) ) ) {
				// all is known, store constant
				offset_variable_t s( op.getParameterVariable( 0 ), op.getParameterInteger( 1 ) );
				auto itr = constants.find( s );
				if( itr == constants.end() or itr->second.data.integer != op.getParameterInteger( 2 ) )
					constants[ s ] = constant_data( op.getParameterInteger( 2 ) );
			} else if( op.parameterIsVariable( 1 ) ) {
				// don't know where is written, invalidate all
				constants.erase( constants.lower_bound( offset_variable_t::highest_lower_bound( op.getParameterVariable( 1 ) ) ), constants.upper_bound( offset_variable_t::lowest_upper_bound( op.getParameterVariable( 1 ) ) ) );
			} else {
				// don't know what is written, invalidate single entry
				constants.erase( offset_variable_t( op.getParameterVariable( 0 ), op.getParameterInteger( 1 ) ) );
			}
		}
		// lose constants
		if( op.id != iop_t::id_t::IOP_INT_MOV and op.id != iop_t::id_t::IOP_FLT_MOV and op.id != iop_t::id_t::IOP_INT_TUP_STORE and op.id != iop_t::id_t::IOP_INT_TUP_LOAD )
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
	#ifdef OPTIMIZATION_DEBUG
	opt_out << "Applying optimization constantPropagation..." << std::endl;
	#endif
	bool change = true;
	while( change ) {
		change = false;
		for( basic_block_t i = 1; i < basic_blocks.size(); ++i ) {
			auto& b = basic_blocks.at( i );
			// compute ingoing constants
			if( b.parents.size() > 0 ) {
				std::map<offset_variable_t,constant_data> header;
				for( const auto& p : basic_blocks.at( b.parents.at( 0 ) ).const_out ) {
					bool equal = true;
					for( basic_block_t j : b.parents ) {
						auto itr = basic_blocks.at( j ).const_out.find( p.first );
						if( itr == basic_blocks.at( j ).const_out.end() or itr->second != p.second ) {
							equal = false;
							break;
						}
					}
					if( equal )
						header[p.first] = p.second;
				}
				if( header != b.const_in ) {
					b.const_in = std::move( header );
					change = true;
				}
			}
			// propagate constants
			change |= b.constantPropagation();
			// fix jumps
			if( b.jump_child != NO_BASIC_BLOCK and b.operations.at( b.operations.size() - 2 ).id == iop_t::id_t::IOP_NOP )
				b.jump_child = NO_BASIC_BLOCK;
			if( b.jump_child != NO_BASIC_BLOCK and b.operations.at( b.operations.size() - 2 ).id == iop_t::id_t::IOP_JUMP ) {
				b.operations.pop_back();
				b.direct_child = b.jump_child;
				b.jump_child = NO_BASIC_BLOCK;
			}
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
	// compute used and defined variables
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
	// compute live and defined variables
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
			if( b.direct_child != NO_BASIC_BLOCK )
				nout.insert( basic_blocks.at( b.direct_child ).in.begin(), basic_blocks.at( b.direct_child ).in.end() );
			if( b.jump_child != NO_BASIC_BLOCK )
				nout.insert( basic_blocks.at( b.jump_child ).in.begin(), basic_blocks.at( b.jump_child ).in.end() );			
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
	node n = { ENTRY_BASIC_BLOCK, NO_BASIC_BLOCK, NO_BASIC_BLOCK, 0, ERROR_LABEL };
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
		basic_blocks.back().direct_child = n.id+1;
		for( ; i < s; ++i ) {
			if( o.at(i).id == iop_t::id_t::IOP_LABEL ) {
				--i;
				break;
			}
			basic_blocks.back().operations.push_back( o.at(i) );
			if( o.at(i).isJump() )
				break;
		}
	}
	// set children
	for( auto& b : basic_blocks ) {
		if( b.operations.size() == 0 )
			continue;
		auto& op = b.operations.back();
		if( op.id == iop_t::id_t::IOP_JUMP )
			b.direct_child = labeled_blocks[ op.label ];
		else if( op.isJump() )
			b.jump_child = labeled_blocks[ op.label ];
	}
	// correct for last block
	if( basic_blocks.back().operations.size() == 0 )
		basic_blocks.back().direct_child = 0;
	else {
		const auto& op = basic_blocks.back().operations.back();
		if( not op.isJump() )
			basic_blocks.back().direct_child = 0;
	}
	// set parents
	for( basic_block_t b = 0; b < basic_blocks.size(); ++b )
		for( basic_block_t c : { basic_blocks.at(b).direct_child, basic_blocks.at(b).jump_child } )
			if( c != NO_BASIC_BLOCK )
				basic_blocks.at(c).parents.push_back(b);
	// to be removed
	computeLiveness();
}

intermediateCode::function flowGraph::generateFunction() const {
	std::vector<iop_t> ret;
	for( basic_block_t b = 1; b < basic_blocks.size(); ++b )
		ret.insert( ret.end(), basic_blocks.at(b).operations.cbegin(), basic_blocks.at(b).operations.cend() );
	return intermediateCode::function{ original_function->symbol, original_function->id, original_function->label, original_function->parent, ret };
}

std::vector<std::vector<iop_t>> flowGraph::generateCodeByBlock() const {
	std::vector<std::vector<iop_t>> rb;
	for( basic_block_t i = 1; i < basic_blocks.size(); ++i ) {
		rb.emplace_back();
		auto& r = rb.back();
		auto& b = basic_blocks.at(i);
		r.insert( r.end(), b.operations.cbegin(), b.operations.cend() ); 
	}
	return rb;
}

const flowGraph::node& flowGraph::getBlock( basic_block_t b ) const {
	return basic_blocks.at( b );
}


void flowGraph::optimize( int optimization_level ) {
	// first optimization
	expandBlocks();	
	// second optimization
	if( optimization_level >= 1 )
		constantPropagation();
	if( optimization_level >= 1 )
		cyclicEquivalence();
	// third optimization
	if( optimization_level >= 1 )
		removeUselessBooleans();
	if( optimization_level >= 1 )
		removeMoveIdempotent();
	// fourth optimization
	if( optimization_level >= 1 )
		removeWriteOnlyMemory();
	if( optimization_level >= 1 )
		clearNOP();
	if( optimization_level >= 1 )
		removeDeadCode();
	// final optimization
	flipConstantCompares();
	contractBlocks();
	computeLiveness(); // temp
	if( opt_out.enabled ) {
		print( opt_out.stream, false );
	}	
}

// ***********************************************
// * I/O
// ***********************************************

void flowGraph::print( std::ostream& os, bool info ) const {
	os << std::setfill(' ') << std::left;
	size_t s = basic_blocks.size();
	for( size_t i = 1; i < s; ++i ) {
		const auto& b = basic_blocks.at(i);
		if( info ) {
			os << "BLOCK " << i << "(" << b.label << ") [to " << b.direct_child << " " << b.jump_child << "; from";
			for( basic_block_t x : b.parents )
				os << " " << x;
			os << "]:";
			os << "\nUSE: ";
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
			/*os << "\nCONST IN: ";
			for( const auto& p : b.const_in )
				os << symtab->getName( scptab->getVariableSymbol( p.first ) ) << " ";
			os << "\nCONST OUT: ";
			for( const auto& p : b.const_out )
				os << symtab->getName( scptab->getVariableSymbol( p.first ) ) << " ";*/
			os << "\n";
		}
		size_t j = b.instruction_offset;
		if( ( not info ) and b.label != ERROR_LABEL )
			os << b.label << ":\n"; 
		for( iop_t op : b.operations )
			os << " (" << std::setw(2) << (j++) << ") " << op << std::endl;
		/*{// temp
			for( auto i : b.computeLiveness() )
				os << i << std::endl;
		}*/
	}
}


std::ostream& operator<<( std::ostream& os, const live_interval_t& i ) {
	return os << symtab->getName( scptab->getVariableSymbol( i.variable ) ) << " [" << i.begin << "," << i.end << "]"; 
}

std::ostream& operator<<( std::ostream& os, const flowGraph& fg ) {
	fg.print( os, true );
	return os;
}