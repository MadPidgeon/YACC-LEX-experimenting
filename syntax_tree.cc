#include "syntax_tree.h"
#include <iomanip>
#include <vector>
#include <string>

const std::vector<std::string> syntaxTree::node_name = {
	"INTEGER", "FLOAT", "STRING", "VARIABLE",

	"ASSIGN", "GARBAGE",

	"COMPARISON_CHAIN",

	"EQUALS", "NOT_EQUALS", "LESSER", "GREATER", "LESSER_EQ", "GREATER_EQ",

	"ADD", "SUBTRACT", "MULTIPLY", "DIVIDE", "REMAINDER", "UMIN",

	"JOIN", "MEET", "IN",

	"ARGUMENT_LIST",

	"IF", "WHILE", "ELSE", "FOR", "BREAK", "CONTINUE", "RETURN",

	"SEQUENTIAL_BLOCK", "PARALLEL_BLOCK", "BLOCK_LIST",

	"LIST", "SET", "SINGLE_TYPE_EXPRESSION_LIST", "SIZE_OF",

	"TUPLE", "TUPLE_LIST",

	"FUNCTION_CALL", "FUNCTION_DEFINITION",

	"LIST_INDEXING", "TUPLE_INDEXING",

	"EMPTY"
};

scopeTable* syntaxTree::scopes;

type_t syntaxTree::node::computeDatatype() {
	int c = 0;
	int i;
	for( i = 0; i < 2; ++i ) {
		if( children[i] ) {
			++c;
			if( children[i]->data_type == ERROR_TYPE )
				return ERROR_TYPE;
		}
	}
	switch( id ) {
		// 0 children
		case SN::INTEGER:	case SN::STRING:	case SN::FLOAT:
		case SN::VARIABLE:	case SN::EMPTY:		case SN::GARBAGE:
		case SN::CONTINUE:	case SN::BREAK:
			if( c != 0 )
				return ERROR_TYPE;
			break;
		// optional left child
		case SN::TUPLE:
			if( children[1] )
				return ERROR_TYPE;
			break;
		// both optional
		case SN::FUNCTION_CALL:
			break;
		// 1 child (left)
		case SN::COMPARISON_CHAIN:
		case SN::UMIN: 		case SN::SIZE_OF:
		case SN::LIST:		case SN::SET:
		case SN::RETURN:
		case SN::TUPLE_INDEXING:
		case SN::SEQUENTIAL_BLOCK:
		case SN::PARALLEL_BLOCK:
			if( c != 1 or children[1] != nullptr )
				return ERROR_TYPE;
			break;
		// 1 child(left), optional right child
		case SN::SINGLE_TYPE_EXPRESSION_LIST:
		case SN::BLOCK_LIST:
		case SN::TUPLE_LIST:
		case SN::ARGUMENT_LIST:
			if( children[0] == nullptr )
				return ERROR_TYPE;
			break;
		// 2 children
		default:
			if( c != 2 )
				return ERROR_TYPE;
	}
	switch( id ) {
		case SN::INTEGER:
			return INT_TYPE;
		case SN::COMPARISON_CHAIN:
			if( children[0]->data_type != ERROR_TYPE )
				return INT_TYPE;
			return BOOL_TYPE;
		case SN::EQUALS:   case SN::NOT_EQUALS:
		case SN::LESSER:   case SN::GREATER:  case SN::LESSER_EQ:case SN::GREATER_EQ:
			if( children[0]->data_type != children[1]->data_type )
				return ERROR_TYPE;
			return children[0]->data_type;
		case SN::FLOAT:
			return FLT_TYPE;
		case SN::STRING:
			return STR_TYPE;
		case SN::GARBAGE:
		case SN::IF:       case SN::WHILE:    case SN::ELSE:     case SN::FOR:
		case SN::EMPTY:
		case SN::SEQUENTIAL_BLOCK:            case SN::PARALLEL_BLOCK:
		case SN::BLOCK_LIST:
		case SN::BREAK:    case SN::CONTINUE: case SN::RETURN:
			return VOID_TYPE;
		case SN::ASSIGN:
			if( children[0]->data_type != children[1]->data_type )
				return ERROR_TYPE;
			return children[0]->data_type;
		case SN::MULTIPLY: case SN::DIVIDE:   case SN::REMAINDER:
		case SN::ADD:      case SN::SUBTRACT:
			if( not ( children[0]->isIntegral() and children[1]->isIntegral() ) )
				return ERROR_TYPE;
			if( children[0]->data_type != children[1]->data_type )
				return ERROR_TYPE;
			return children[0]->data_type;
		case SN::UMIN:
			if( not children[0]->isIntegral() )
				return ERROR_TYPE;
			return children[0]->data_type;
		case SN::VARIABLE:
			return syntaxTree::scopes->getVariableType( data.integer );
		case SN::SINGLE_TYPE_EXPRESSION_LIST:
			if( children[1] and children[0]->data_type != children[1]->data_type )
				return ERROR_TYPE;
			return children[0]->data_type;
		case SN::LIST:
			return type_t( LST_STRUCTURE, { children[0]->data_type } );
		case SN::SET:
			return type_t( SET_STRUCTURE, { children[0]->data_type } );
		case SN::FUNCTION_CALL:
			// implement argument checking as well
			if( children[1] ) {
				if( not children[1]->data_type.isFunction() )
					return ERROR_TYPE;
				return children[1]->data_type.getParameter(1);
			} else
				return syntaxTree::scopes->getFunctionReturnType( data.integer );
		case SN::TUPLE: 
			if( not children[0] )
				return TUP_TYPE;
			return children[0]->data_type;
		case SN::TUPLE_LIST: case SN::ARGUMENT_LIST:
			return (children[1] ? children[1]->data_type : TUP_TYPE).rightFlattenTypeProduct( children[0]->data_type );
		case SN::LIST_INDEXING:
			if( children[0]->data_type.isList() and children[1]->data_type == INT_TYPE )
				return children[0]->data_type.getChildType();
			return ERROR_TYPE;
		case SN::TUPLE_INDEXING:
			if( children[0]->data_type.isTuple() and data.integer >= 0 and data.integer < children[0]->data_type.getParameterCount() )
				return children[0]->data_type.getParameter( data.integer );
			return ERROR_TYPE;
		case SN::JOIN: case SN::MEET:
			if( ( children[0]->data_type == STR_TYPE or children[0]->data_type.isList() or children[0]->data_type.isSet() ) and children[0]->data_type == children[1]->data_type )
				return children[0]->data_type;
			return ERROR_TYPE;
		case SN::IN:
			if( ( children[1]->data_type.isList() or children[1]->data_type.isSet() ) and children[0]->data_type == children[1]->data_type.getChildType() )
				return BOOL_TYPE;
			if( children[0]->data_type == UTF8CHAR_TYPE and children[1]->data_type == STR_TYPE )
				return BOOL_TYPE;
			return ERROR_TYPE;
		case SN::SIZE_OF:
			if( children[0]->data_type.isList() or children[1]->data_type.isSet() )
				return INT_TYPE;
			return ERROR_TYPE;
		case SN::FUNCTION_DEFINITION:
			return ERROR_TYPE; // entered by other code
		default:
			lerr << error_line() << "Operation " << (*this) << " not yet implemented!" << std::endl;
			return ERROR_TYPE;
	}
}

void syntaxTree::computeTyping() {
	if( root )
		root->computeTyping();
}

void syntaxTree::node::computeTyping() {
	for( int i = 0; i < 2; ++i )
		if( children[i] )
			children[i]->computeTyping();
	data_type = computeDatatype();
}

bool syntaxTree::node::isIntegral() const {
	return data_type == INT_TYPE or data_type == FLT_TYPE;
}

void syntaxTree::node::setType( syntaxTree::SN t ) {
	id = t;
	data_type = computeDatatype();
}

void syntaxTree::print( std::ostream& os, bool print_nulls ) const {
	if( getRoot() )
		getRoot()->print( os, print_nulls );
}


void syntaxTree::node::print( std::ostream& os, bool print_nulls, int depth ) const {
	os << std::endl << std::string(depth*2,' ') << node_name.at(id) << "," << data_type << "," << data.integer << "," << std::flush;
	for( int i = 0; i < 2; ++i )
		if( children[i] )
			children[i]->print( os, print_nulls, depth+1 );
		else if( print_nulls )
			os << std::endl << std::string(depth*2+2,' ') << "NULL";
}

syntaxTree::node* syntaxTree::node::clone() const {
	node* c[2] = {nullptr,nullptr};
	for( int i = 0; i < 2; ++i )
		if( children[i] )
			c[i] = children[i]->clone();
	return new node( id, c[0], c[1], data );
}

void syntaxTree::node::setChild( int i, syntaxTree::node* n ) {
	children[i] = n;
	data_type = computeDatatype();
}

syntaxTree::node::node( syntaxTree::SN node_type, node* left, node* right, extra_data_t d ) {
	id = node_type;
	children[0] = left;
	children[1] = right;
	parent = nullptr;
	for( int i = 0; i < 2; ++i )
		if( children[i] )
			children[i]->parent = this;
	data = d;
	data_type = computeDatatype();
}

syntaxTree::node::~node() {
	if( id == SN::STRING )
		delete [] data.string;
	if( parent != nullptr )
		throw;
	for( int i = 0; i < 2; ++i ) {
		if( children[i] ) {
			children[i]->parent = nullptr;
			delete children[i];
		}
	}
}

syntaxTree::node* syntaxTree::translateAbstractParseList( const parseTree::node* n, PN pjoiner, SN sjoiner, bool right_final_leaf ) {
	if( n == nullptr ) {
		return nullptr;
	} else if( n->id == pjoiner ) {
		return new node( sjoiner, translateExpression( n->children[0] ), translateAbstractParseList( n->children[1], pjoiner, sjoiner, right_final_leaf ) );
	} else if( right_final_leaf ) {
		return translateExpression( n );
	} else {
		lerr << error_line() << "Unexpected parse tree structure" << std::endl;
		return makeErrorNode();
	}
}

syntaxTree::node* syntaxTree::translateExpression( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateExpression(" << std::flush;
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	if( n->id == PN::ID )
		return translateIdentifier( n );
	else if( n->id == PN::INT )
		return new node( SN::INTEGER, nullptr, nullptr, {.integer=n->data.integer} );
	else if( n->id == PN::FLT )
		return new node( SN::FLOAT, nullptr, nullptr, {.floating=n->data.floating} );
	else if( n->id == PN::STR )
		return new node( SN::STRING, nullptr, nullptr, syntaxTree::node::extra_data_t{.string=n->data.string} );
	else if( n->id == PN::FUNCTION_CALL or n->id == PN::MEMBER_CALL )
		return translateFunctionCall( n );
	else if( n->id == PN::RELATION_OP )
		return translateComparisonChain( n );
	else if( n->id == PN::LIST or n->id == PN::SET or n->id == PN::TUPLE )
		return translateContainer( n );
	else if( n->id == PN::INLINE_FUNCTION_DEF )
		return translateInlineFunctionDefinition( n );
	else
		return translateOperator( n );
}

syntaxTree::node* syntaxTree::translateComparisonChain( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateComparisonChain(" << std::flush;
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	assert( n->id == PN::RELATION_OP );
	node* r = new node( SN::COMPARISON_CHAIN );
	const parseTree::node* parse_head = n;
	node** syntax_head = &r->children[0];
	while( parse_head and parse_head->id == PN::RELATION_OP ) {
		SN id = SN( SN::EQUALS + parse_head->data.integer );
		*syntax_head = new node( id, translateExpression( parse_head->children[0] ) );
		parse_head = parse_head->children[1];
		syntax_head = &(*syntax_head)->children[1];
	}
	*syntax_head = translateExpression( parse_head ); 
	return r;
}

syntaxTree::node* syntaxTree::recursiveTranslateTypedArguments( const parseTree::node* n ) {
	const parseTree::node* def = n;
	if( n->id == PN::COMMA )
		def = n->children[0];
	if( def->id != PN::VARIABLE_DECLARATION ) {
		lerr << error_line() << "Expected variable declaration in function argument list" << std::endl;
		return makeErrorNode();
	}
	type_t t = translateType( def->children[0] );
	if( def->children[1]->id != PN::ID ) {
		lerr << error_line() << "Expected identifier in variable declaration" << std::endl;
		return makeErrorNode();
	}
	symbol_t s = def->children[1]->data.symbol;
	variable_t v = scptab->addVariable( scope_stack.back(), s, t );
	syntaxTree::node* tail = nullptr;
	if( n->id == PN::COMMA )
		tail = recursiveTranslateTypedArguments( n->children[1] );
	return new node( SN::TUPLE_LIST, new node( SN::VARIABLE, nullptr, nullptr, {.integer=v} ), tail );
}

syntaxTree::node* syntaxTree::translateTypedArguments( const parseTree::node* n ) {
	assert( n->id == PN::TUPLE );
	if( n->children[0] )
		return recursiveTranslateTypedArguments( n->children[0] );
	else
		return nullptr;
}

syntaxTree::node* syntaxTree::translateOperator( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateOperator(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	assert( n->isOperator() );
	SN id = SN::EMPTY;
	switch( n->id ) {
		case PN::LOGIC_OP: case PN::LOGIC_ASSIGN_OP: case PN::LATTICE_OP: case PN::LATTICE_ASSIGN_OP:
			id = n->data.integer == 0 ? SN::JOIN : SN::MEET;
			break;
		case PN::ADD_OP: case PN::ADD_ASSIGN_OP:
			id = n->data.integer == 0 ? SN::ADD : SN::SUBTRACT;
			break;
		case PN::MULT_OP: case PN::MULT_ASSIGN_OP:
			id = n->data.integer == 0 ? SN::MULTIPLY : ( n->data.integer == 1 ? SN::DIVIDE : SN::REMAINDER );
			break;
		case PN::IN_OP:
			id = SN::IN;
			break;
		case PN::ASSIGN:
			id = SN::ASSIGN;
			break;
		case PN::SIZE_OP:
			id = SN::SIZE_OF;
			break;
		default:
			lerr << error_line() << "Unhandled operator " << n->id << std::endl;
			return nullptr;
	}
	node* lhs = translateExpression( n->children[0] );
	if( n->id == PN::SIZE_OP ) {
		return new node( id, lhs );
	} else if( n->id >= PN::LOGIC_ASSIGN_OP and n->id <= PN::EXPONENT_ASSIGN_OP ) {
		return new node( SN::ASSIGN, lhs, new node( id, lhs->clone(), translateExpression( n->children[1] ) ) );
	} else {
		return new node( id, lhs, translateExpression( n->children[1] ) );
	}	
}

syntaxTree::node* syntaxTree::translateIdentifier( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateIdentifier(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	assert( n->id == PN::ID );
	return new node( SN::VARIABLE, nullptr, nullptr, {.integer=scptab->getVariable( scope_stack.back(), n->data.symbol )} );
}

syntaxTree::node* syntaxTree::translateControlFlow( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateControlFlow(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	assert( n->isControlFlow() );
	switch( n->id ) {
		case PN::IF:
			return new node( SN::IF, translateExpression( n->children[0] ), translateStatement( n->children[1] ) );
		case PN::FOR: {
			if( n->children[0]->id != PN::IN_OP ) {
				lerr << error_line() << "Expected 'in' in for loop expression" << std::endl;
				return nullptr;
			}
			if( n->children[0]->children[0]->id != PN::ID ) {
				lerr << error_line() << "Expected identifier in for loop expression" << std::endl;
				return nullptr;
			}
			node* iterable = translateExpression( n->children[0]->children[1] );
			type_t t = ERROR_TYPE;
			if( iterable->data_type.isList() or iterable->data_type.isSet() ) {
				t = iterable->data_type.getChildType();
			} else if( iterable->data_type == STR_TYPE ) {
				t = UTF8CHAR_TYPE;
			} else {
				lerr << error_line() << "Type " << t << " is not iterable" << std::endl;
				return nullptr;
			}
			node* iterator = new node( SN::VARIABLE, nullptr, nullptr, {.integer = scptab->addVariable( scope_stack.back(), n->children[0]->children[0]->data.symbol, t ) } );
			return new node( SN::FOR, new node( SN::IN, iterator, iterable ), translateStatement( n->children[1] ) );
		}
		case PN::WHILE:
			return new node( SN::WHILE, translateExpression( n->children[0] ), translateStatement( n->children[1] ) );
		case PN::ELSE:
			return new node( SN::ELSE, translateStatement( n->children[0] ), translateStatement( n->children[1] ) );
		case PN::BREAK:
		case PN::CONTINUE:
			if( n->children[0] == nullptr )
				return new node( n->id == PN::BREAK ? SN::BREAK : SN::CONTINUE, nullptr, nullptr, {.integer=1} );
			else if( n->children[0]->id == PN::INT )
				return new node( n->id == PN::BREAK ? SN::BREAK : SN::CONTINUE, nullptr, nullptr, {.integer=n->children[0]->data.integer} );
			else {
				lerr << error_line() << "Can only break or continue a constant number of loops" << std::endl;
				return new node( n->id == PN::BREAK ? SN::BREAK : SN::CONTINUE, nullptr, nullptr, {.integer=1} );
			}
		case PN::RETURN:
			return new node( SN::RETURN, translateExpression( n->children[0] ) );
		default:
			syntree_out << error_line() << "Unhandled control flow " << n->id << std::endl;
			return nullptr;
	}
}

syntaxTree::node* syntaxTree::recursiveTranslateStatementList( const parseTree::node* n ) {
	if( n == nullptr )
		return nullptr;
	return new node( SN::BLOCK_LIST, translateStatement( n->children[0] ), recursiveTranslateStatementList( n->children[1] ) );
}

syntaxTree::node* syntaxTree::translateStatementList( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateStatementList(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	assert( n->id == PN::LIST or n->id == PN::SET );
	SN id = SN::SEQUENTIAL_BLOCK;
	if( n->id == PN::SET )
		id = SN::PARALLEL_BLOCK;
	return new node( id, recursiveTranslateStatementList( n->children[0] ) );
}

syntaxTree::node* syntaxTree::recursiveTranslateContainer( const parseTree::node* n, SN join ) {
	if( n->id != PN::COMMA )
		return new node( join, translateExpression( n ) );
	node* tail = recursiveTranslateContainer( n->children[1], join );
	return new node( join, translateExpression( n->children[0] ), tail, {.integer=tail->data.integer+1} );
}

syntaxTree::node* syntaxTree::translateContainer( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateContainer(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	assert( n->id == PN::LIST or n->id == PN::SET or n->id == PN::TUPLE );
	SN join = SN::SINGLE_TYPE_EXPRESSION_LIST;
	SN head = SN::LIST;
	if( n->id == PN::TUPLE ) {
		join = SN::TUPLE_LIST;
		head = SN::TUPLE;
	} else if( n->id == PN::SET )
		head = SN::SET;
	if( n->children[0] ) {
		node* tail = recursiveTranslateContainer( n->children[0], join );
		return new node( head, tail, nullptr, {.integer=tail->data.integer+1} );
	} else {
		return new node( head, nullptr, nullptr, {.integer=0} );
	}
}

syntaxTree::node* syntaxTree::translateInlineFunctionDefinition( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateInlineFunctionDefinition(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	assert( n->id == PN::INLINE_FUNCTION_DEF );
	scope_t parent_scope = scope_stack.back();
	scope_stack.push_back( scptab->addScope( parent_scope ) );
	node* arguments = translateTypedArguments( n->children[0] );
	if( n->children[1]->id != PN::FUNCTION_CALL ) {
		if( n->children[1]->id == PN::LIST or n->children[1]->id == PN::SET ) {
			lerr << error_line() << "Expected return type before function body" << std::endl;
			return makeErrorNode();
		} else if( n->children[1]->id == PN::TUPLE ) {
			lerr << error_line() << "Expected function body" << std::endl;
			return makeErrorNode();
		} else {
			lerr << error_line() << "Expected function definition" << std::endl;
			return makeErrorNode();
		}
	}
	type_t return_type = translateType( n->children[1]->children[0] );
	function_t f = scptab->addFunctionDeclaration( parent_scope, NONE_SYMBOL, return_type, scptab->getAllVariables( scope_stack.back() ) );
	node* body = translateStatementList( n->children[1]->children[1] );
	scope_stack.pop_back();
	node* r = new node( SN::FUNCTION_DEFINITION, arguments, body, {.integer = f} );
	r->data_type = type_t( FNC_STRUCTURE, { arguments->data_type, return_type } );
	return r;
}


syntaxTree::node* syntaxTree::translateFunctionCall( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateFunctionCall(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	assert( n->id == PN::FUNCTION_CALL or n->id == PN::MEMBER_CALL );

	// collect parameters
	node* lhs,* rhs;
	if( n->id == PN::FUNCTION_CALL )
		rhs = translateContainer( n->children[1] );
	else if( n->id == PN::MEMBER_CALL ) {
		if( n->children[1]->id != PN::FUNCTION_CALL ) {
			lerr << error_line() << "Member variables not yet implemented" << std::endl;
			return makeErrorNode();
		}
		rhs = translateContainer( n->children[1]->children[1] );
		node* list = rhs->children[0];
		rhs->children[0] = nullptr;
		delete rhs;
		node* object = translateExpression( n->children[0] );
		int index = list ? list->data.integer+1 : 0;
		rhs = new node( SN::TUPLE, new node( SN::TUPLE_LIST, object, list, {.integer=index} ), nullptr, {.integer=index+1} );
		n = n->children[1];
	}
	if( rhs->id != SN::TUPLE ) {
		lerr << error_line() << "Expected function arguments" << std::endl;
		return makeErrorNode();
	}

	// determine function
	size_t tuple_size = rhs->data_type.getParameterCount();
	if( n->children[0]->id == PN::ID ) { // named function call
		if( n->children[0]->data.symbol == TUP_SYMBOL )
			return rhs;
		function_t f = scptab->getFunction( scope_stack.back(), n->children[0]->data.symbol, rhs->data_type );
		variable_t v = scptab->getVariable( scope_stack.back(), n->children[0]->data.symbol );
		if( f == ERROR_FUNCTION and v == ERROR_FUNCTION ) {
			lerr << error_line() << "Unknown variable or function " << symtab->getName( n->children[0]->data.symbol ) << " taking argument " << rhs->data_type << std::endl;
			return new node( SN::FUNCTION_CALL, makeErrorNode(), rhs );
		}
		if( v == ERROR_VARIABLE or scptab->getVariableScope( v ) <= scptab->getFunctionScope( f ) ) { // named function
			return new node( SN::FUNCTION_CALL, rhs, nullptr, {.integer = f} );
		} else { // variable
			lhs = new node( SN::VARIABLE, nullptr, nullptr, {.integer = v} );
		}
	} else { // anonymous function call
		lhs = translateExpression( n->children[0] );
	}

	// list or tuple indexing
	if( lhs->data_type.isList() or lhs->data_type.isTuple() ) {
		if( tuple_size != 1 ) {
			lerr << error_line() << "Unexpected number of arguments to container indexing" << std::endl;
			return makeErrorNode();
		}
		node* index = rhs->children[0]->children[0];
		rhs->children[0]->children[0]->parent = nullptr;
		rhs->children[0]->children[0] = nullptr;
		delete rhs;
		if( lhs->data_type.isTuple() ) {
			if( index->id != SN::INTEGER ) {
				lerr << error_line() << "Can only index tuples by constants" << std::endl;
				return makeErrorNode();
			}
			int i = index->data.integer;
			delete index;
			return new node( SN::TUPLE_INDEXING, lhs, nullptr, {.integer=i} );
		} else
			return new node( SN::LIST_INDEXING, lhs, index );
	}
	return new node( SN::FUNCTION_CALL, rhs, lhs );
}

syntaxTree::node* syntaxTree::makeErrorNode() const {
	return new node( SN::VARIABLE, nullptr, nullptr, {.integer=ERROR_VARIABLE} );
}

syntaxTree::node* syntaxTree::translateStatement( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateStatement(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	if( n->id == PN::ASSIGN or n->id == PN::COMMA or n->id == PN::VARIABLE_DECLARATION )
		return translateVariableDeclaration( n );
	else if( n->isOperator() )
		return translateOperator( n );
	else if( n->isControlFlow() )
		return translateControlFlow( n );
	else if( n->id == PN::LIST or n->id == PN::SET ) // pas op met [1,2,3](1) = 4;
		return translateStatementList( n );
	else if( n->id == PN::FUNCTION_CALL or n->id == PN::MEMBER_CALL )
		return translateFunctionCall( n );
	syntree_out << error_line() << "Unhandled statement " << n->id << std::endl;
	return nullptr;
}

syntaxTree::node* syntaxTree::translateExpressionList( const parseTree::node* n ) {
	assert( n->id == PN::COMMA );
	lerr << error_line() << "C-style comma operator not (yet) supported" << std::endl;
	return nullptr;
}

syntaxTree::node* syntaxTree::translateVariableDeclaration( const parseTree::node* n, type_t t ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateVariableDeclaration(" << std::flush;
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << "," << t << ")" << std::endl;
	#endif
	if( n->id == PN::ID ) {
		// declaration only
		scptab->addVariable( scope_stack.back(), n->data.symbol, t );
		return nullptr;
	} else if( n->id == PN::ASSIGN ) {
		// assignment
		if( n->children[0]->id != PN::ID ) {
			lerr << error_line() << "Expected identifier in variable declaration" << std::endl;
			return nullptr;
		}
		scptab->addVariable( scope_stack.back(), n->children[0]->data.symbol, t );
		return new node( SN::BLOCK_LIST, translateExpression( n ) );
	} else {
		lerr << error_line() << "Expected variable declaration" << std::endl;
		return nullptr;
	}
}

type_t syntaxTree::translateType( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateType(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	if( n->id == PN::ID ) {
		type_t t = scptab->getTypeDefinition( scope_stack.back(), n->data.symbol );
		if( t == ERROR_TYPE ) {
			lerr << error_line() << "Unknown type " << symtab->getName( n->data.symbol ) << std::endl;
		}
		return t;
	} else if( n->id == PN::FUNCTION_CALL ) {
		if( n->children[0]->id == PN::ID ) {
			type_t arguments = translateType( n->children[1] );
			type_t t = scptab->getTypeDefinition( scope_stack.back(), n->children[0]->data.symbol, arguments.unpackProduct() );
			if( t == ERROR_TYPE ) {
				lerr << error_line() << "Unknown variadic type " << symtab->getName( n->children[0]->data.symbol ) << std::endl;
			}
			return t;
		} else {
			lerr << error_line() << "Type arithmetic not (yet) implemented" << std::endl;
			return ERROR_TYPE;
		}
	} else if( n->id == PN::TUPLE ) {
		type_t t = TUP_TYPE;
		parseTree::node* itr;
		for( itr = n->children[0]; itr->id == PN::COMMA; itr = itr->children[1] )
			t = t.leftFlattenTypeProduct( translateType( itr->children[0] ) );
		t = t.leftFlattenTypeProduct( translateType( itr ) );
		return t;
	} else {
		lerr << error_line() << "Expected type definition" << std::endl;
		return ERROR_TYPE;
	}
}

syntaxTree::node* syntaxTree::translateVariableDeclaration( const parseTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	syntree_out << "translateVariableDeclaration(";
	if( syntree_out.enabled )
		n->print( syntree_out.stream );
	syntree_out << ")" << std::endl;
	#endif
	const parseTree::node* assignment = n;
	if( n->id == PN::COMMA )
		assignment = n->children[0];	
	if( assignment->id != PN::ASSIGN and assignment->id != PN::VARIABLE_DECLARATION ) {
		if( n->id == PN::COMMA )
			return translateExpressionList( n );
		return translateExpression( n );
	}
	const parseTree::node* declaration = assignment;
	if( assignment->id == PN::ASSIGN )
		declaration = assignment->children[0];
	if( declaration->id != PN::VARIABLE_DECLARATION )
		return translateExpression( n );
	type_t t = translateType( declaration->children[0] );
	if( t == ERROR_TYPE ) {
		lerr << error_line() << "Unknown type in variable declaration" << std::endl;
		return nullptr;
	}
	const parseTree::node* id_node = declaration->children[1];
	if( id_node->id != PN::ID ) {
		lerr << error_line() << "Expected identifier in variable declaration" << std::endl;
		return nullptr;
	}
	scopes->addVariable( scope_stack.back(), id_node->data.symbol, t );
	node* r;
	if( assignment->id == PN::ASSIGN ) {
		r = new node( SN::ASSIGN, translateIdentifier( id_node ), translateExpression( assignment->children[1] ) );
	} else {
		r = new node( SN::EMPTY );
	}
	if( n->id != PN::COMMA )
		return r;

	// multiple declarations
	r = new node( SN::BLOCK_LIST, r );
	parseTree::node* parse_head = n->children[1];
	node** syntax_head = &r->children[1];
	while( parse_head and parse_head->id == PN::COMMA ) {
		node* decl = translateVariableDeclaration( parse_head->children[0], t ); 
		if( decl != nullptr ) {
			*syntax_head = decl;
			syntax_head = &(*syntax_head)->children[1];
		}
		parse_head = parse_head->children[1];
	}
	*syntax_head = translateVariableDeclaration( parse_head, t ); 
	return r;
}


syntaxTree::syntaxTree( scopeTable* s ) {
	assert( SN::COUNT == node_name.size() );
	scopes = s;
	scope_stack.push_back( GLOBAL_SCOPE );
	root = nullptr;
}

syntaxTree::syntaxTree( const parseTree& pt, scopeTable* s ) {
	assert( SN::COUNT == node_name.size() );
	scopes = s;
	scope_stack.push_back( GLOBAL_SCOPE );
	root = translateStatement( pt.getRoot() );
}

const syntaxTree::node* syntaxTree::getRoot() const {
	return root;
}

void syntaxTree::setRoot( node* r ) {
	root = r;
}

std::ostream& operator<<( std::ostream& os, const syntaxTree::node& n ) {
	return os << syntaxTree::node_name.at( n.id );
}

std::ostream& operator<<( std::ostream& os, const syntaxTree& t ) {
	if( t.getRoot() )
		t.getRoot()->print( os, false );
	else
		os << "NULL";
	return os;
}
