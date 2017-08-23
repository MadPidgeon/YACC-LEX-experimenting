#include <iomanip>
#include <stack>
#include "intermediate_code.h"

const std::vector<std::string> iop_id_to_str = {
	"NOP",
	"GARBAGE",
	"INT_MOVE",
	"STR_MOVE",
	"LABEL",
	"JUMP", 
	"JT", "JF",
	"JE", "JN", "JL", "JG", "JLE", "JGE",
	"FJ",
	"ADD_PARAM", "RESERVE_RETURN",
	"FUNCTION", 
	"RETURN",
	"INT_ANDEQ", "INT_OREQ", "INT_XOREQ", "INT_NANDEQ",
	"INT_ADDEQ", "INT_SUBEQ", "INT_MULEQ", 
	"INT_MODDIV",
	"FLT_ADDEQ", "FLT_SUBEQ", "FLT_MULEQ", "FLT_DIVEQ",
	"STR_CONEQ",
	"INT_EQ", "INT_NEQ", "INT_L", "INT_G", "INT_LE", "INT_GE",
	"INT_ARR_LOAD", "INT_ARR_STORE",
	"LIST_ALLOCATE"
};

const std::vector<uint8_t> iop_fields = {
	0,
	IOFR|IOFS,
	IOFR|IOFA|IOFS,
	IOFR|IOFA|IOFS,
	IOFL,
	IOFL|IOFJ,
	IOFA|IOFL|IOFJ, IOFA|IOFL|IOFJ,
	IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ, IOFA|IOFB|IOFL|IOFJ,
	IOFR|IOFL|IOFJ,
	IOFA, IOFA,
	IOFR|IOFL|IOFS,
	IOFA,
	IOFR|IOFA, IOFR|IOFA, IOFR|IOFA ,IOFR|IOFA,
	IOFR|IOFA, IOFR|IOFA, IOFR|IOFA,
	IOFR|IOFA|IOFB,
	IOFR|IOFA|IOFF, IOFR|IOFA|IOFF, IOFR|IOFA|IOFF, IOFR|IOFA|IOFF,
	IOFR|IOFA,
	IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB|IOFS,
	IOFR|IOFA|IOFB|IOFS, IOFR|IOFA|IOFB,
	IOFR|IOFA|IOFS
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

void iop_t::invertJump() {
	assert( isJump() );
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
		default:;
	}
}

// ********************************************
// * Translation
// ********************************************

size_t intermediateCode::function::addOperation( iop_t::id_t type, variable_t r, variable_t a, variable_t b, label_t l, iop_t::constant_t c, iop_t::constant_t c2 ) {
	operations.push_back( iop_t{ type, r, a, b, l, c, c2 } );
	return operations.size()-1;
}

void intermediateCode::function::translateNode( const syntaxTree::node* n, loop_stack_t& loop_stack ) {
	#ifdef SYNTAX_TREE_DEBUG
	std::cout << "translateNode(" << n->type << "):";
	n->print( std::cout, 0, 1 );
	std::cout << std::endl;
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
		case N_VARIABLE: case N_INTEGER: case N_FLOAT: case N_STRING: case N_LIST: case N_SET:
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
		case N_BREAK: case N_CONTINUE:
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
	if( n->type == N_LIST or n->type == N_SET )
		return translateContainer( n );
	if( n->type == N_FUNCTION_CALL )
		return translateFunctionCall( n );
	if( n->type == N_JOIN or n->type == N_MEET )
		return translateFunctionOperation( n );
	if( n->type == N_LIST_INDEXING )
		return translateReadIndexing( n );
	if( n->type == N_COMPARISON_CHAIN )
		return translateComparisonChain( n );
	return translateArithmetic( n );
}

void intermediateCode::function::translateAssign( const syntaxTree::node* n ) {
	assert( n->type == N_ASSIGN );
	translateLValue( n->children[0], translateExpression( n->children[1] ) );
}

void intermediateCode::function::translateLValue( const syntaxTree::node* n, variable_t value ) {
	assert( n->type == N_VARIABLE or n->type == N_LIST_INDEXING );
	if( n->type == N_VARIABLE )
		addOperation( iop_t::id_t::IOP_INT_MOV, translateVariable( n ), value );
	else if( n->type == N_LIST_INDEXING ) {
		variable_t index = translateExpression( n->children[1] );
		variable_t list = translateVariable( n->children[0] );
		addOperation( iop_t::id_t::IOP_INT_ARR_STORE, list, index, value );
	}
}

void intermediateCode::function::translateGarbage( const syntaxTree::node* n ) {
	assert( n->type == N_GARBAGE );
	addOperation( iop_t::id_t::IOP_GARBAGE, variable_t( n->data.integer ) );
}

void intermediateCode::function::translateArguments( const syntaxTree::node* n ) {
	assert( n->type == N_ARGUMENT_LIST );
	variable_t a = translateExpression( n->children[0] );
	addOperation( iop_t::id_t::IOP_ADD_PARAM, ERROR_VARIABLE, a );
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
	} else
		lerr << error_line() << "Arithmetic on type " << n->data_type << " not yet implemented" << std::endl;
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
	} else 
		lerr << error_line() << "Branching operation " << n->type << " not yet implemented" << std::endl;
}

void intermediateCode::function::translateControlFlow( const syntaxTree::node* n, loop_stack_t& loop_stack ) {
	assert( n->type == N_BREAK or n->type == N_CONTINUE );
	label_t target;
	if( n->type == N_BREAK ) {
		int back_num = n->data.integer;
		assert( back_num > 0 );
		if( back_num > loop_stack.size() ) 
			lerr << error_line() << "There are not enough loops to break free from" << std::endl;
		target = loop_stack.at( loop_stack.size() - back_num ).first;
	} else {
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
	type_t t = parent->scptab->getFunctionReturnType( f );
	variable_t r = ( t != VOID_TYPE ) ? parent->newTemporaryVariable( t ) : ERROR_VARIABLE;
	if( r != ERROR_VARIABLE )
		addOperation( iop_t::id_t::IOP_RESERVE_RETURN, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, {.integer=t.rawSize()} );
	if( n->children[0] )
		translateArguments( n->children[0] );
	addOperation( iop_t::id_t::IOP_FUNCTION, r, ERROR_VARIABLE, ERROR_VARIABLE, parent->getFunctionLabel( f ), iop_t::constant_t{ .integer = f } );
	return r; 
}

variable_t intermediateCode::function::translateFunctionOperation( const syntaxTree::node* n ) {
	assert( n->type == N_JOIN or n->type == N_MEET );
	function_t f = JOIN_STR_FUNCTION; // others to be implemented
	addOperation( iop_t::id_t::IOP_RESERVE_RETURN );
	variable_t a = translateExpression( n->children[0] );
	addOperation( iop_t::id_t::IOP_ADD_PARAM, ERROR_VARIABLE, a );
	variable_t b = translateExpression( n->children[1] );
	addOperation( iop_t::id_t::IOP_ADD_PARAM, ERROR_VARIABLE, b );
	variable_t r = parent->newTemporaryVariable( n->data_type );
	addOperation( iop_t::id_t::IOP_FUNCTION, r, ERROR_VARIABLE, ERROR_VARIABLE, parent->getFunctionLabel( f ), iop_t::constant_t{ .integer = f } );
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
	} else 
		lerr << error_line() << "Floating-point constants not yet implemented" << std::endl;
	return r;	
}

variable_t intermediateCode::function::translateContainer( const syntaxTree::node* n ) {
	if( n->type == N_LIST ) {
		size_t s = n->data_type.getChildType().rawSize();
		variable_t r = parent->newTemporaryVariable( n->data_type );
		addOperation( iop_t::id_t::IOP_LIST_ALLOCATE, r, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_LABEL, { .integer = s*n->data.integer } ); 
		translateListElements( n->children[0], r, n->data.integer );
		return r;
	}
	lerr << error_line() << "Sets not yet implemented" << std::endl;
}

void intermediateCode::function::translateListElements( const syntaxTree::node* n, variable_t r, size_t abstract_size ) {
	assert( n->type == N_SINGLE_TYPE_EXPRESSION_LIST );
	variable_t x = translateExpression( n->children[0] );
	addOperation( iop_t::id_t::IOP_INT_ARR_STORE, r, ERROR_VARIABLE, x, ERROR_LABEL, {.integer=abstract_size-1-n->data.integer /*1337*/} );
	if( n->children[1] )
		translateListElements( n->children[1], r, abstract_size );
}

variable_t intermediateCode::function::translateReadIndexing( const syntaxTree::node* n ) {
	variable_t list = translateExpression( n->children[0] );
	variable_t offset = translateExpression( n->children[1] );
	variable_t r = parent->newTemporaryVariable( n->data_type );
	addOperation( iop_t::id_t::IOP_INT_ARR_LOAD, r, list, offset );
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
	assert( iop_fields.size() == iop_t::id_t::COUNT );
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
	if( in.size() < 5 )
		return in;
	for( int i = 0; i < 3; ++i )
		escape( in.at(i), out );
	out += "..";
	return out;
}

std::ostream& operator<<( std::ostream& os, iop_t o ) {
	const int w = 18;
	os << std::setw(w) << iop_id_to_str[o.id];
	std::string x;
	if( (IOFR & iop_fields.at( o.id )) and (o.id != iop_t::id_t::IOP_FUNCTION or o.r != 0) ) {
		x = symtab->getName( scptab->getVariableSymbol( o.r ) );
		x.resize( 5, ' ' );
	} else			
		x = "     ";
	os << x << " ";
	if( IOFA & iop_fields.at( o.id ) ) {
		if( o.a != 0 )
			x = symtab->getName( scptab->getVariableSymbol( o.a ) );
		else if( o.id != iop_t::id_t::IOP_STR_MOV )
			x = std::to_string( o.c_a.integer );
		else
			x = shorthand( std::string( o.c_a.string ) );
		x.resize( 5, ' ' );
	} else			
		x = "     ";
	os << x << " ";
	if( IOFB & iop_fields.at( o.id ) ) {
		if( o.b != 0 )
			x = symtab->getName( scptab->getVariableSymbol( o.b ) );
		else
			x = std::to_string( o.c_b.integer );
		x.resize( 5, ' ' );
	} else			
		x = "     ";
	os << x << " ";
	if( IOFL & iop_fields.at( o.id ) )
		os << std::setw(4) << o.label;
	return os;
}

std::ostream& operator<<( std::ostream& os, const intermediateCode::function& f ) {
	const int w = 18;
	os << std::setw(3) << f.id <<  "| " << std::setw(w) << symtab->getName( f.symbol ) << "r     a     b     l\n---+-----------------------------------------\n";
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