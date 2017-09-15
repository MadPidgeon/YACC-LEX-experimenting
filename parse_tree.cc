#include "parse_tree.h"
#include <cassert>

const std::vector<std::string> parseTree::node::node_name = {
	"NOP",

	"TUPLE", "LIST", "SET", "SEQUENCE_PART", "ELLIPSIS", "SPACE_JOINER", "COMMA",

	"MEMBER_OF", "ASSIGN", "ID", "INT", "FLT", "STR", "VARIABLE_DECLARATION", "FUNCTION_CALL", "INLINE_FUNCTION_DEF",

	"IF", "WHILE", "FOR", "ELSE", 

	"RETURN", "BREAK", "CONTINUE",

	"LOGIC_OP", "RELATION_OP", "IN_OP", "LATTICE_OP", "SIZE_OP", "ADD_OP", "MULT_OP", "EXPONENT_OP",

	"LOGIC_ASSIGN_OP", "LATTICE_ASSIGN_OP", "ADD_ASSIGN_OP", "MULT_ASSIGN_OP", "EXPONENT_ASSIGN_OP",

	"INCREMENT"
};

parseTree::node::node( id_t i, node* a, node* b, int64_t d ) {
	id = i;
	children[0] = a;
	children[1] = b;
	data.integer = d;
}

parseTree::node::node( id_t i, node* a, node* b, extra_data_t d ) {
	id = i;
	children[0] = a;
	children[1] = b;
	data = d;
}

void parseTree::generateTokenData() {
	uint64_t n = tokens.size();
	token_data.resize( n );
	std::vector<uint64_t> bracket_stack;
	std::vector<bool> bracket_contains_statement;
	bracket_stack.push_back( 0 );
	bracket_contains_statement.push_back( true );
	for( uint64_t i = 0; i < n; ++i ) {
		token_t& t = tokens.at( i );
		if( t.isLeftBracket() ) {
			bracket_stack.push_back( i );
			bracket_contains_statement.push_back( false );
		} else if( t.isRightBracket() ) {
			uint64_t h = bracket_stack.back();
			token_t& s = tokens.at( h );
			if( bracket_stack.size() == 1 ) {
				lerr << error_line() << "Missing left parenthesis" << std::endl;
			} else if( s.getDual() != t ) {
				lerr << error_line() << "Mismatched parenthesis" << std::endl;
			} else {
				bracket_stack.pop_back();
				token_data.at( i ).pair = h;
				token_data.at( h ).pair = i;
				if( bracket_contains_statement.back() ) {
					bracket_contains_statement.pop_back();
					bracket_contains_statement.back() = true;
					s.data.integer = t.data.integer = STATEMENT_BRACKET_TOKEN;
				} else {
					bracket_contains_statement.pop_back();
					s.data.integer = t.data.integer = DATA_BRACKET_TOKEN;
				}
			}
		} else if( t.id == token_t::id_t::SEMICOLON or t.isControlFlow() ) {
			bracket_contains_statement.back() = true;
		}
	}
	if( bracket_stack.size() != 1 ) {
		lerr << error_line() << "Missing right parenthesis" << std::endl;
	}
}

/*void parseTree::generateTokenData() {
	uint64_t n = tokens.size();
	token_data.resize( n );
	std::vector<uint64_t> bracket_stack;
	std::vector<bool> bracket_contains_statement;
	std::vector<bool> bracket_ends_statement;
	std::vector<uint64_t> semicolon_stack;
	bracket_stack.push_back( 0 );
	bracket_contains_statement.push_back( true ); // possibly not required due to .next
	semicolon_stack.push_back( 0 );
	for( uint64_t i = 0; i < n; ++i ) {
		const token_t& t = tokens.at( i );
		if( t.isLeftBracket() ) {
			bracket_stack.push_back( i );
			semicolon_stack.push_back( i );
			bracket_contains_statement.push_back( false );
		} else if( t.isRightBracket() ) {
			uint64_t h = bracket_stack.back();
			const token_t& s = tokens.at( h );
			if( bracket_stack.size() == 1 ) {
				lerr << error_line() << "Missing left parenthesis" << std::endl;
			} else if( s.getDual() != t ) {
				lerr << error_line() << "Mismatched parenthesis" << std::endl;
			} else {
				bracket_stack.pop_back();
				semicolon_stack.pop_back();
				token_data.at( semicolon_stack.back() ).next = i;
				semicolon_stack.back() = i;
				token_data.at( i ).pair = h;
				token_data.at( h ).pair = i;
				if( bracket_contains_statement.back() ) {
					bracket_contains_statement.pop_back();
					bracket_contains_statement.back() = true;
					token_data.at( i ).statements = token_data.at( h ).statements = true;
				} else
					bracket_contains_statement.pop_back();
			}
		} else if( t.id == token_t::id_t::SEMICOLON or t.id == token_t::id_t::ELSE ) {
			bracket_contains_statement.back() = true;
			token_data.at( semicolon_stack.back() ).next = i;
			semicolon_stack.back() = i;
		} else if( t.isControlFlow() ) {
			bracket_contains_statement.back() = true;
		}
	}
	if( bracket_stack.size() != 1 ) {
		lerr << error_line() << "Missing right parenthesis" << std::endl;
	}
}*/



/*void parseTree::parseSemicolonSeparatedStatements( uint64_t start, uint64_t end ) {

}*/



parseTree::node* parseTree::parseStatementList( interval_t interval ) {
	interval_t sub_interval = interval;
	node* head = new node( node::id_t::LIST /*temp*/ );
	node** tail = &( head->children[0] );
	while( sub_interval.size() > 1 ) {
		*tail = new node( node::id_t::SEQUENCE_PART, parseStatement( sub_interval ) );
		tail = &( (*tail)->children[1] );
	}
	return head;
}

parseTree::node* parseTree::parseStatement( interval_t& interval ) {
	#ifdef PARSE_TREE_DEBUG
	std::cout << "parseStatement(" << interval << "):" << std::endl;
	// printTokens( interval ); std::cout << std::endl;
	#endif
	token_t t = tokens.at( interval.begin+1 );
	if( t.isControlFlow() )
		return parseControlFlow( interval );
	else if( t.isLeftBracket() and t.data.integer == STATEMENT_BRACKET_TOKEN ) {
		interval_t sub_interval = { interval.begin+1, token_data.at( interval.begin+1 ).pair };
		interval.begin = sub_interval.end;
		return parseStatementList( sub_interval );
	}
	else
		return parseExpression( interval );
}

parseTree::node* parseTree::parseControlFlow( interval_t& interval ) {
	#ifdef PARSE_TREE_DEBUG
	std::cout << "parseControlFlow(" << interval << "):" << std::endl;
	// printTokens( interval ); std::cout << std::endl;
	#endif
	token_t t = tokens.at( interval.begin+1 );
	interval.begin += 1;
	node* r;
	if( t.id == TK::BREAK or t.id == TK::CONTINUE ) {
		if( tokens.at( interval.begin+1 ).id == TK::SEMICOLON ) {
			r = new node( t.id == TK::BREAK ? PN::BREAK : PN::CONTINUE );
			interval.begin += 1;
		} else
			r = new node( t.id == TK::BREAK ? PN::BREAK : PN::CONTINUE, parseExpression( interval ) );
	} else if( t.id == token_t::id_t::RETURN ) {
		r = new node( node::id_t::RETURN, parseExpression( interval ) );
	} else if( t.id == token_t::id_t::ELSE ) {
		lerr << error_line() << "No matching control flow statement matching \"else\"" << std::endl;
		r = parseStatement( interval );
	} else { // if, for, while
		node::id_t id = node::id_t::IF;
		if( t.id == token_t::id_t::WHILE )
			id = node::id_t::WHILE;
		else if( t.id == token_t::id_t::FOR )
			id = node::id_t::FOR;
		if( tokens.at( interval.begin+1 ).id != token_t::id_t::LBRA ) {
			lerr << "Expected left parenthesis after " << t << std::endl;
			return nullptr;
		}
		interval_t expression_interval = { interval.begin+1, token_data.at( interval.begin+1 ).pair };
		node* a = parseExpression( expression_interval );
		interval.begin = expression_interval.end;
		if( interval.size() <= 1 ) {
			lerr << error_line() << "Expected statement after " << t << std::endl;
			return nullptr;
		}
		if( tokens.at( interval.begin+1 ).id == token_t::id_t::SPACE_JOINER )
			interval.begin += 1;
		node* b = parseStatement( interval );
		if( interval.size() > 1 and tokens.at( interval.begin+1 ).id == token_t::id_t::ELSE ) {
			interval.begin += 1;
			node* c = parseStatement( interval );
			r = new node( id, a, new node( node::id_t::ELSE, b, c ) );
		} else {
			r = new node( id, a, b );
		}
	}
	return r;
}

parseTree::node* parseTree::parseAtom( token_t t ) {
	if( t.id == token_t::id_t::INT ) {
		return new node( node::id_t::INT, nullptr, nullptr, t.data );
	} else if( t.id == token_t::id_t::STR ) {
		return new node( node::id_t::STR, nullptr, nullptr, t.data );
	} else if( t.id == token_t::id_t::FLT ) {
		return new node( node::id_t::FLT, nullptr, nullptr, t.data );
	} else if( t.id == token_t::id_t::ID ) {
		return new node( node::id_t::ID, nullptr, nullptr, t.data );
	} else {
		lerr << error_line() << "Token is not an atom" << std::endl;
		throw;
	}
}

/*parseTree::node* parseTree::parseExpression( interval_t& interval ) {
	#ifdef PARSE_TREE_DEBUG
	std::cout << "parseExpression(" << interval << "):" << std::endl;
	printTokens( interval );
	std::cout << std::endl;
	#endif
	while( interval.size() > 0 and tokens.at( interval.begin ).id != token_t::id_t::SEMICOLON ) {
		// temp
		++interval.begin;
	}
	return nullptr;
}*/

void parseTree::parseOperation( std::deque<node*>& output, token_t t ) {
	if( output.size() < 2 ) {
		lerr << error_line() << "Expected two operands to binary operator " << t << std::endl;
		return;
	}
	node* b = output.back();
	output.pop_back();
	node* a = output.back();
	if( t.id >= token_t::id_t::LOGIC_OP and t.id <= token_t::id_t::EXPONENT_ASSIGN_OP ) {
		output.back() = new node( node::id_t( t.id + node::id_t::LOGIC_OP - token_t::id_t::LOGIC_OP ), a, b, t.data );
	} else if( t.id == token_t::id_t::ASSIGN ) {
		output.back() = new node( node::id_t::ASSIGN, a, b );
	} else if( t.id == token_t::id_t::COMMA ) {
		output.back() = new node( node::id_t::COMMA, a, b );
	} else if( t.id == token_t::id_t::SPACE_JOINER ) {
		if( b->id == node::id_t::ID )
			output.back() = new node( node::id_t::VARIABLE_DECLARATION, a, b );
		else
			output.back() = new node( node::id_t::FUNCTION_CALL, a, b );
	} else if( t.id == token_t::id_t::MAPS_TO ) {
		output.back() = new node( node::id_t::INLINE_FUNCTION_DEF, a, b );
	} else {
		lerr << error_line() << "Unknown operator " << t << std::endl;
		output.back() = new node( node::id_t::ADD_OP, a, b );
	}
}

void parseTree::parseSequence( std::deque<node*>& output, token_t t ) {
	/*node* parameters = nullptr;
	while( output.back() != nullptr ) {
		parameters = new node( node::id_t::SEQUENCE_PART, output.back(), parameters );
		output.pop_back();
	}*/
	node* parameters = nullptr;
	if( output.back() != nullptr ) {
		parameters = output.back();
		output.pop_back();
		if( output.back() != nullptr ) {
			lerr << error_line() << "What even is going on here" << std::endl;
		}
	}

	node::id_t id = node::id_t::TUPLE;
	if( t.id == token_t::id_t::RSEQ )
		id = node::id_t::LIST;
	else if( t.id == token_t::id_t::RPAR )
		id = node::id_t::SET;
	output.back() = new node( id, parameters );
}

parseTree::node* parseTree::parseExpression( interval_t& interval ) {
	#ifdef PARSE_TREE_DEBUG
	std::cout << "parseExpression(" << interval << "):" << std::endl;
	printTokens( interval );
	std::cout << std::endl;
	#endif
	interval.begin += 1;
	token_t t = tokens.at( interval.begin );
	std::deque<node*> output;
	std::deque<token_t> operators;
	while( interval.begin < interval.end and t != EOF_TOKEN and t.id != token_t::id_t::SEMICOLON ) {
		// ----------------------
		std::cout << "Output: ";
		for( node* x : output ) {
			if( x == nullptr )
				std::cout << "NULL ";
			else
				std::cout << node::node_name.at( x->id ) << " ";
		}
		std::cout << std::endl;
		std::cout << "Stack : ";
		for( token_t x : operators ) {
			std::cout << x << " ";
		}
		std::cout << std::endl;
		// ----------------------
		if( t.isAtom() ) {
			output.push_back( parseAtom( t ) );
		} /*else if( t.id == token_t::id_t::COMMA ) {
			while( ( not operators.empty() ) and ( not operators.back().isLeftBracket() ) ) {
				parseOperation( output, operators.back() );
				operators.pop_back();
			}
			if( operators.empty() ) {
				lerr << error_line() << "Missing left parenthesis" << std::endl;
				return nullptr;
			}
		}*/ else if( t.isBinaryOperator() ) {
			while( ( not operators.empty() ) and ( not operators.back().isLeftBracket() ) ) {
				const token_t& u = operators.back();
				if( t.isLeftAssociative() ) {
					if( t.precedence() > u.precedence() )
						break;
				} else {
					if( t.precedence() >= u.precedence() )
						break;
				}
				parseOperation( output, u );
				operators.pop_back();
			}
			operators.push_back( t );
		} else if( t.isLeftBracket() and t.data.integer == DATA_BRACKET_TOKEN ) {
			operators.push_back( t );
			output.push_back( nullptr );
		} else if( t.isLeftBracket() and t.data.integer == STATEMENT_BRACKET_TOKEN ) {
			interval_t sub_interval = { interval.begin, token_data.at( interval.begin ).pair };
			output.push_back( parseStatementList( sub_interval ) );
			interval.begin = sub_interval.end;
		} else if( t.isRightBracket() and t.data.integer == DATA_BRACKET_TOKEN ) {
			while( ( not operators.empty() ) and ( not operators.back().isLeftBracket() ) ) {
				parseOperation( output, operators.back() );
				operators.pop_back();
			}
			if( operators.empty() ) {
				lerr << error_line() << "Missing left parenthesis" << std::endl;
				return nullptr;
			} else if( operators.back().getDual() != t ) {
				lerr << error_line() << "Mismatched parenthesis" << std::endl;
				return nullptr;
			} else {
				operators.pop_back();
				parseSequence( output, t );
			}
			/*while( ( not operators.empty() ) and ( not operators.back().isLeftBracket() ) ) {
				parseOperation( output, operators.back() );
				operators.pop_back();
			}
			if( operators.empty() ) {
				lerr << error_line() << "Missing left parenthesis" << std::endl;
				return nullptr;
			} else if( operators.back().getDual() != t ) {
				lerr << error_line() << "Mismatched parenthesis" << std::endl;
				return nullptr;
			} else if( operators.back().id == token_t::id_t::ID ) {
				parseFunctionCall( output, operators.back() );
				operators.pop_back();
			} else {
				parseSequence( output, t );
				operators.pop_back();
			}*/
		} else if( t.isStatement() ) {
			lerr << error_line() << "Unexpected statement " << t << " in expression" << std::endl; 
		} else {
			lerr << error_line() << "Unhandled case " << t << " in expression parser" << std::endl;
		}
		interval.begin += 1;
		t = tokens.at( interval.begin );
	}
	while( not operators.empty() ) {
		if( operators.back().isLeftBracket() ) {
			lerr << error_line() << "Missing right parenthesis" << std::endl;
			return nullptr;
		}
		parseOperation( output, operators.back() );
		operators.pop_back();
	}
	// ----------------------
	std::cout << "Output: ";
	for( node* x : output ) {
		if( x == nullptr )
			std::cout << "NULL ";
		else
			std::cout << node::node_name.at( x->id ) << " ";
	}
	std::cout << std::endl;
	// ----------------------
	if( output.size() == 1 )
		return output.front();
	else {
		lerr << error_line() << "Missing operands in expression" << std::endl;
		return nullptr;
	}
}

/*parseTree::node* parseTree::parseStatement( int& begin, int& end ) {
	#ifdef PARSE_TREE_DEBUG
	std::cout << "parseStatement(" << begin << "," << end << "):" << std::endl;
	printTokens( begin, end );
	std::cout << std::endl;
	#endif
	token_t t = tokens.at( begin+1 );
	if( t.isControlFlow() )
		return parseControlFlow( begin, end );
	else {
		node* r = parseExpression( begin+1, end );
		begin = end;
		end = token_data.at( begin ).next;
	}
}

parseTree::node* parseTree::parseExpression( int begin, int end ) {
	#ifdef PARSE_TREE_DEBUG
	std::cout << "parseExpression(" << begin << "," << end << "):" << std::endl;
	printTokens( begin, end );
	std::cout << std::endl;
	#endif
}

parseTree::node* parseTree::parseControlFlow( int& begin, int& end ) {
	#ifdef PARSE_TREE_DEBUG
	std::cout << "parseControlFlow(" << begin << "," << end << "):" << std::endl;
	printTokens( begin, end );
	std::cout << std::endl;
	#endif
	token_t t = tokens.at( begin+1 );
	node* r;
	if( t.id == token_t::id_t::BREAK or t.id == token_t::id_t::CONTINUE ) {
		r = new node( t.id == token_t::id_t::BREAK ? node::id_t::BREAK : node::id_t::CONTINUE, parseExpression( begin+2, end ) );
		begin = end;
		end = token_data.at( begin ).next;
	} else if( t.id == token_t::id_t::RETURN ) {
		r = new node( node::id_t::RETURN, parseExpression( begin+2, end ) );
		begin = end;
		end = token_data.at( begin ).next;
	} else if( t.id == token_t::id_t::ELSE ) {
		lerr << error_line() << "No matching control flow statement matching \"else\"" << std::endl;
		begin = end;
		end = token_data.at( begin ).next;
		parseStatement( begin, end );
	} else { // if, for, while
		node::id_t id = node::id_t::IF;
		if( t.id == token_t::id_t::WHILE )
			id = node::id_t::WHILE;
		else if( t.id == token_t::id_t::FOR )
			id = node::id_t::FOR;
		if( tokens.at( begin+2 ).id != token_t::id_t::LBRA ) {
			lerr << "Expected left parenthesis after " << t << std::endl;
			return nullptr;
		}
		node* a = parseExpression( begin+2, end );
		begin = end;
		end = token_data.at( begin ).next;
		if( end == 0 ) {
			lerr << error_line() << "Expected statement after " << t << std::endl;
			return nullptr;
		}
		node* b = parseStatement( begin, end );
		if( begin != 0 and tokens.at( begin ).id == token_t::id_t::ELSE ) {
			begin = end;
			end = token_data.at( begin ).next;
			node* c = parseStatement( begin, end );
			r = new node( id, a, new node( node::id_t::ELSE, b, c ) );
		} else {
			r = new node( id, a, b );
		}
	}
}*/


void parseTree::printTokens( interval_t interval, bool show_extra ) const {
	for( size_t i = interval.begin; i <= interval.end; ++i ) {
		std::cout << tokens.at( i );
		if( show_extra ) {
			std::cout << "(" << i << "," << token_data.at( i ).pair << ")";
		}
		std::cout << " ";
	}
}

/*void shuntingYard( tokenizer& input ) {
	token_t t = input.getToken();
	std::deque<token_t> output;
	std::deque<token_t> operators;
	while( t != EOF_TOKEN ) {
		if( t.isAtom() ) {
			if( input.peekToken(1).id == token_t::id_t::LBRA )
				operators.push_back( t );
			else
				output.push_back( t );
		} else if( t.id == token_t::id_t::SEMICOLON or t.id == token_t::id_t::COMMA ) {
			while( ( not operators.empty() ) and ( not operators.back().isLeftBracket() ) ) {
				output.push_back( operators.back() );
				operators.pop_back();
			}
			if( operators.empty() ) {
				lerr << error_line() << "Missing left parenthesis" << std::endl;
				return;
			}
		} else if( t.isBinaryOperator() ) {
			while( not operators.empty() ) {
				const token_t& u = operators.back();
				if( t.isLeftAssociative() ) {
					if( t.precedence() > u.precedence() )
						break;
				} else {
					if( t.precedence() >= u.precedence() )
						break;
				}
				output.push_back( u );
				operators.pop_back();
			}
			output.push_back( t );
		} else if( t.isLeftBracket() ) {
			output.push_back( t );
		} else if( t.isRightBracket() ) {
			while( ( not operators.empty() ) and ( not operators.back().isLeftBracket() ) ) {
				output.push_back( operators.back() );
				operators.pop_back();
			}
			if( operators.empty() ) {
				lerr << error_line() << "Missing left parenthesis" << std::endl;
				return;
			} else if( operators.back().getDual() != t ) {
				lerr << error_line() << "Mismatched parenthesis" << std::endl;
				return;
			} else if( operators.back().id == token_t::id_t::ID ) {
				output.push_back( token_t{ token_t::id_t::INT, { .integer = argument_count.back() } } );
				operators.back().id = token_t::id_t::FUNCTION;
				output.push_back( operators.back() );
				operators.pop_back();
			} else {
				output.push_back( token_t{ token_t::id_t::INT, { .integer = argument_count.back() } } );
				output.push_back( operators.back() );
				operators.pop_back();
			}
		} else if( t.isStatement() ) {
			operators.push_back( t );
		}
		t = input.getToken();
	}
}*/

/*void translateSequence( token_t::id_t previous, int depth ) {

}*/

bool parseTree::node::isOperator() const {
	return ( id >= id_t::LOGIC_OP and id <= id_t::EXPONENT_ASSIGN_OP ) or id == id_t::ASSIGN;
}

bool parseTree::node::isControlFlow() const {
	return ( id >= id_t::IF and id <= id_t::CONTINUE );
}

const parseTree::node* parseTree::getRoot() const {
	return root;
}

size_t interval_t::size() const {
	if( begin > end )
		return 0;
	return end - begin;
}

std::ostream& operator<<( std::ostream& os, interval_t i ) {
	return os << "[" << i.begin << "," << i.end << "]";
}

void parseTree::print( std::ostream& os, bool extra ) const {
	if( root )
		root->print( os, extra );
}

void parseTree::node::print( std::ostream& os, bool extra, int depth ) const {
	for( int i = 0; i < depth; ++i )
		os << "  ";
	os << node_name.at( id ) << std::endl;
	for( int i = 0; i < 2; ++i )
		if( children[i] )
			children[i]->print( os, extra, depth+1 );
		else if( extra ) {
			for( int j = 0; j < depth+1; ++j )
				os << "  ";
			os << "NULL\n";
		}
}

std::ostream& operator<<( std::ostream& os, const parseTree& tree ) {
	tree.print( os, false );
	return os;
}

parseTree::parseTree( const std::vector<token_t>& ts ) {
	tokens = ts;
	token_data.resize( tokens.size(), {0} );
	generateTokenData();
	root = parseStatementList( { 0, tokens.size()-1 } );
	assert( node::node_name.size() == node::id_t::COUNT );
}
