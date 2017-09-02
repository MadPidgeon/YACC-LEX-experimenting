#include "syntax_tree.h"
#include <iomanip>
#include <vector>
#include <string>

const std::vector<std::string> node_t_to_str = {
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
	switch( type ) {
		// 0 children
		case N_INTEGER:
		case N_STRING:
		case N_FLOAT:
		case N_VARIABLE:
		case N_EMPTY:
		case N_CONTINUE:
		case N_BREAK:
		case N_GARBAGE:
			if( c != 0 )
				return ERROR_TYPE;
			break;
		// both optional
		case N_FUNCTION_CALL:
		case N_TUPLE:
			break;
		// 1 child (left)
		case N_COMPARISON_CHAIN:
		case N_UMIN: 
		case N_LIST: 
		case N_SET:
		case N_RETURN:
		case N_TUPLE_INDEXING:
		case N_SIZE_OF:
			if( c != 1 or children[1] != nullptr )
				return ERROR_TYPE;
			break;
		// 1 child(left), optional right child
		case N_SINGLE_TYPE_EXPRESSION_LIST:
		case N_BLOCK_LIST:
		case N_SEQUENTIAL_BLOCK:
		case N_PARALLEL_BLOCK:
		case N_TUPLE_LIST:
		case N_ARGUMENT_LIST:
			if( children[0] == nullptr )
				return ERROR_TYPE;
			break;
		// 2 children
		default:
			if( c != 2 )
				return ERROR_TYPE;
	}
	switch( type ) {
		case N_INTEGER:
			return INT_TYPE;
		case N_COMPARISON_CHAIN:
			if( children[0]->data_type != ERROR_TYPE )
				return INT_TYPE;
			return BOOL_TYPE;
		case N_EQUALS:   case N_NOT_EQUALS:
		case N_LESSER:   case N_GREATER:  case N_LESSER_EQ:case N_GREATER_EQ:
			if( children[0]->data_type != children[1]->data_type )
				return ERROR_TYPE;
			return children[0]->data_type;
		case N_FLOAT:
			return FLT_TYPE;
		case N_STRING:
			return STR_TYPE;
		case N_GARBAGE:
		case N_IF:       case N_WHILE:    case N_ELSE:     case N_FOR:
		case N_EMPTY:
		case N_SEQUENTIAL_BLOCK:          case N_PARALLEL_BLOCK:
		case N_BLOCK_LIST:
		case N_BREAK:    case N_CONTINUE: case N_RETURN:
			return VOID_TYPE;
		case N_ASSIGN:
			if( children[0]->data_type != children[1]->data_type )
				return ERROR_TYPE;
			return children[0]->data_type;
		case N_MULTIPLY: case N_DIVIDE:   case N_REMAINDER:
		case N_ADD:      case N_SUBTRACT:
			if( not ( children[0]->isIntegral() and children[1]->isIntegral() ) )
				return ERROR_TYPE;
			if( children[0]->data_type != children[1]->data_type )
				return ERROR_TYPE;
			return children[0]->data_type;
		case N_UMIN:
			if( not children[0]->isIntegral() )
				return ERROR_TYPE;
			return children[0]->data_type;
		case N_VARIABLE:
			return syntaxTree::scopes->getVariableType( data.integer );
		case N_SINGLE_TYPE_EXPRESSION_LIST:
			if( children[1] and children[0]->data_type != children[1]->data_type )
				return ERROR_TYPE;
			return children[0]->data_type;
		case N_LIST:
			return type_t( LST_STRUCTURE, { children[0]->data_type } );
		case N_SET:
			return type_t( SET_STRUCTURE, { children[0]->data_type } );
		case N_FUNCTION_CALL:
			// implement argument checking as well
			if( children[1] ) {
				assert( children[1]->data_type.isFunction() );
				return children[1]->data_type.getParameter(1);
			} else
				return syntaxTree::scopes->getFunctionReturnType( data.integer );
		case N_TUPLE: 
			if( not children[0] )
				return TUP_TYPE;
			// intentional lack of break
		case N_TUPLE_LIST: case N_ARGUMENT_LIST:
			return (children[1] ? children[1]->data_type : TUP_TYPE).rightFlattenTypeProduct( children[0]->data_type );
		case N_LIST_INDEXING:
			if( children[0]->data_type.isList() and children[1]->data_type == INT_TYPE )
				return children[0]->data_type.getChildType();
			return ERROR_TYPE;
		case N_TUPLE_INDEXING:
			if( children[0]->data_type.isTuple() and data.integer >= 0 and data.integer < children[0]->data_type.getParameterCount() )
				return children[0]->data_type.getParameter( data.integer );
			return ERROR_TYPE;
		case N_JOIN: case N_MEET:
			if( ( children[0]->data_type == STR_TYPE or children[0]->data_type.isList() or children[0]->data_type.isSet() ) and children[0]->data_type == children[1]->data_type )
				return children[0]->data_type;
			return ERROR_TYPE;
		case N_IN:
			if( ( children[1]->data_type.isList() or children[1]->data_type.isSet() ) and children[0]->data_type == children[1]->data_type.getChildType() )
				return BOOL_TYPE;
			if( children[0]->data_type == UTF8CHAR_TYPE and children[1]->data_type == STR_TYPE )
				return BOOL_TYPE;
			return ERROR_TYPE;
		case N_SIZE_OF:
			if( children[0]->data_type.isList() or children[1]->data_type.isSet() )
				return INT_TYPE;
			return ERROR_TYPE;
		case N_FUNCTION_DEFINITION:
			return ERROR_TYPE; // entered by other code
		default:
			lerr << error_line() << "Operation N(" << node_t_to_str[type] << ") not yet implemented!" << std::endl;
			return ERROR_TYPE;
	}
}

bool syntaxTree::node::isIntegral() const {
	return data_type == INT_TYPE or data_type == FLT_TYPE;
}

void syntaxTree::node::setType( node_t t ) {
	type = t;
	data_type = computeDatatype();
}

void syntaxTree::node::print( std::ostream& os, bool print_nulls, int depth ) const {
	os << std::endl << std::string(depth*2,' ') << node_t_to_str.at(type) << "," << data_type << "," << data.integer << "," << std::flush;
	for( int i = 0; i < 2; ++i )
		if( children[i] )
			children[i]->print( os, print_nulls, depth+1 );
		else if( print_nulls )
			os << std::endl << std::string(depth*2+2,' ') << "NULL";
}

syntaxTree::node::node( node_t node_type, node* left, node* right, extra_data_t d ) {
	type = node_type;
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
	if( type == node_t::N_STRING )
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

syntaxTree::syntaxTree( scopeTable* s ) {
	root = nullptr;
	scopes = s;
	assert( N_COUNT == node_t_to_str.size() );
}

const syntaxTree::node* syntaxTree::getRoot() const {
	return root;
}

void syntaxTree::setRoot( node* r ) {
	root = r;
}

std::ostream& operator<<( std::ostream& os, const syntaxTree& t ) {
	if( t.getRoot() )
		t.getRoot()->print( os, false );
	else
		os << "NULL";
	return os;
}
