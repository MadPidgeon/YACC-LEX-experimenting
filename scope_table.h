#pragma once
#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include "types.h"
#include "symbol_table.h"
#include "error_reporting.h"

#define ERROR_SCOPE		0
#define GLOBAL_SCOPE	1

#define ERROR_VARIABLE	0

typedef uint64_t scope_t;
typedef uint64_t variable_t;

class scopeTable {
	friend std::ostream& operator<<( std::ostream&, const scopeTable& );
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
		typedefTable typedefs;
	};
	std::deque<scope> scopes;
	std::deque<declaration> declarations;
	symbolTable* symtab;
	structureTable* strtab;
public:
	bool addTypeDefinition( scope_t, symbol_t, structure_t, std::vector<type_t> par = {}, std::vector<type_t> sub = {} );
	type_t getTypeDefinition( scope_t, symbol_t, std::vector<type_t> = {} ) const;
	scope_t addScope( scope_t super );
	variable_t addVariable( scope_t, symbol_t, type_t );
	scope_t getScope( variable_t ) const;
	type_t getVariableType( variable_t ) const;
	variable_t getVariable( scope_t, symbol_t ) const;
	scopeTable( symbolTable*, structureTable* );
};

std::ostream& operator<<( std::ostream& os, const scopeTable& );