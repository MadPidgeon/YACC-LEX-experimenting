#pragma once
#include <vector>
#include <map>
#include <deque>
#include "types.h"
#include "symbol_table.h"

#define ERROR_SCOPE		0
#define GLOBAL_SCOPE	1

#define ERROR_VARIABLE	0

typedef uint64_t scope_t;
typedef uint64_t variable_t;

class scopeTable {
	struct declaration {
		variable_t declaration_id;
		type_t type;
		symbol_t symbol;
		scope_t scope;
	};
	struct scope {
		scope_t scope_id;
		scope_t super_scope;
		std::vector<scope_t> sub_scopes;
		std::map<symbol_t,variable_t> declarations;
	};
	std::deque<scope> scopes;
	std::deque<declaration> declarations;
public:
	scope_t addScope( scope_t super );
	variable_t addVariable( scope_t, type_t, symbol_t );
	scope_t getScope( variable_t ) const;
	type_t getType( variable_t ) const;
	variable_t getVariable( symbol_t, scope_t ) const;
	scopeTable();
};