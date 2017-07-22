#include "scope_table.h"
#include <iomanip>

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
	scopes.at( scope ).declarations[symbol] = id;
	return id;
}

scope_t scopeTable::getScope( variable_t variable ) const {
	return declarations.at( variable ).scope;
}

type_t scopeTable::getVariableType( variable_t variable ) const {
	return declarations.at( variable ).type;
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
	assert( addScope( ERROR_SCOPE ) == ERROR_SCOPE );
	assert( addScope( ERROR_SCOPE ) == GLOBAL_SCOPE );
	assert( addVariable( ERROR_SCOPE, ERROR_SYMBOL, ERROR_TYPE ) == ERROR_VARIABLE );
	addTypeDefinition( GLOBAL_SCOPE, INT_SYMBOL, INT_STRUCTURE );
	addTypeDefinition( GLOBAL_SCOPE, FLT_SYMBOL, FLT_STRUCTURE );
	addTypeDefinition( GLOBAL_SCOPE, STR_SYMBOL, STR_STRUCTURE );
	addTypeDefinition( GLOBAL_SCOPE, LST_SYMBOL, LST_STRUCTURE, { type_t(int64_t(0)) }, { type_t(int64_t(0)) } );
	addTypeDefinition( GLOBAL_SCOPE, SET_SYMBOL, SET_STRUCTURE, { type_t(int64_t(0)) }, { type_t(int64_t(0)) } );
	symtab = sym;
	strtab = str;
}
