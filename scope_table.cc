#include "scope_table.h"
#include <iomanip>
#include <algorithm>

bool scopeTable::addTypeDefinition( scope_t scope, symbol_t symbol, structure_t structure, std::vector<type_t> par, std::vector<type_t> sub ) {
	return scopes.at( scope ).typedefs.addType( symbol, structure, par, sub );
}

type_t scopeTable::getTypeDefinition( scope_t deep, symbol_t symbol, std::vector<type_t> pars ) const {
	scope_t scope = deep;
	while( scope != ERROR_SCOPE ) {
		type_t t = scopes.at( scope ).typedefs.getType( symbol, pars );
		if( t != ERROR_TYPE )
			return t;
		scope = scopes.at( scope ).super_scope;
	}
	return ERROR_TYPE;
}

scope_t scopeTable::addScope( scope_t super ) {
	scope_t id = scopes.size();
	scope s = { id, super, {}, {} };
	scopes.push_back( s );
	scopes.at( super ).sub_scopes.push_back( id );
	return id;
}

variable_t scopeTable::addVariable( scope_t scope, symbol_t symbol, type_t type ) {
	if( scopes.at( scope ).declarations.find( symbol ) != scopes.at( scope ).declarations.end() )
		return ERROR_VARIABLE;
	variable_t id = declarations.size();
	declaration d = { id, type, symbol, scope };
	declarations.push_back( d );
	if( symbol != NONE_SYMBOL )
		scopes.at( scope ).declarations[symbol] = id;
	return id;
}

scope_t scopeTable::getVariableScope( variable_t variable ) const {
	return declarations.at( variable ).scope;
}

type_t scopeTable::getVariableType( variable_t variable ) const {
	return declarations.at( variable ).type;
}

symbol_t scopeTable::getVariableSymbol( variable_t variable ) const {
	return declarations.at( variable ).symbol;
}

function_t scopeTable::addFunctionDeclaration( scope_t scope, symbol_t symbol, type_t type, std::vector<variable_t> args ) {
	std::vector<type_t> a;
	for( variable_t v : args )
		a.push_back( getVariableType( v ) );
	auto i = std::make_pair(symbol,a);
	std::cout << "addFunctionDeclaration " << symbol;
	for( type_t t : a )
		std::cout << ", " << t;
	std::cout << std::endl;
	if( scopes.at( scope ).function_declarations.find( i ) != scopes.at( scope ).function_declarations.end() )
		return ERROR_FUNCTION;
	function_t id = function_declarations.size();
	function_declaration d = { id, type, args, symbol, scope };
	function_declarations.push_back( d );
	if( symbol != NONE_SYMBOL )
		scopes.at( scope ).function_declarations[i] = id;
	return id;
}

std::vector<variable_t> scopeTable::getAllVariables( scope_t scope ) const {
	std::vector<variable_t> r;
	for( const auto& p : scopes.at( scope ).declarations )
		r.push_back( p.second );
	std::sort( r.begin(), r.end() );
	return r;
}


type_t scopeTable::getFunctionReturnType( function_t function ) const {
	return function_declarations.at( function ).return_type;
}

symbol_t scopeTable::getFunctionSymbol( function_t function ) const {
	return function_declarations.at( function ).symbol;
}

scope_t scopeTable::getFunctionScope( function_t function ) const {
	return function_declarations.at( function ).scope;
}

const std::vector<variable_t>& scopeTable::getFunctionArguments( function_t function ) const {
	return function_declarations.at( function ).args;
}

function_t scopeTable::getFunctionCount() const {
	return function_declarations.size();
}

function_t scopeTable::getFunction( scope_t deep, symbol_t symbol, type_t type ) const {
	scope_t scope = deep;
	auto arg_types = type.unpackProduct();
	auto to_find = std::make_pair( symbol, arg_types );
	while( scope != ERROR_SCOPE ) {
		auto loc = scopes.at( scope ).function_declarations.find( to_find );
		if( loc != scopes.at( scope ).function_declarations.end() )
			return loc->second;
		scope = scopes.at( scope ).super_scope;
	}
	return ERROR_FUNCTION;
}

variable_t scopeTable::getVariable( scope_t deep, symbol_t symbol ) const {
	scope_t scope = deep;
	while( scope != ERROR_SCOPE ) {
		auto loc = scopes.at( scope ).declarations.find( symbol );
		if( loc != scopes.at( scope ).declarations.end() )
			return loc->second;
		scope = scopes.at( scope ).super_scope;
	}
	return ERROR_VARIABLE;
}

scopeTable::scopeTable( symbolTable* sym, structureTable* str ) {
	symtab = sym;
	strtab = str;
	assert( addScope( ERROR_SCOPE ) == ERROR_SCOPE );
	assert( addScope( ERROR_SCOPE ) == GLOBAL_SCOPE );
	assert( addVariable( ERROR_SCOPE, ERROR_SYMBOL, ERROR_TYPE ) == ERROR_VARIABLE );
	addTypeDefinition( GLOBAL_SCOPE, INT_SYMBOL, INT_STRUCTURE );
	addTypeDefinition( GLOBAL_SCOPE, FLT_SYMBOL, FLT_STRUCTURE );
	addTypeDefinition( GLOBAL_SCOPE, STR_SYMBOL, STR_STRUCTURE );
	addTypeDefinition( GLOBAL_SCOPE, LST_SYMBOL, LST_STRUCTURE, { type_t(int64_t(0)) }, { type_t(int64_t(0)) } );
	addTypeDefinition( GLOBAL_SCOPE, SET_SYMBOL, SET_STRUCTURE, { type_t(int64_t(0)) }, { type_t(int64_t(0)) } );
	addTypeDefinition( GLOBAL_SCOPE, FNC_SYMBOL, FNC_STRUCTURE, { type_t(int64_t(0)), type_t(int64_t(1)) }, { type_t(int64_t(0)), type_t(int64_t(1)) } );

	assert( addFunctionDeclaration( ERROR_SCOPE, ERROR_SYMBOL, ERROR_TYPE ) == ERROR_FUNCTION );
	assert( addFunctionDeclaration( GLOBAL_SCOPE, PRINT_SYMBOL, VOID_TYPE, { addVariable( ERROR_SCOPE, NONE_SYMBOL, STR_TYPE ) } ) == PRINT_FUNCTION );
	assert( addFunctionDeclaration( GLOBAL_SCOPE, SCAN_SYMBOL, STR_TYPE ) == SCAN_FUNCTION );
	assert( addFunctionDeclaration( GLOBAL_SCOPE, STR_SYMBOL, STR_TYPE, { addVariable( ERROR_SCOPE, NONE_SYMBOL, INT_TYPE ) } ) == ITOA_FUNCTION );
	assert( addFunctionDeclaration( GLOBAL_SCOPE, NONE_SYMBOL, STR_TYPE, { addVariable( ERROR_SCOPE, NONE_SYMBOL, STR_TYPE ), addVariable( ERROR_SCOPE, NONE_SYMBOL, STR_TYPE ) } ) == JOIN_STR_FUNCTION );
	assert( addFunctionDeclaration( GLOBAL_SCOPE, STR_SYMBOL, STR_TYPE, { addVariable( ERROR_SCOPE, NONE_SYMBOL, FLT_TYPE ) } ) == FTOA_FUNCTION );
	assert( addFunctionDeclaration( ERROR_SCOPE, NONE_SYMBOL, VOID_TYPE ) == GLOBAL_FUNCTION );
}

std::ostream& operator<<( std::ostream& os, const scopeTable& t ) {
	os << std::setfill(' ') << std::left << "SCOPE ID    NAME" << std::endl;
	for( size_t i = 0; i < t.scopes.size(); ++i ) {
		if( t.scopes.at(i).declarations.size() > 0 ) {
			os << std::setw( 6 ) << i;
			auto itr = t.scopes.at(i).declarations.begin();
			os << std::setw( 6 ) << itr->second << symtab->getName( itr->first ) << std::endl;
			for( ++itr; itr != t.scopes.at(i).declarations.end(); ++itr )
				os << "      " << std::setw( 6 ) << itr->second << symtab->getName( itr->first ) << std::endl;
		}
	}
	return os;
}
