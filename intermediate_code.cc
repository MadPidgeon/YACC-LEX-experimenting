#include <iomanip>
#include <stack>
#include "intermediate_code.h"

const std::vector<std::string> iop_id_to_str = {
	"NOP",
	"GARBAGE",
	"INT_MOVE", "STR_MOVE", "FLT_MOVE",
	"LABEL",
	"JUMP", 
	"JT", "JF",
	"JE", "JN", "JL", "JG", "JLE", "JGE",
	"FJ",
	"INT_ADD_PARAM", "FLT_ADD_PARAM", "RESERVE_RETURN",
	"FUNCTION", 
	"LABEL_TO_PTR",
	"RETURN",
	"INT_ANDEQ", "INT_OREQ", "INT_XOREQ", "INT_NANDEQ",
	"INT_ADDEQ", "INT_SUBEQ", "INT_MULEQ", 
	"INT_MODDIV",
	"FLT_ADDEQ", "FLT_SUBEQ", "FLT_MULEQ", "FLT_DIVEQ",
	"FLT_ADD", "FLT_SUB", "FLT_MUL", "FLT_DIV",
	"STR_CONEQ",
	"INT_EQ", "INT_NEQ", "INT_L", "INT_G", "INT_LE", "INT_GE",
	"INT_ARR_LOAD", "INT_ARR_STORE",
	"FLT_ARR_LOAD", "FLT_ARR_STORE",
	"INT_TUP_LOAD", "INT_TUP_STORE",
	"FLT_TUP_LOAD", "FLT_TUP_STORE",
	"LIST_ALLOCATE", "LIST_SIZE"
};

const std::vector<uint8_t> iop_t::iop_fields = {
	0,
	IOFR|IOFS,
	IOFR|IOFA|IOFS, IOFR|IOFA|IOFS, IOFR|IOFA|IOFS|IOFF,
	IOFL,
	IOFL|IOFJ,
	IOFA|IOFL|IOFJ, IOFA|IOFL|IOFJ,
	IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ,
	IOFR|IOFL|IOFJ,
	IOFA, IOFA|IOFF, IOFA,
	IOFR|IOFA|IOFL|IOFS, // FUNCTION is not a jump!
	IOFR|IOFL|IOFS,
	IOFA,
	IOFR|IOFA, IOFR|IOFA, IOFR|IOFA ,IOFR|IOFA,
	IOFR|IOFA, IOFR|IOFA, IOFR|IOFA,
	IOFR|IOFA|IOFB,
	IOFR|IOFA|IOFF, IOFR|IOFA|IOFF, IOFR|IOFA|IOFF, IOFR|IOFA|IOFF,
	IOFR|IOFA|IOFB|IOFF, IOFR|IOFA|IOFB|IOFF, IOFR|IOFA|IOFB|IOFF, IOFR|IOFA|IOFB|IOFF,
	IOFR|IOFA,
	IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS,
	IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS,
	IOFR|IOFA|IOFB|IOFS|IOFF, IOFR|IOFA|IOFB|IOFS|IOFF,
	IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS,
	IOFR|IOFA|IOFB|IOFS|IOFF, IOFR|IOFA|IOFB|IOFS|IOFF, // should this be floating
	IOFR|IOFA|IOFS, IOFR|IOFA|IOFS
};


// ********************************************
// * Operation Properties
// ********************************************

bool iop_t::isFloating() const {
	return iop_fields.at(id) & IOFF;
}

bool iop_t::usesResultParameter() const {
	return iop_fields.at(id) & IOFR;
}

bool iop_t::usesReadParameterA() const {
	return iop_fields.at(id) & IOFA;
}

bool iop_t::usesReadParameterB() const {
	return iop_fields.at(id) & IOFB;
}

bool iop_t::isJump() const {
	return iop_fields.at(id) & IOFJ;
}

bool iop_t::usesLabel() const {
	return iop_fields.at(id) & IOFL;
}

std::string iop_t::debugName() const {
	return iop_id_to_str.at(id);
}

void iop_t::setParameterVariable( int i, variable_t v ) {
	switch( i ) {
		case 0:
			r = v;
			break;
		case 1:
			a = v;
			break;
		case 2:
			b = v;
			break;
		default:
			throw;
	}
}

void iop_t::setParameterInteger( int i, int64_t x ) {
	switch( i ) {
		case 1:
			a = ERROR_VARIABLE;
			c_a.integer = x;
			break;
		case 2:
			b = ERROR_VARIABLE;
			c_b.integer = x;
			break;
		default:
			throw;
	}
}

void iop_t::setParameterFloating( int i, double x ) {
	switch( i ) {
		case 1:
			a = ERROR_VARIABLE;
			c_a.floating = x;
			break;
		case 2:
			b = ERROR_VARIABLE;
			c_b.floating = x;
			break;
		default:
			throw;
	}
}

variable_t iop_t::getParameterVariable( int i ) const {
	switch( i ) {
		case 0:
			return r;
		case 1:
			return a;
		case 2:
			return b;
		default:
			throw;
	}
}

int64_t iop_t::getParameterInteger( int i ) const {
	switch( i ) {
		case 1:
			return c_a.integer;
		case 2:
			return c_b.integer;
		default:
			throw;
	}
}

double iop_t::getParameterFloating( int i ) const {
	switch( i ) {
		case 1:
			return c_a.floating;
		case 2:
			return c_b.floating;
		default:
			throw;
	}
}


bool iop_t::parameterIsVariable( int i ) const {
	return getParameterVariable( i ) != ERROR_VARIABLE;
}

bool iop_t::parameterIsWritten( int i ) const {
	switch( i ) {
		case 0:
			return usesResultParameter() and id != id_t::IOP_INT_ARR_STORE;
		case 1:
			return false;
		case 2:
			return id == id_t::IOP_INT_MODDIV;
		default:
			throw;
	}
}

bool iop_t::parameterIsRead( int i ) const {
	switch( i ) {
		case 0:
			return usesResultParameter() and not ( iop_fields.at(id) & IOFS );
		case 1:
			return usesReadParameterA();
		case 2:
			return usesReadParameterB() and ( id != IOP_INT_MODDIV );
		default:
			throw;
	}
}

std::vector<variable_t> iop_t::getReadVariables() const {
	std::vector<variable_t> ret;
	for( int i = 0; i < 3; ++i )
		if( parameterIsRead( i ) and parameterIsVariable( i ) )
			ret.push_back( getParameterVariable( i ) );
	return ret;
}

std::vector<variable_t> iop_t::getWrittenVariables() const {
	std::vector<variable_t> ret;
	for( int i = 0; i < 3; ++i )
		if( parameterIsWritten( i ) and parameterIsVariable( i ) )
			ret.push_back( getParameterVariable( i ) );
	return ret;
}

std::vector<variable_t> iop_t::getReadOnlyVariables() const {
	std::vector<variable_t> ret;
	for( int i = 0; i < 3; ++i )
		if( parameterIsRead( i ) and ( not parameterIsWritten( i ) ) and parameterIsVariable( i ) )
			ret.push_back( getParameterVariable( i ) );
	return ret;
}
	
std::vector<variable_t> iop_t::getWriteOnlyVariables() const {
	std::vector<variable_t> ret;
	for( int i = 0; i < 3; ++i )
		if( ( not parameterIsRead( i ) ) and parameterIsWritten( i ) and parameterIsVariable( i ) )
			ret.push_back( getParameterVariable( i ) );
	return ret;
}

bool iop_t::operator==( const iop_t& other ) const {
	if( id != other.id )
		return false;
	if( usesResultParameter() and r != other.r )
		return false;
	if( usesReadParameterA() ) {
		if( a != other.a )
			return false;
		if( a == ERROR_VARIABLE and c_a.integer != other.c_a.integer )
			return false;
	}
	if( usesReadParameterB() ) {
		if( b != other.b )
			return false;
		if( b == ERROR_VARIABLE and c_b.integer != other.c_b.integer )
			return false;
	}
	if( usesLabel() and label != other.label )
		return false;
	return true;
}

void iop_t::negateCompare() {
	switch( id ) {
		case id_t::IOP_JT:
			id = id_t::IOP_JF;
			break;
		case id_t::IOP_JF:
			id = id_t::IOP_JT;
			break;
		case id_t::IOP_JE:
			id = id_t::IOP_JN;
			break;
		case id_t::IOP_JN:
			id = id_t::IOP_JE;
			break;
		case id_t::IOP_JL:
			id = id_t::IOP_JGE;
			break;
		case id_t::IOP_JG:
			id = id_t::IOP_JLE;
			break;
		case id_t::IOP_JLE:
			id = id_t::IOP_JG;
			break;
		case id_t::IOP_JGE:
			id = id_t::IOP_JL;
			break;
		case id_t::IOP_INT_EQ:
			id = id_t::IOP_INT_NEQ;
			break;
		case id_t::IOP_INT_NEQ:
			id = id_t::IOP_INT_EQ;
			break;
		case id_t::IOP_INT_L:
			id = id_t::IOP_INT_GE;
			break;
		case id_t::IOP_INT_G:
			id = id_t::IOP_INT_LE;
			break;
		case id_t::IOP_INT_LE:
			id = id_t::IOP_INT_G;
			break;
		case id_t::IOP_INT_GE:
			id = id_t::IOP_INT_L;
			break;
		default:;
	}
}

void iop_t::rotateCompare() {
	switch( id ) {
		case id_t::IOP_JL:
			id = id_t::IOP_JG;
			break;
		case id_t::IOP_JG:
			id = id_t::IOP_JL;
			break;
		case id_t::IOP_JLE:
			id = id_t::IOP_JGE;
			break;
		case id_t::IOP_JGE:
			id = id_t::IOP_JLE;
			break;
		case id_t::IOP_INT_L:
			id = id_t::IOP_INT_G;
			break;
		case id_t::IOP_INT_G:
			id = id_t::IOP_INT_L;
			break;
		case id_t::IOP_INT_LE:
			id = id_t::IOP_INT_GE;
			break;
		case id_t::IOP_INT_GE:
			id = id_t::IOP_INT_LE;
			break;
		default:
			return;
	}
	std::swap( a, b );
	std::swap( c_a, c_b );
}

void iop_t::legalizeCompare() {
	if( id >= id_t::IOP_JE and id <= id_t::IOP_JGE ) {
		if( a == ERROR_VARIABLE )
			rotateCompare();
	} else if( id >= id_t::IOP_INT_EQ and id <= id_t::IOP_INT_GE ) {
		if( a == ERROR_VARIABLE )
			rotateCompare();
	}
}

// ********************************************
// * Translation
// ********************************************

size_t intermediateCode::function::addOperation( iop_t::id_t type, variable_t r, variable_t a, variable_t b, label_t l, iop_t::constant_t c, iop_t::constant_t c2 ) {
	operations.push_back( iop_t{ type, r, a, b, l, c, c2 } );
	#ifdef SYNTAX_TREE_DEBUG
	std::cout << operations.back() << std::endl;
	#endif
	return operations.size()-1;
}

void intermediateCode::function::translateNode( const syntaxTree::node* n, loop_stack_t& loop_stack ) {
	#ifdef SYNTAX_TREE_DEBUG
	ic_out << "translateNode(" << n->type << ")" << std::endl;
	#endif
	switch( n->type ) {
		case N_EMPTY:
			addOperation( iop_t::id_t::IOP_NOP );
			break;
		case N_ASSIGN:
			translateAssign( n );
			break;
		case N_GARBAGE:
			translateGarbage( n );
			break;
		case N_ARGUMENT_LIST:
			translateArguments( n );
			break;
		case N_EQUALS: case N_NOT_EQUALS: case N_ADD: case N_SUBTRACT: case N_MULTIPLY: case N_DIVIDE: case N_REMAINDER: case N_UMIN: 
		case N_VARIABLE: case N_INTEGER: case N_FLOAT: case N_STRING: case N_LIST: case N_SET: case N_TUPLE:
			translateExpression( n );
			break;
		case N_IF: case N_WHILE: case N_FOR:
			translateBranching( n, loop_stack );
			break;
		case N_SEQUENTIAL_BLOCK: case N_PARALLEL_BLOCK:
			translateBlock( n, loop_stack );
			break;
		case N_FUNCTION_CALL:
			translateFunctionCall( n );
			break;
		case N_JOIN: case N_MEET:
			translateFunctionOperation( n );
			break;
		case N_BREAK: case N_CONTINUE: case N_RETURN:
			translateControlFlow( n, loop_stack );
			break;
		default:
			lerr << error_line(true) << "Operation ST(" << n->type << ") not yet implemented" << std::endl;
	}
}

variable_t intermediateCode::function::translateExpression( const syntaxTree::node* n ) {
	if( n->type == N_VARIABLE )
		return translateVariable( n );
	if( n->type == N_INTEGER or n->type == N_FLOAT or n->type == N_STRING )
		return translateConstant( n );
	if( n->type == N_LIST or n->type == N_SET or n->type == N_TUPLE )
		return translateContainer( n );
	if( n->type == N_FUNCTION_CALL )
		return translateFunctionCall( n );
	if( n->type == N_JOIN or n->type == N_MEET )
		return translateFunctionOperation( n );
	if( n->type == N_LIST_INDEXING or n->type == N_TUPLE_INDEXING or n->type == N_SIZE_OF )
		return translateReadIndexing( n );
	if( n->type == N_COMPARISON_CHAIN )
		return translateComparisonChain( n );
	if( n->type == N_FUNCTION_DEFINITION )
		return translateFunctionDefinition( n );
	return translateArithmetic( n );
}

void intermediateCode::function::translateAssign( const syntaxTree::node* n ) {
	assert( n->type == N_ASSIGN );
	translateLValue( n->children[0], translateExpression( n->children[1] ) );
}

void intermediateCode::function::translateLValue( const syntaxTree::node* n, variable_t value ) {
	assert( n->type == N_VARIABLE or n->type == N_LIST_INDEXING );
	if( n->type == N_VARIABLE ) 
		translateGeneralAssignment( n->data.integer, value );
	else if( n->type == N_LIST_INDEXING ) {
		variable_t index = translateExpression( n->children[1] );
		variable_t list = translateVariable( n->children[0] );
		variable_t offset = parent->newTemporaryVariable( INT_TYPE );
		addOperation( iop_t::id_t::IOP_INT_MOV, offset, index );
		addOperation( iop_t::id_t::IOP_INT_MULEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=n->data_type.rawSize()/8} );
		translateAssignToListElementWeak( list, offset, value );
	} else if( n->type == N_TUPLE_INDEXING ) {
		variable_t tup = translateVariable( n->children[0] );
		size_t offset = 0;
		for( int i = 0; i < n->data.integer; ++i )
			offset += n->data_type.getParameter( i ).rawSize();
		variable_t index = parent->newTemporaryVariable( INT_TYPE );
		addOperation( iop_t::id_t::IOP_INT_MOV, index, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=offset/8} );
		translateAssignToTupleElementWeak( tup, index, value );
	} else {
		lerr << error_line() << "Cannot assign to anything other than a variable" << std::endl;
	}
}

// ---------------------------------------------------------------------------------------------

void intermediateCode::function::translateAssignToListElementWeak( variable_t target, variable_t offset, variable_t source ) {
	// offset in bytes is $offset * 8
	type_t st = parent->scptab->getVariableType( source );
	if( st == INT_TYPE or st.isFunction() ) {
		addOperation( iop_t::id_t::IOP_INT_ARR_STORE, target, offset, source );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( st.isList() or st.isSet() ) { // weak list copy
		addOperation( iop_t::id_t::IOP_INT_ARR_STORE, target, offset, source );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( st == FLT_TYPE ) {
		addOperation( iop_t::id_t::IOP_FLT_ARR_STORE, target, offset, source );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( st == STR_TYPE ) {
		addOperation( iop_t::id_t::IOP_INT_ARR_STORE, target, offset, source ); // temp
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( st.isTuple() ) { // fix
		size_t c = st.rawSize();
		assert( c % 8 == 0 );
		c >>= 3;
		variable_t inter = parent->newTemporaryVariable( INT_TYPE );
		for( size_t i = 0; i < c; ++i ) {
			addOperation( iop_t::id_t::IOP_INT_TUP_LOAD, inter, source, ERROR_VARIABLE, ERROR_LABEL, {.integer=0}, {.integer=i} );
			addOperation( iop_t::id_t::IOP_INT_ARR_STORE, target, offset, inter );
			addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
		}		
	}
}

void intermediateCode::function::translateAssignFromListElementWeak( variable_t target, variable_t source, variable_t offset ) {
	type_t tt = parent->scptab->getVariableType( target );
	if( tt == INT_TYPE or tt.isFunction() )  {
		addOperation( iop_t::id_t::IOP_INT_ARR_LOAD, target, source, offset );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( tt.isList() or tt.isSet() ) {
		addOperation( iop_t::id_t::IOP_INT_ARR_LOAD, target, source, offset );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( tt == FLT_TYPE ) {
		addOperation( iop_t::id_t::IOP_FLT_ARR_LOAD, target, source, offset );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( tt == STR_TYPE ) {
		addOperation( iop_t::id_t::IOP_INT_ARR_LOAD, target, source, offset ); // temp
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( tt.isTuple() ) { // fix
		size_t c = tt.rawSize();
		assert( c % 8 == 0 );
		c >>= 3;
		variable_t inter = parent->newTemporaryVariable( INT_TYPE );
		for( size_t i = 0; i < c; ++i ) {
			addOperation( iop_t::id_t::IOP_INT_ARR_LOAD, inter, source, offset );
			addOperation( iop_t::id_t::IOP_INT_TUP_STORE, target, ERROR_VARIABLE, inter, ERROR_LABEL, {.integer=i} );
			addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
		}
	}
}

void intermediateCode::function::translateListToListAssignmentWeak( variable_t target, variable_t ot, variable_t source, variable_t os ) {
	// not fit for use yet
	type_t t = parent->scptab->getVariableType( target ).getChildType();
	variable_t inter = parent->newTemporaryVariable( t );
	translateAssignFromListElementWeak( inter, source, os );
	translateAssignToListElementWeak( target, ot, inter );
}

void intermediateCode::function::translateAssignToTupleElementWeak( variable_t target, variable_t offset, variable_t source ) {
	// offset in bytes is $offset * 8
	type_t st = parent->scptab->getVariableType( source );
	if( st == INT_TYPE or st.isFunction() or st.isList() or st.isSet() ) {
		addOperation( iop_t::id_t::IOP_INT_TUP_STORE, target, offset, source );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( st == FLT_TYPE ) {
		addOperation( iop_t::id_t::IOP_FLT_TUP_STORE, target, offset, source );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( st == STR_TYPE ) {
		addOperation( iop_t::id_t::IOP_INT_TUP_STORE, target, offset, source ); // temp
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( st.isTuple() ) { // fix
		size_t c = st.rawSize();
		assert( c % 8 == 0 );
		c >>= 3;
		variable_t inter = parent->newTemporaryVariable( INT_TYPE );
		for( size_t i = 0; i < c; ++i ) {
			addOperation( iop_t::id_t::IOP_INT_TUP_LOAD, inter, source, ERROR_VARIABLE, ERROR_LABEL, {.integer=0}, {.integer=i} );
			addOperation( iop_t::id_t::IOP_INT_TUP_STORE, target, offset, inter );
			addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
		}		
	}
}

void intermediateCode::function::translateAssignFromTupleElementWeak( variable_t target, variable_t source, variable_t offset ) {
	type_t tt = parent->scptab->getVariableType( source );
	if( tt == INT_TYPE or tt.isFunction() or tt.isList() or tt.isSet() ) {
		addOperation( iop_t::id_t::IOP_INT_TUP_LOAD, target, source, offset );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( tt == FLT_TYPE ) {
		addOperation( iop_t::id_t::IOP_FLT_TUP_LOAD, target, source, offset );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( tt == STR_TYPE ) {
		addOperation( iop_t::id_t::IOP_INT_TUP_LOAD, target, source, offset ); // temp
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
	} else if( tt.isTuple() ) { // fix
		size_t c = tt.rawSize();
		assert( c % 8 == 0 );
		c >>= 3;
		variable_t inter = parent->newTemporaryVariable( INT_TYPE );
		for( size_t i = 0; i < c; ++i ) {
			addOperation( iop_t::id_t::IOP_INT_TUP_LOAD, inter, source, offset );
			addOperation( iop_t::id_t::IOP_INT_TUP_STORE, target, ERROR_VARIABLE, inter, ERROR_LABEL, {.integer=i} );
			addOperation( iop_t::id_t::IOP_INT_ADDEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1} );
		}
	}
}

void intermediateCode::function::translateTupleToTupleAssignmentWeak( variable_t target, variable_t ot, variable_t source, variable_t os ) {
	type_t t = parent->scptab->getVariableType( target );
	variable_t inter = parent->newTemporaryVariable( t );
	translateAssignFromTupleElementWeak( inter, source, os );
	translateAssignToTupleElementWeak( target, ot, inter );
}


void intermediateCode::function::translateGeneralAssignment( variable_t target, variable_t source ) {
	type_t tt = parent->scptab->getVariableType( target );
	type_t st = parent->scptab->getVariableType( source );
	assert( tt == st );
	if( tt.isList() ) {
		addOperation( iop_t::id_t::IOP_INT_MOV, target, source );
	} else if( tt.isSet() ) {
		addOperation( iop_t::id_t::IOP_INT_MOV, target, source );
	} else if( tt == INT_TYPE or tt.isFunction() ) {
		addOperation( iop_t::id_t::IOP_INT_MOV, target, source );
	} else if( tt == FLT_TYPE ) {
		addOperation( iop_t::id_t::IOP_FLT_MOV, target, source );
	} else if( tt == STR_TYPE ) {
		addOperation( iop_t::id_t::IOP_STR_MOV, target, source );
	} else if( tt.isTuple() ) {
		variable_t ot = parent->newTemporaryVariable( INT_TYPE );
		variable_t os = parent->newTemporaryVariable( INT_TYPE );
		addOperation( iop_t::id_t::IOP_INT_MOV, ot, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=0} );
		addOperation( iop_t::id_t::IOP_INT_MOV, os, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=0} );
		translateTupleToTupleAssignmentWeak( target, ot, source, os );
	}
}

// ---------------------------------------------------------------------------------------------

void intermediateCode::function::translateGarbage( const syntaxTree::node* n ) {
	assert( n->type == N_GARBAGE );
	addOperation( iop_t::id_t::IOP_GARBAGE, variable_t( n->data.integer ) );
}

void intermediateCode::function::translateArguments( const syntaxTree::node* n ) {
	assert( n->type == N_ARGUMENT_LIST );
	variable_t a = translateExpression( n->children[0] );
	if( n->children[0]->data_type == FLT_TYPE )
		addOperation( iop_t::id_t::IOP_FLT_ADD_PARAM, ERROR_VARIABLE, a );
	else
		addOperation( iop_t::id_t::IOP_INT_ADD_PARAM, ERROR_VARIABLE, a );
	if( n->children[1] )
		translateArguments( n->children[1] );
}

variable_t intermediateCode::function::translateArithmetic( const syntaxTree::node* n ) {
	if( not ( n->type == N_ADD or n->type == N_SUBTRACT or n->type == N_MULTIPLY or n->type == N_DIVIDE or n->type == N_REMAINDER or n->type == N_UMIN ) ) {
		std::cerr << error_line() << "translateArithmetic received " << n->type << std::endl;
		throw;
	}
	variable_t s[2];
	for( int i = 0; i < 2; ++i )
		s[i] = translateExpression( n->children[i] );
	variable_t r = parent->newTemporaryVariable( parent->scptab->getVariableType( s[0] ) );
	iop_t::id_t id;
	if( n->data_type == INT_TYPE ) {
		if( n->type == N_NOT_EQUALS or n->type == N_EQUALS ) { // 3 argument ops
			lerr << error_line() << "Depricated block reached" << std::endl;
			/*if( n->type == N_NOT_EQUALS )
				id = iop_t::id_t::IOP_INT_NEQ;
			else if( n->type == N_EQUALS )
				id = iop_t::id_t::IOP_INT_EQ;
			addOperation( id, r, s[0], s[1] );
			return r;*/
		} else { // 2 argument ops
			addOperation( iop_t::id_t::IOP_INT_MOV, r, s[0] );
			if( n->type == N_ADD )
				id = iop_t::id_t::IOP_INT_ADDEQ;
			else if( n->type == N_SUBTRACT )
				id = iop_t::id_t::IOP_INT_SUBEQ;
			else if( n->type == N_MULTIPLY )
				id = iop_t::id_t::IOP_INT_MULEQ;
			else
				lerr << error_line() << "Arithmetic operation " << n->type << " not yet implemented for integers" << std::endl;
			addOperation( id, r, s[1] );
			return r;
		}
	} else if( n->data_type == FLT_TYPE ) {
		if( n->type == N_ADD )
			id = iop_t::id_t::IOP_FLT_ADD;
		else if( n->type == N_SUBTRACT )
			id = iop_t::id_t::IOP_FLT_SUB;
		else if( n->type == N_MULTIPLY )
			id = iop_t::id_t::IOP_FLT_MUL;
		else if( n->type == N_DIVIDE )
			id = iop_t::id_t::IOP_FLT_DIV;
		else
			lerr << error_line() << "Arithmetic operation " << n->type << " not yet implemented for integers" << std::endl;
		addOperation( id, r, s[0], s[1] );
		return r;
		
	}
	lerr << error_line() << "Arithmetic on type " << n->data_type << " not yet implemented" << std::endl;
}

variable_t intermediateCode::function::translateFunctionDefinition( const syntaxTree::node* n ) {
	assert( n->type == N_FUNCTION_DEFINITION );
	parent->defineFunction( n->data.integer, n->children[1] );
	variable_t r = parent->newTemporaryVariable( n->data_type );
	addOperation( iop_t::id_t::IOP_LABEL_TO_PTR, r, ERROR_VARIABLE, ERROR_VARIABLE, parent->getFunctionLabel( n->data.integer ) );
	return r;
}

void intermediateCode::function::translateBranching( const syntaxTree::node* n, loop_stack_t& loop_stack ) {
	assert( n->type == N_IF or n->type == N_WHILE or n->type == N_FOR );
	if( n->type == N_IF ) {
		variable_t a = translateExpression( n->children[0] );
		label_t endif = parent->newLabel();
		addOperation( iop_t::id_t::IOP_JF, ERROR_VARIABLE, a, ERROR_VARIABLE, endif );
		if( n->children[1]->type == N_ELSE ) {
			translateNode( n->children[1]->children[0], loop_stack );
			label_t endelse = parent->newLabel();
			addOperation( iop_t::id_t::IOP_JUMP, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, endelse );
			addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, endif );
			translateNode( n->children[1]->children[1], loop_stack );
			addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, endelse );
		} else {
			translateNode( n->children[1], loop_stack );
			addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, endif );
		}
	} else if( n->type == N_WHILE ) {
		label_t condition_check = parent->newLabel(), natural_exit = parent->newLabel();
		label_t abrupt_exit = n->children[1]->type == N_ELSE ? parent->newLabel() : natural_exit;
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, condition_check );
		variable_t exp = translateExpression( n->children[0] );
		addOperation( iop_t::id_t::IOP_JF, ERROR_VARIABLE, exp, ERROR_VARIABLE, natural_exit );
		loop_stack.push_back( std::make_pair( abrupt_exit, condition_check ) );
		if( n->children[1]->type == N_ELSE )
			translateNode( n->children[1]->children[0], loop_stack );
		else 
			translateNode( n->children[1], loop_stack );
		loop_stack.pop_back();
		addOperation( iop_t::id_t::IOP_JUMP, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, condition_check );
		if( n->children[1]->type == N_ELSE ) {
			addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, natural_exit );
			translateNode( n->children[1]->children[1], loop_stack );
		}
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, abrupt_exit );
	} else if( n->type == N_FOR ) {
		assert( n->children[0]->type == N_IN );
		std::cout << "FOR" << std::endl;
		bool has_else = n->children[1]->type == N_ELSE;
		variable_t v = translateVariable( n->children[0]->children[0] );
		variable_t l = translateContainer( n->children[0]->children[1] );
		variable_t s = parent->newTemporaryVariable( INT_TYPE );
		variable_t i = parent->newTemporaryVariable( INT_TYPE );
		label_t condition_check = parent->newLabel();
		label_t natural_exit = parent->newLabel();
		label_t abrupt_exit = has_else ? parent->newLabel() : natural_exit;
		loop_stack.push_back( std::make_pair( abrupt_exit, condition_check ) );
		addOperation( iop_t::id_t::IOP_INT_MOV, i, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=0} );
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, condition_check );
		addOperation( iop_t::id_t::IOP_LIST_SIZE, s, l );
		addOperation( iop_t::id_t::IOP_JGE, ERROR_VARIABLE, i, s, natural_exit );
		addOperation( iop_t::id_t::IOP_INT_ARR_LOAD, v, l, i );
		if( has_else )
			translateNode( n->children[1]->children[0], loop_stack );
		else
			translateNode( n->children[1], loop_stack );
		loop_stack.pop_back();
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, i, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=1 /*should be raw size*/} );
		addOperation( iop_t::id_t::IOP_JUMP, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, condition_check );
		if( has_else ) {
			addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, natural_exit );
			translateNode( n->children[1]->children[1], loop_stack );
		}
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, abrupt_exit );
	} else
		lerr << error_line() << "Branching operation " << n->type << " not yet implemented" << std::endl;
}

void intermediateCode::function::translateControlFlow( const syntaxTree::node* n, loop_stack_t& loop_stack ) {
	assert( n->type == N_BREAK or n->type == N_CONTINUE or n->type == N_RETURN );
	label_t target;
	if( n->type == N_RETURN ) {
		addOperation( iop_t::id_t::IOP_RETURN, ERROR_VARIABLE, translateExpression( n->children[0] ) );
		return;
	}
	// break or continue
	if( n->type == N_BREAK ) {
		int back_num = n->data.integer;
		assert( back_num > 0 );
		if( back_num > loop_stack.size() ) 
			lerr << error_line() << "There are not enough loops to break free from" << std::endl;
		target = loop_stack.at( loop_stack.size() - back_num ).first;
	} else{
		if( loop_stack.size() == 0 )
			lerr << error_line() << "There is no loop to continue" << std::endl;
		target = loop_stack.back().second;
	}
	addOperation( iop_t::id_t::IOP_JUMP, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, target );
}


void intermediateCode::function::translateSequentialBlock( const syntaxTree::node* n, loop_stack_t& loop_stack ) {
	for( int i = 0; i < 2; ++i )
		if( n->children[i] ) 
			if( n->children[i]->type == N_BLOCK_LIST )
				translateSequentialBlock( n->children[i], loop_stack );
			else
				translateNode( n->children[i], loop_stack );
}

void intermediateCode::function::translateBlock( const syntaxTree::node* n, loop_stack_t& loop_stack ) {
	assert( n->type == N_SEQUENTIAL_BLOCK or n->type == N_PARALLEL_BLOCK );
	if( n->type == N_SEQUENTIAL_BLOCK ) {
		translateSequentialBlock( n, loop_stack );
	} else
		lerr << error_line() << "Parallel blocks not yet implemented" << std::endl;
}

variable_t intermediateCode::function::translateFunctionCall( const syntaxTree::node* n ) {
	assert( n->type == N_FUNCTION_CALL );
	function_t f = n->data.integer;
	if( f == 0 ) { // function pointer, children[1] != nullptr
		assert( n->children[1] );
		type_t t = n->children[1]->data_type.getParameter(1);
		variable_t r = ( t != VOID_TYPE ) ? parent->newTemporaryVariable( t ) : ERROR_VARIABLE;
		if( r != ERROR_VARIABLE )
			addOperation( iop_t::id_t::IOP_RESERVE_RETURN, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=t.rawSize()} );
		if( n->children[0] )
			translateArguments( n->children[0] );
		addOperation( iop_t::id_t::IOP_FUNCTION, r, translateVariable( n->children[1] ), ERROR_VARIABLE, ERROR_LABEL, iop_t::constant_t{ .integer = 0 } );
		return r; 
	} else {
		type_t t = parent->scptab->getFunctionReturnType( f );
		variable_t r = ( t != VOID_TYPE ) ? parent->newTemporaryVariable( t ) : ERROR_VARIABLE;
		if( r != ERROR_VARIABLE )
			addOperation( iop_t::id_t::IOP_RESERVE_RETURN, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=t.rawSize()} );
		if( n->children[0] )
			translateArguments( n->children[0] );
		addOperation( iop_t::id_t::IOP_FUNCTION, r, ERROR_VARIABLE, ERROR_VARIABLE, parent->getFunctionLabel( f ), iop_t::constant_t{ .integer = 0 } );
		return r; 
	}
	
}

variable_t intermediateCode::function::translateFunctionOperation( const syntaxTree::node* n ) {
	assert( n->type == N_JOIN or n->type == N_MEET );
	variable_t r = parent->newTemporaryVariable( n->data_type );
	if( n->data_type == STR_TYPE ) {
		function_t f = JOIN_STR_FUNCTION; // others to be implemented
		addOperation( iop_t::id_t::IOP_RESERVE_RETURN, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=8} );
		variable_t a = translateExpression( n->children[0] );
		addOperation( iop_t::id_t::IOP_INT_ADD_PARAM, ERROR_VARIABLE, a );
		variable_t b = translateExpression( n->children[1] );
		addOperation( iop_t::id_t::IOP_INT_ADD_PARAM, ERROR_VARIABLE, b );
		addOperation( iop_t::id_t::IOP_FUNCTION, r, ERROR_VARIABLE, ERROR_VARIABLE, parent->getFunctionLabel( f ), iop_t::constant_t{ .integer = /*f*/ 0 } );
	} else if( n->data_type.isList() ) {
		label_t first_check = parent->newLabel();
		label_t second_check = parent->newLabel();
		label_t first_end = parent->newLabel();
		label_t second_end = parent->newLabel();
		variable_t sa = parent->newTemporaryVariable( INT_TYPE );
		variable_t sb = parent->newTemporaryVariable( INT_TYPE );
		variable_t sr = parent->newTemporaryVariable( INT_TYPE );
		variable_t i1 = parent->newTemporaryVariable( INT_TYPE );
		variable_t i2 = parent->newTemporaryVariable( INT_TYPE );
		variable_t temp = parent->newTemporaryVariable( n->data_type.getChildType() );
		variable_t a = translateExpression( n->children[0] );
		size_t qwords = n->data_type.getChildType().rawSize()/8;
		addOperation( iop_t::id_t::IOP_LIST_SIZE, sa, a );
		variable_t b = translateExpression( n->children[1] );
		addOperation( iop_t::id_t::IOP_LIST_SIZE, sb, b );
		addOperation( iop_t::id_t::IOP_INT_MOV, sr, sa );
		addOperation( iop_t::id_t::IOP_INT_ADDEQ, sr, sb );		
		addOperation( iop_t::id_t::IOP_INT_MULEQ, sa, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=qwords} );
		addOperation( iop_t::id_t::IOP_INT_MULEQ, sb, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=qwords} );
		addOperation( iop_t::id_t::IOP_LIST_ALLOCATE, r, sr );
		addOperation( iop_t::id_t::IOP_INT_MOV, i1, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=0} );
		addOperation( iop_t::id_t::IOP_INT_MOV, i2, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=0} );		
		
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, first_check );
		addOperation( iop_t::id_t::IOP_JGE, ERROR_VARIABLE, i1, sa, first_end );
		translateAssignFromListElementWeak( temp, a, i1 );
		translateAssignToListElementWeak( r, i2, temp );
		addOperation( iop_t::id_t::IOP_JUMP, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, first_check );
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, first_end );

		addOperation( iop_t::id_t::IOP_INT_MOV, i1, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=0});
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, second_check );
		addOperation( iop_t::id_t::IOP_JGE, ERROR_VARIABLE, i1, sb, second_end );
		translateAssignFromListElementWeak( temp, b, i1 );
		translateAssignToListElementWeak( r, i2, temp );
		addOperation( iop_t::id_t::IOP_JUMP, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, second_check );
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, second_end );
	} else
		lerr << error_line() << "Operators join and meet are not implemented for type " << n->children[0]->data_type << std::endl;
	return r;
}

variable_t intermediateCode::function::translateVariable( const syntaxTree::node* n ) {
	assert( n->type == N_VARIABLE );
	return variable_t( n->data.integer );
}

variable_t intermediateCode::function::translateConstant( const syntaxTree::node* n ) {
	assert( n->type == N_INTEGER or n->type == N_FLOAT or n->type == N_STRING );
	variable_t r;
	if( n->type == N_INTEGER ) {
		r = parent->newTemporaryVariable( INT_TYPE );
		addOperation( iop_t::id_t::IOP_INT_MOV, r, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, iop_t::constant_t{ .integer = n->data.integer } );
	} else if( n->type == N_STRING ) {
		r = parent->newTemporaryVariable( STR_TYPE );
		addOperation( iop_t::id_t::IOP_STR_MOV, r, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, iop_t::constant_t{ .string = n->data.string } );
	} else  {
		r = parent->newTemporaryVariable( FLT_TYPE );
		addOperation( iop_t::id_t::IOP_FLT_MOV, r, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, iop_t::constant_t{ .floating = n->data.floating } );
	}
	return r;	
}

variable_t intermediateCode::function::translateContainer( const syntaxTree::node* n ) {
	if( n->type == N_LIST ) {
		// size_t s = n->data_type.getChildType().rawSize();
		variable_t r = parent->newTemporaryVariable( n->data_type );
		variable_t o = parent->newTemporaryVariable( INT_TYPE );
		addOperation( iop_t::id_t::IOP_LIST_ALLOCATE, r, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, { .integer = /*s**/ n->data.integer } ); 
		addOperation( iop_t::id_t::IOP_INT_MOV, o, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=0} );
		translateListElements( n->children[0], r, o );
		return r;
	} else if( n->type == N_TUPLE ) {
		variable_t r = parent->newTemporaryVariable( n->data_type );
		translateTupleElements( n, r, 0 );
		return r;
	}
	lerr << error_line() << "Sets not yet implemented" << std::endl;
}

void intermediateCode::function::translateListElements( const syntaxTree::node* n, variable_t r, variable_t o ) {
	assert( n->type == N_SINGLE_TYPE_EXPRESSION_LIST );
	variable_t x = translateExpression( n->children[0] );
	translateAssignToListElementWeak( r, o, x );
	if( n->children[1] )
		translateListElements( n->children[1], r, o );
}

void intermediateCode::function::translateTupleElements( const syntaxTree::node* n, variable_t r, size_t current_offset ) {
	assert( n->type == N_TUPLE or n->type == N_TUPLE_LIST );
	variable_t x = translateExpression( n->children[0] );
	addOperation( iop_t::id_t::IOP_INT_TUP_STORE, r, ERROR_VARIABLE, x, ERROR_LABEL, {.integer = current_offset} );
	if( n->children[1] )
		translateTupleElements( n->children[1], r, current_offset + n->children[0]->data_type.rawSize()/8 );
}

variable_t intermediateCode::function::translateReadIndexing( const syntaxTree::node* n ) {
	#ifdef SYNTAX_TREE_DEBUG
	std::cout << "translateReadIndexing" << std::endl;
	#endif
	assert( n->type == N_LIST_INDEXING or n->type == N_TUPLE_INDEXING or n->type == N_SIZE_OF );
	variable_t r = parent->newTemporaryVariable( n->data_type );
	if( n->type == N_LIST_INDEXING ) {
		variable_t list = translateExpression( n->children[0] );
		variable_t index = translateExpression( n->children[1] );
		variable_t offset = parent->newTemporaryVariable( INT_TYPE );
		addOperation( iop_t::id_t::IOP_INT_MOV, offset, index );
		addOperation( iop_t::id_t::IOP_INT_MULEQ, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=n->data_type.rawSize()/8} );
		translateAssignFromListElementWeak( r, list, offset );
		return r;
	} else if( n->type == N_TUPLE_INDEXING ) {
		variable_t tuple = translateExpression( n->children[0] );
		size_t index = n->data.integer;
		size_t base_offset = 0;
		for( int i = 0; i < index; ++i )
			base_offset += n->children[0]->data_type.getParameter( i ).rawSize();
		variable_t offset = parent->newTemporaryVariable( INT_TYPE );
		addOperation( iop_t::id_t::IOP_INT_MOV, offset, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=base_offset/8} );
		translateAssignFromTupleElementWeak( r, tuple, offset );
	} else {
		variable_t list = translateExpression( n->children[0] );
		addOperation( iop_t::id_t::IOP_LIST_SIZE, r, list );
	}
	return r;
}

variable_t intermediateCode::function::translateComparisonChain( const syntaxTree::node* n ) {
	assert( n->type == N_COMPARISON_CHAIN );
	return translateComparison( n->children[0] ).second;
}

std::pair<variable_t,variable_t> intermediateCode::function::translateComparison( const syntaxTree::node* n ) {
	// (child_left, comparison_result)
	variable_t left = translateExpression( n->children[0] );
	variable_t result = parent->newTemporaryVariable( BOOL_TYPE );
	if( n->children[1]->type < N_EQUALS or n->children[1]->type > N_GREATER_EQ ) {
		variable_t right = translateExpression( n->children[1] );
		addOperation( iop_t::id_t( iop_t::id_t::IOP_INT_EQ + n->type - N_EQUALS ), result, left, right );
	} else {
		auto p = translateComparison( n->children[1] );
		variable_t right = p.first;
		addOperation( iop_t::id_t( iop_t::id_t::IOP_INT_EQ + n->type - N_EQUALS ), result, left, right );
		addOperation( iop_t::id_t::IOP_INT_ANDEQ, result, p.second );
	}
	return std::make_pair( left, result );
}

void intermediateCode::declareFunction( function_t f ) {
	assert( f == functions.size() );
	label_t l = newLabel();
	assert( l == f );
	functions.push_back( function{ scptab->getFunctionSymbol( f ), f, l, this } );
}

void intermediateCode::defineFunction( function_t f, const syntaxTree::node* n ) {
	loop_stack_t loop_stack;
	functions.at( f ).translateNode( n, loop_stack );
}

label_t intermediateCode::getFunctionLabel( function_t f ) const {
	return functions.at( f ).label;
}

label_t intermediateCode::newLabel() {
	return label_count++;
}

variable_t intermediateCode::newTemporaryVariable( type_t type ) {
	return scptab->addVariable( GLOBAL_SCOPE, symtab->addTemporarySymbol(), type );
}

const intermediateCode::function& intermediateCode::getFunction( function_t f ) const {
	return functions.at(f);
}

const std::vector<intermediateCode::function>& intermediateCode::getFunctions() const {
	return functions;
}

intermediateCode::intermediateCode( scopeTable* t ) {
	scptab = t;
	label_count = 0;
	assert( iop_id_to_str.size() == iop_t::id_t::COUNT );
	assert( iop_t::iop_fields.size() == iop_t::id_t::COUNT );
	for( function_t f = 0; f < scptab->getFunctionCount(); ++f )
		declareFunction( f );
}

type_t intermediateCode::function::getReturnType() const {
	return parent->scptab->getFunctionReturnType( id );
}

type_t intermediateCode::function::getArgumentType( size_t i ) const {
	return parent->scptab->getVariableType( parent->scptab->getFunctionArguments( id ).at(i) );
}

const std::vector<variable_t>& intermediateCode::function::getArguments() const {
	return parent->scptab->getFunctionArguments( id );
}

// ********************************************
// * Strength Reduction
// ********************************************

bool iop_t::reduceStrength() {
	if( usesReadParameterA() and parameterIsVariable( 1 ) )
		return false;
	if( usesReadParameterB() and parameterIsVariable( 2 ) )
		return false;
	bool j = isJump();
	switch( id ) {
		case id_t::IOP_JF:
			id = ( c_a.integer & 1 ) ? id_t::IOP_NOP : id_t::IOP_JUMP;
			break;
		case id_t::IOP_JT:
			id = (! ( c_a.integer & 1 ) ) ? id_t::IOP_NOP : id_t::IOP_JUMP;
			break;
		case id_t::IOP_JE:
			id = ( c_a.integer == c_b.integer ) ? id_t::IOP_JUMP : id_t::IOP_NOP;
			break;
		case id_t::IOP_JN:
			id = ( c_a.integer != c_b.integer ) ? id_t::IOP_JUMP : id_t::IOP_NOP;
			break;
		case id_t::IOP_JL:
			id = ( c_a.integer <  c_b.integer ) ? id_t::IOP_JUMP : id_t::IOP_NOP;
			break;
		case id_t::IOP_JG:
			id = ( c_a.integer >  c_b.integer ) ? id_t::IOP_JUMP : id_t::IOP_NOP;
			break;
		case id_t::IOP_JLE:
			id = ( c_a.integer <= c_b.integer ) ? id_t::IOP_JUMP : id_t::IOP_NOP;
			break;
		case id_t::IOP_JGE:
			id = ( c_a.integer >= c_b.integer ) ? id_t::IOP_JUMP : id_t::IOP_NOP;
			break;
		case id_t::IOP_INT_EQ:
			c_a.integer = ( c_a.integer == c_b.integer );
			break;
		case id_t::IOP_INT_NEQ:
			c_a.integer = ( c_a.integer != c_b.integer );
			break;
		case id_t::IOP_INT_L:
			c_a.integer = ( c_a.integer < c_b.integer );
			break;
		case id_t::IOP_INT_G:
			c_a.integer = ( c_a.integer > c_b.integer );
			break;
		case id_t::IOP_INT_LE:
			c_a.integer = ( c_a.integer <= c_b.integer );
			break;
		case id_t::IOP_INT_GE:
			c_a.integer = ( c_a.integer >= c_b.integer );
			break;
		case id_t::IOP_INT_ANDEQ:
			if( c_a.integer != 0 )
				return false;
			break;
		case id_t::IOP_INT_OREQ: case id_t::IOP_INT_XOREQ: case IOP_INT_ANDNEQ:
			if( c_a.integer == 0 ) {
				id = id_t::IOP_NOP;
				return true;
			}
			return false;
		default:
			return false;
	}
	if( not j )
		id = id_t::IOP_INT_MOV;
	else 
		a = b = ERROR_VARIABLE;
	return true;
}

bool iop_t::reduceStrengthEq( int64_t i ) {
	if( usesReadParameterA() and parameterIsVariable( 1 ) )
		return false;
	if( usesReadParameterB() and parameterIsVariable( 2 ) )
		return false;
	switch( id ) {
		case id_t::IOP_INT_ANDEQ:
			c_a.integer &= i;
			break;
		case id_t::IOP_INT_OREQ:
			c_a.integer |= i;
			break;
		case id_t::IOP_INT_XOREQ:
			c_a.integer ^= i;
			break;
		case id_t::IOP_INT_ANDNEQ:
			c_a.integer &= ~i;
			break;
		case id_t::IOP_INT_ADDEQ:
			c_a.integer += i;
			break;
		case id_t::IOP_INT_SUBEQ:
			c_a.integer = i - c_a.integer;
			break;
		case id_t::IOP_INT_MULEQ: // moddiv is weird
			c_a.integer *= i;
			break;
		default:
			return false;
	}
	id = id_t::IOP_INT_MOV;
	return true;
}

// ********************************************
// * I/O
// ********************************************

void escape( char c, std::string& out ) {
	if( c == '\n' )
		out += "\\n";
	else if( c == '\t' )
		out += "\\t";
	else
		out.push_back( c );
}

std::string shorthand( std::string in ) {
	std::string out;
	if( in.size() < 7 )
		return in;
	for( int i = 0; i < 5; ++i )
		escape( in.at(i), out );
	out += "..";
	return out;
}

std::ostream& operator<<( std::ostream& os, iop_t o ) {
	const int w = 18;
	os << std::setw(w) << std::left << iop_id_to_str[o.id];
	std::string x;
	if( o.usesResultParameter() and (o.id != iop_t::id_t::IOP_FUNCTION or o.r != 0) ) {
		x = symtab->getName( scptab->getVariableSymbol( o.r ) );
		x.resize( 7, ' ' );
	} else			
		x = "       ";
	os << x << " ";
	if( o.usesReadParameterA() ) {
		if( o.a != 0 )
			x = symtab->getName( scptab->getVariableSymbol( o.a ) );
		else if( o.isFloating() )
			x = std::to_string( o.c_a.floating );
		else if( o.id != iop_t::id_t::IOP_STR_MOV )
			x = std::to_string( o.c_a.integer );
		else
			x = shorthand( std::string( o.c_a.string ) );
		x.resize( 7, ' ' );
	} else			
		x = "       ";
	os << x << " ";
	if( o.usesReadParameterB() ) {
		if( o.b != 0 )
			x = symtab->getName( scptab->getVariableSymbol( o.b ) );
		else if( o.isFloating() )
			x = std::to_string( o.c_b.floating );
		else
			x = std::to_string( o.c_b.integer );
		x.resize( 7, ' ' );
	} else			
		x = "       ";
	os << x << " ";
	if( o.usesLabel() )
		os << std::setw(7) << o.label;
	return os;
}

std::ostream& operator<<( std::ostream& os, const intermediateCode::function& f ) {
	const int w = 18;
	os << std::setw(3) << f.id <<  "| " << std::setw(w) << symtab->getName( f.symbol ) << "r       a       b       l\n---+-------------------------------------------------\n";
	size_t i = 0;
	for( const auto& o : f.operations )
		os << std::setw(3) << (i++) << "| " << o << std::endl;
	return os;
}

std::ostream& operator<<( std::ostream& os, const intermediateCode& ic ) {
	os << std::setfill(' ') << std::left;
	for( const auto& f : ic.functions )
		if( f.id >= GLOBAL_FUNCTION )
			os << f << std::endl;
	return os;
}