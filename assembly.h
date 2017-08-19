#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include "intermediate_code.h"
#include "register_allocation.h"

#define ERROR_ASM_REG	(instruction::asm_reg{-1,0})

#define RAX_REG		0
#define RBX_REG		1
#define RCX_REG		2
#define RDX_REG		3
#define RSI_REG		5
#define RDI_REG		6
#define RSP_REG		7
#define R8_REG		8
#define R9_REG		9
#define R10_REG		10

struct instruction {
	struct asm_reg;
	struct complex_address;
	struct parameter;
	enum id_t {
		NOP,
		MOV, LEA,
		PUSH, POP,
		LABEL,
		CALL, RET,
		CMP, TEST,
		ADD, SUB, IMUL, IDIV,
		FADD, FSUB, FMUL, FDIV,
		AND, OR, XOR, ANDN, 
		SETE, SETNE,
		JMP, JE, JNE, JL, JG, JLE, JGE,
		SYSCALL,
		COUNT
	} id;
	struct asm_reg {
		register_t id;
		size_t size;
		operator bool() const;
		static const std::vector<std::string> name_64;
		static const std::vector<std::string> name_32;
		static const std::vector<std::string> name_16;
		static const std::vector<std::string> name_8;
		asm_reg();
		asm_reg( register_t i, size_t s );
	};
	struct complex_address {
		asm_reg base;
		asm_reg offset;
		int64_t constant;
		int64_t scale;
		label_t raw_address;
		operator bool() const;
		void print( std::ostream& os, bool size = true ) const;
		complex_address();
		complex_address( asm_reg b, asm_reg off );
		complex_address( parameter b, parameter off = parameter(0) );
		complex_address( asm_reg b, asm_reg off, int64_t c, int64_t s, label_t r );
	};
	struct parameter {
		asm_reg reg;
		complex_address mem;
		union {
			double floating;
			int64_t integer;
		} con;
		void print( std::ostream& os, bool size = true ) const;
		parameter( asm_reg ar );
		parameter( complex_address ca );
		parameter( int64_t i );
		parameter() = default;
	} arg[2];
	instruction( id_t, parameter a = parameter( 0 ), parameter b = parameter( 0 ) );
	static const std::vector<int> parameter_count;
	static const std::vector<std::string> names;
};

std::ostream& operator<<( std::ostream& os, instruction i );

class assemblyGenerator {
	std::stringstream definitions;
	std::deque<instruction> instructions;
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
	void evacuateRegisters();
	label_t addStringDefinition( std::string );
	register_t getRegister( variable_t x, bool define = false );
	assemblyGenerator( const intermediateCode& );
	assemblyGenerator();
};

std::ostream& operator<<( std::ostream& os, const assemblyGenerator& ag );