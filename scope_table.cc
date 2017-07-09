#include "scope_table.h"

scope_t scopeTable::addScope( scope_t super ) {
	scope_t id = scopes.size();
	scope s = { id, super, {}, {} };
	scopes.push_back( s );
	scopes.at( super ).sub_scopes.push_back( id );
	return id;
}

variable_t scopeTable::addVariable( scope_t scope, type_t type, symbol_t symbol ) {
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

type_t scopeTable::getType( variable_t variable ) const {
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

scopeTable::scopeTable() {
	addScope( ERROR_SCOPE );
	addScope( ERROR_SCOPE );
	addVariable( ERROR_SCOPE, ERROR_TYPE, ERROR_SYMBOL );
}
