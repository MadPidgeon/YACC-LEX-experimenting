#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include "scope_table.h"

typedef int64_t stack_offset_t;

// STACK FRAME:
// -------------------------
// Return value
// -------------------------
// Argument 1
// ...
// Argument n-1
// Argument n
// -------------------------
// Return address
// Old base pointer         < rbp
// -------------------------
// Local variable 1
// Local variable 2
// Local variable 3
// ...
// Local variable m         < rsp
// -------------------------
// Argument stack
// ...
// ...

class stackFrame {
public:
	struct variable_storage {
		variable_t variable;
		size_t size; // in bytes
		stack_offset_t offset;
		variable_storage( variable_t v = ERROR_VARIABLE, size_t s = 0, stack_offset_t o = 0 );
	};
private:
	std::vector<variable_storage> return_values;
	std::vector<variable_storage> function_arguments;
	variable_storage return_address;
	variable_storage old_bp;
	std::vector<variable_storage> saved_variables;
	std::vector<variable_storage> argument_stack;
	size_t byte_count;
	std::map<variable_t,std::pair<int,size_t>> variable_location;
public:
	size_t localVariableSize() const;
	size_t popCountOnReturn() const;
	stack_offset_t addVariable( variable_t v, size_t s );
	stack_offset_t getVariableLocation( variable_t v ) const;
	stack_offset_t getVariableDirectedLocation( variable_t v ) const;
	bool isLocalVariable( variable_t v ) const;
	void print( std::ostream& os ) const;
	void reset( std::vector<variable_storage> returns, std::vector<variable_storage> arguments );
	stackFrame();
	stackFrame( std::vector<variable_storage> returns, std::vector<variable_storage> arguments );
};

std::ostream& operator<<( std::ostream&, const stackFrame& );