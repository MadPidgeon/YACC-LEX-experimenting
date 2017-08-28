#pragma once
#include <vector>
#include <deque>
#include <iostream>
#include "types.h"
#include "symbol_table.h"
#include "error_reporting.h"

#define ERROR_SCOPE			0
#define GLOBAL_SCOPE		1

#define ERROR_VARIABLE		0

#define ERROR_FUNCTION		0
#define PRINT_FUNCTION		1
#define SCAN_FUNCTION		2
#define ITOA_FUNCTION		3
#define JOIN_STR_FUNCTION	4
#define FTOA_FUNCTION		5
#define GLOBAL_FUNCTION		6	// should always be the last predefined function

typedef uint64_t scope_t;
typedef uint64_t variable_t;
typedef uint64_t function_t;

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
		std::map<std::pair<symbol_t,std::vector<type_t>>,variable_t> function_declarations;
		typedefTable typedefs;
	};
	struct function_declaration {
		function_t declaration_id;
		type_t return_type;
		std::vector<variable_t> args;
		symbol_t symbol;
		scope_t scope;
	};
	std::deque<scope> scopes;
	std::deque<declaration> declarations;
	std::deque<function_declaration> function_declarations;
	symbolTable* symtab;
	structureTable* strtab;
public:
	bool addTypeDefinition( scope_t, symbol_t, structure_t, std::vector<type_t> par = {}, std::vector<type_t> sub = {} );
	type_t getTypeDefinition( scope_t, symbol_t, std::vector<type_t> = {} ) const;
	scope_t addScope( scope_t super );
	variable_t addVariable( scope_t, symbol_t, type_t );
	function_t addFunctionDeclaration( scope_t, symbol_t, type_t, std::vector<variable_t> = {} );
	type_t getFunctionReturnType( function_t ) const;
	const std::vector<variable_t>& getFunctionArguments( function_t ) const;
	std::vector<variable_t> getAllVariables( scope_t ) const;
	function_t getFunctionCount() const;
	function_t getFunction( scope_t, symbol_t, type_t ) const;
	symbol_t getFunctionSymbol( function_t ) const;
	scope_t getFunctionScope( function_t ) const;
	scope_t getVariableScope( variable_t ) const;
	type_t getVariableType( variable_t ) const;
	symbol_t getVariableSymbol( variable_t ) const;
	variable_t getVariable( scope_t, symbol_t ) const;
	scopeTable( symbolTable*, structureTable* );
};

std::ostream& operator<<( std::ostream& os, const scopeTable& );

extern scopeTable* scptab; // for I/O purposes only