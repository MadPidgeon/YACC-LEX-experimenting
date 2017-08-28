#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "intermediate_code.h"
#include "register_allocation.h"
#include "stack_frame.h"

#define ERROR_ASM_REG	(instruction::asm_reg{-1,0})

#define A_REG		0
#define B_REG		1
#define C_REG		2
#define D_REG		3
#define BP_REG		4
#define SI_REG		5
#define DI_REG		6
#define SP_REG		7
#define R8_REG		8
#define R9_REG		9
#define R10_REG		10

#define RAX			instruction::asm_reg( A_REG, 8 )
#define RBX			instruction::asm_reg( B_REG, 8 )
#define RCX			instruction::asm_reg( C_REG, 8 )
#define RDX			instruction::asm_reg( D_REG, 8 )
#define RBP			instruction::asm_reg( BP_REG, 8 )
#define RSI			instruction::asm_reg( SI_REG, 8 )
#define RDI			instruction::asm_reg( DI_REG, 8 )
#define RSP			instruction::asm_reg( SP_REG, 8 )
#define R8			instruction::asm_reg( R8_REG, 8 )
#define R9			instruction::asm_reg( R9_REG, 8 )
#define R10			instruction::asm_reg( R10_REG, 8 )

struct instruction {
	struct asm_reg;
	struct complex_address;
	struct parameter;
	enum id_t {
		NOP,
		MOV, MOVSD, LEA,
		PUSH, POP,
		LABEL,
		CALL, RET,
		CMP, TEST,
		ADD, SUB, IMUL, IDIV,
		VADDSD, VSUBSD, VMULSD, VDIVSD,
		AND, OR, XOR, ANDN, 
		SETE, SETNE, SETL, SETG, SETLE, SETGE,
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
	} arg[3];
	instruction( id_t, parameter a = parameter( 0 ), parameter b = parameter( 0 ), parameter c = parameter( 0 ) );
	static const std::vector<int> parameter_count;
	static const std::vector<std::string> names;
};

std::ostream& operator<<( std::ostream& os, instruction i );

class assemblyGenerator {
	std::stringstream definitions;
	std::deque<instruction> instructions;
	register_t volatile_register_cycle;
	register_t volatile_floating_register_cycle;
	std::vector<variable_t> register_variables;
	registerAllocation ra;
	registerAllocation ra_flt;
	size_t extra_definition_offset;
	variable_t return_variable;
	label_t end_of_function;
	stackFrame sf;
public:
	void generateInstruction( iop_t, std::string prefix, size_t instruction_index );
	void generateFunction( const intermediateCode::function&, std::string prefix, bool main );
	void generateProgram( const intermediateCode& );
	void print( std::ostream& os, bool h = true ) const;
	void storeRegister( register_t, size_t instruction_index );
	void loadVariable( variable_t, register_t );
	void evacuateRegisters( size_t instruction_index );
	void restoreRegisters( size_t instruction_index );
	void resetRegisters();
	label_t addStringDefinition( std::string );
	label_t addFloatingDefinition( double );
	label_t addFunctionPointerDefinition( label_t l );
	register_t getRegister( variable_t x, size_t instruction_index, bool define = false );
	assemblyGenerator( const intermediateCode& );
	assemblyGenerator();
};

std::ostream& operator<<( std::ostream& os, const assemblyGenerator& ag );