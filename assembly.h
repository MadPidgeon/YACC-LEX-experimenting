#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include "intermediate_code.h"
#include "register_allocation.h"

class assemblyGenerator {
	std::stringstream definitions;
	std::stringstream instructions;
	registerAllocation ra;
	std::vector<variable_t> register_variable;
	std::unordered_map<variable_t,register_t> variable_register;
	size_t extra_definition_offset;
public:
	void generateInstruction( iop_t, std::string prefix );
	void generateFunction( const intermediateCode::function&, std::string prefix );
	void generateProgram( const intermediateCode& );
	void print( std::ostream& os ) const;
	void storeRegister( register_t );
	void loadVariable( variable_t, register_t );
	std::string addStringDefinition( std::string );
	register_t getRegister( variable_t x, bool define = false );
	assemblyGenerator( const intermediateCode& );
	assemblyGenerator();
};

std::ostream& operator<<( std::ostream& os, const assemblyGenerator& ag );