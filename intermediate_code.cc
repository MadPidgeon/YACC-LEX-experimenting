#include <iomanip>
#include "intermediate_code.h"

const std::vector<std::string> iop_id_to_str = {
	"NOP",
	"INT_MOVE",
	"STR_MOVE",
	"LABEL",
	"JUMP", 
	"JE", "JN", "JL", "JG", "JLE", "JGE",
	"FJ",
	"ADD_PARAM",
	"FUNCTION", 
	"RETURN",
	"INT_ADDEQ", "INT_SUBEQ", "INT_MULEQ", 
	"INT_MODDIV",
	"FLT_ADDEQ", "FLT_SUBEQ", "FLT_MULEQ", "FLT_DIVEQ",
	"STR_CONEQ",
	"INT_EQ", "INT_NEQ",
	"INT_ARR_LOAD", "INT_ARR_STORE"
};

const std::vector<uint8_t> iop_fields = {
	0,
	IOFR|IOFA,
	IOFR|IOFA,
	IOFL,
	IOFL,
	IOFA|IOFL, IOFA|IOFL, IOFA|IOFL, IOFA|IOFL, IOFA|IOFL, IOFA|IOFL,
	IOFR|IOFL,
	IOFA,
	IOFR|IOFL,
	IOFA,
	IOFR|IOFA, IOFR|IOFA, IOFR|IOFA,
	IOFR|IOFA|IOFB,
	IOFR|IOFA|IOFF, IOFR|IOFA|IOFF, IOFR|IOFA|IOFF, IOFR|IOFA|IOFF,
	IOFR|IOFA,
	IOFR|IOFA, IOFR|IOFA,
	IOFR|IOFA|IOFB, IOFR|IOFA|IOFB
};

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

std::string iop_t::debugName() const {
	return iop_id_to_str.at(id);
}

std::vector<variable_t> iop_t::getReadVariables() const {
	uint8_t f = IOFA;
	std::vector<variable_t> ret;
	if( iop_fields[id] & IOFR ) {
		if( id != IOP_INT_MOV and id != IOP_STR_MOV and id != IOP_FUNCTION )
			ret.push_back( r );
	}
	for( variable_t x : { a, b } ) {
		if(  x != 0 and ( iop_fields[id] & f ) )
			ret.push_back( x );
		f <<= 1;
	}
	return ret;
}

std::vector<variable_t> iop_t::getWrittenVariables() const {
	if( id == IOP_INT_MODDIV )
		return { r, b };
	if( id == IOP_FUNCTION and r == ERROR_VARIABLE )
		return {};
	if( iop_fields[id] & IOFR )
		return { r };
	return {};
}

std::vector<variable_t> iop_t::getReadOnlyVariables() const {
	if( id == IOP_INT_MODDIV )
		return { a };
	if( id == IOP_FUNCTION )
		return {};
	uint8_t f = IOFA;
	std::vector<variable_t> ret;
	for( variable_t x : { a, b } ) {
		if( x != 0 and ( iop_fields[id] & f ) )
			ret.push_back( x );
		f <<= 1;
	}
	return ret;
}
	
std::vector<variable_t> iop_t::getWriteOnlyVariables() const {
	if( id == IOP_INT_MODDIV )
		return {b};
	if( id == IOP_FUNCTION and r != ERROR_VARIABLE )
		return {r};
	if( id == IOP_INT_MOV || id == IOP_STR_MOV )
		return {r};
	return {};
}

size_t intermediateCode::function::addOperation( iop_t::id_t type, variable_t r, variable_t a, variable_t b, label_t l, iop_t::constant_t c ) {
	operations.push_back( iop_t{ type, r, a, b, l, c } );
	return operations.size()-1;
}

void intermediateCode::function::translateNode( const syntaxTree::node* n ) {
	switch( n->type ) {
		case N_EMPTY:
			addOperation( iop_t::id_t::IOP_NOP );
		break;
		case N_ASSIGN:
			translateAssign( n );
		break;
		case N_ARGUMENT_LIST:
			translateArguments( n );
		break;
		case N_EQUALS: case N_NOT_EQUALS: case N_ADD: case N_SUBTRACT: case N_MULTIPLY: case N_DIVIDE: case N_REMAINDER: case N_UMIN: 
		case N_VARIABLE: case N_INTEGER: case N_FLOAT: case N_STRING:
			translateExpression( n );
		break;
		case N_IF: case N_WHILE: case N_FOR:
			translateBranching( n );
		break;
		case N_SEQUENTIAL_BLOCK: case N_PARALLEL_BLOCK:
			translateBlock( n );
		break;
		case N_FUNCTION_CALL:
			translateFunctionCall( n );
		break;
		default:
			lerr << error_line(true) << "Operation " << n->type << " not yet implemented" << std::endl;
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
	return translateArithmetic( n );
}

void intermediateCode::function::translateAssign( const syntaxTree::node* n ) {
	assert( n->type == N_ASSIGN );
	variable_t s[2] = { translateVariable( n->children[0] ), translateExpression( n->children[1] ) };
	addOperation( iop_t::id_t::IOP_INT_MOV, s[0], s[1] );
}

void intermediateCode::function::translateArguments( const syntaxTree::node* n ) {
	assert( n->type == N_ARGUMENT_LIST );
	variable_t a = translateExpression( n->children[0] );
	addOperation( iop_t::id_t::IOP_ADD_PARAM, ERROR_VARIABLE, a );
	if( n->children[1] )
		translateArguments( n->children[1] );
}

variable_t intermediateCode::function::translateArithmetic( const syntaxTree::node* n ) {
	assert( n->type == N_EQUALS or n->type == N_NOT_EQUALS or n->type == N_ADD or n->type == N_SUBTRACT or n->type == N_MULTIPLY or n->type == N_DIVIDE or n->type == N_REMAINDER or n->type == N_UMIN );
	variable_t s[2];
	for( int i = 0; i < 2; ++i )
		s[i] = translateExpression( n->children[i] );
	variable_t r = parent->newTemporaryVariable( parent->scptab->getVariableType( s[0] ) );
	iop_t::id_t id;
	addOperation( iop_t::id_t::IOP_INT_MOV, r, s[0] );
	if( n->data_type == INT_TYPE ) {
		if( n->type == N_ADD )
			id = iop_t::id_t::IOP_INT_ADDEQ;
		else if( n->type == N_SUBTRACT )
			id = iop_t::id_t::IOP_INT_SUBEQ;
		else if( n->type == N_MULTIPLY )
			id = iop_t::id_t::IOP_INT_MULEQ;
		else if( n->type == N_NOT_EQUALS )
			id = iop_t::id_t::IOP_INT_NEQ;
		else if( n->type == N_EQUALS )
			id = iop_t::id_t::IOP_INT_EQ;
		else
			lerr << error_line() << "Arithmetic operation " << n->type << " not yet implemented for integers" << std::endl;
	} else
		lerr << error_line() << "Arithmetic on type " << n->data_type << " not yet implemented" << std::endl;
	addOperation( id, r, s[1] );
	return r;	
}

void intermediateCode::function::translateBranching( const syntaxTree::node* n ) {
	assert( n->type == N_IF or n->type == N_WHILE or n->type == N_FOR );
	if( n->type == N_IF ) {
		variable_t a = translateArithmetic( n->children[0] );
		label_t endif = parent->newLabel();
		addOperation( iop_t::id_t::IOP_JN, ERROR_VARIABLE, a, ERROR_VARIABLE, endif );
		if( n->children[1]->type == N_ELSE ) {
			translateNode( n->children[1]->children[0] );
			label_t endelse = parent->newLabel();
			addOperation( iop_t::id_t::IOP_JUMP, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, endelse );
			addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, endif );
			translateNode( n->children[1]->children[1] );
			addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, endelse );
		} else {
			translateNode( n->children[1] );
			addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, endif );
		}
	} else if( n->type == N_WHILE ) {
		label_t condition_check = parent->newLabel(), natural_exit = parent->newLabel();
		label_t abrupt_exit = n->children[1]->type == N_ELSE ? parent->newLabel() : natural_exit;
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, condition_check );
		variable_t exp = translateExpression( n->children[0] );
		addOperation( iop_t::id_t::IOP_JN, ERROR_VARIABLE, exp, ERROR_VARIABLE, natural_exit );
		if( n->children[1]->type == N_ELSE )
			translateNode( n->children[1]->children[0] );
		else 
			translateNode( n->children[1] );
		addOperation( iop_t::id_t::IOP_JUMP, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, condition_check );
		if( n->children[1]->type == N_ELSE ) {
			addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, natural_exit );
			translateNode( n->children[1]->children[1] );
		}
		addOperation( iop_t::id_t::IOP_LABEL, ERROR_VARIABLE, ERROR_VARIABLE, ERROR_VARIABLE, abrupt_exit );
	} else 
		lerr << error_line() << "Branching operation " << n->type << " not yet implemented" << std::endl;
}

void intermediateCode::function::translateBlock( const syntaxTree::node* n ) {
	assert( n->type == N_SEQUENTIAL_BLOCK or n->type == N_PARALLEL_BLOCK );
	if( n->type == N_SEQUENTIAL_BLOCK ) {
		const syntaxTree::node* h = n;
		do {
			translateNode( h->children[0] );
			h = h->children[1];
		} while( h != nullptr );
	} else
		lerr << error_line() << "Parallel blocks not yet implemented" << std::endl;
}

variable_t intermediateCode::function::translateFunctionCall( const syntaxTree::node* n ) {
	assert( n->type == N_FUNCTION_CALL );
	function_t f = n->data.integer;
	type_t t = parent->scptab->getFunctionReturnType( f );
	variable_t r = ( t != VOID_TYPE ) ? parent->newTemporaryVariable( t ) : ERROR_VARIABLE;
	if( n->children[0] )
		translateArguments( n->children[0] );
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
	lerr << error_line() << "Lists not yet implemented" << std::endl;
}

void intermediateCode::declareFunction( function_t f ) {
	assert( f == functions.size() );
	functions.push_back( function{ scptab->getFunctionSymbol( f ), f, newLabel(), this } );
}

void intermediateCode::defineFunction( function_t f, const syntaxTree::node* n ) {
	functions.at( f ).translateNode( n );
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
	assert( newLabel() == ERROR_LABEL );
	for( function_t f = 0; f < scptab->getFunctionCount(); ++f )
		declareFunction( f );
}

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
		if( o.b != 0 ) {
			x = symtab->getName( scptab->getVariableSymbol( o.b ) );
			x.resize( 5, ' ' );
		} else
			x = std::to_string( o.c_b.integer );
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
		os << f << std::endl;
	return os;
}