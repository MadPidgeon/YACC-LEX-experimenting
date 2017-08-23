#include <string>
#include <algorithm>
#include <fstream>
#include "assembly.h"
#include "intermediate_code.h"
#include "flow_graph.h"
#include "register_allocation.h"

const register_t usable_register_count = 8;
const register_t bonus_register_count = 8;
const register_t register_count = usable_register_count + bonus_register_count;
const std::vector<register_t> volatile_registers = { A_REG, DI_REG, SI_REG, D_REG };
const std::vector<register_t> usable_registers = { 8, 9, 10, 11, 12, 13, 14, 15 };

// ************************************************
// INSTRUCTION
// ************************************************

const std::vector<std::string> instruction::names = {
	"nop",
	"mov", "lea",
	"push", "pop",
	"label",
	"call", "ret",
	"cmp", "test",
	"add", "sub", "imul", "idiv",
	"fadd", "fsub", "fmul", "fdiv",
	"and", "or", "xor", "andn", 
	"sete", "setne", "setl", "setg", "setle", "setge",
	"jmp", "je", "jne", "jl", "jg", "jle", "jge",
	"syscall"
};

const std::vector<int> instruction::parameter_count = {
	0,
	2, 2,
	1, 1,
	1,
	1, 1,
	2, 2,
	2, 2, 2, 1,
	2, 2, 2, 2,
	2, 2, 2, 2,
	1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1,
	0
};

const std::vector<std::string> instruction::asm_reg::name_64 = { "rax", "rbx", "rcx", "rdx", "rbp", "rsi", "rdi", "rsp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" };
const std::vector<std::string> instruction::asm_reg::name_32 = { "eax", "ebx", "ecx", "edx", "ebp", "esi", "edi", "esp", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d" };
const std::vector<std::string> instruction::asm_reg::name_16 = { "ax", "bx", "cx", "dx", "bp", "si", "di", "sp", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w" };
const std::vector<std::string> instruction::asm_reg::name_8 = { "al", "bl", "cl", "dl", "bpl", "sil", "dil", "spl", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b" };

std::ostream& operator<<( std::ostream& os, instruction::asm_reg r ) {
	if( r.size == 1 )
		os << r.name_8.at( r.id );
	else if( r.size == 2 )
		os << r.name_16.at( r.id );
	else if( r.size == 4 )
		os << r.name_32.at( r.id );
	else
		os << r.name_64.at( r.id );
	return os;
}

void instruction::complex_address::print( std::ostream& os, bool size ) const {
	if( size ) switch( scale ) {
		case 1:
			os << "byte ";
			break;
		case 2:
			os << "word ";
			break;
		case 4:
			os << "dword ";
			break;
		case 8:
			os << "qword ";
			break;
	}
	os << "[" << constant;
	if( raw_address != ERROR_LABEL )
		os << "+_def_" << raw_address;
	if( base )
		os << "+" << base;
	if( offset )
		os << "+" << scale << "*" << offset;
	os << "]";
}


std::ostream& operator<<( std::ostream& os, instruction::complex_address ca ) {
	ca.print( os );
	return os;
}

void instruction::parameter::print( std::ostream& os, bool size ) const {
	if( reg )
		os << reg;
	else if( mem )
		mem.print( os, size );
	else
		os << con.integer;
}

std::ostream& operator<<( std::ostream& os, instruction::parameter p ) {
	p.print( os );
	return os;
}

std::ostream& operator<<( std::ostream& os, instruction inst ) {
	if( inst.id == instruction::id_t::LABEL )
		return os << "_" << inst.arg[0].con.integer << ":";
	os << "\t" << inst.names.at( inst.id );
	if( inst.id == instruction::id_t::CALL or ( inst.id >= instruction::id_t::JMP and inst.id <= instruction::id_t::JGE ) )
		return os << " _" << inst.arg[0].con.integer;
	if( inst.parameter_count.at( inst.id ) >= 1 ) 
		os << " " << inst.arg[0];
	if( inst.parameter_count.at( inst.id ) >= 2 ) {
		os << ", ";
		inst.arg[1].print( os, inst.id != instruction::id_t::LEA );
	}
	return os;
}

instruction::asm_reg::asm_reg( register_t i, size_t s ) : id(i), size(s) {
}

instruction::asm_reg::asm_reg() : asm_reg( -1, 0 ) {
}

instruction::complex_address::complex_address() {
	scale = 0;
}

instruction::complex_address::complex_address( asm_reg b, asm_reg o, int64_t c, int64_t s, label_t r ) : base( b ), offset( o ), constant( c ), scale( s ), raw_address( r ) {
}

instruction::complex_address::complex_address( instruction::asm_reg b, instruction::asm_reg o ) : complex_address( b, o, 0, 0, ERROR_LABEL ) {
}

instruction::complex_address::complex_address( parameter b, parameter off ) {
	constant = 0;
	scale = 8;
	if( b.reg ) {
		base = b.reg;
	} else {
		base = ERROR_ASM_REG;
		constant += b.con.integer;
	}
	if( off.reg ) {
		offset = off.reg;
	} else {
		offset = ERROR_ASM_REG;
		constant += scale * off.con.integer;
	}
	raw_address = 0;
}


instruction::parameter::parameter( instruction::asm_reg ar ) : reg( ar ), con{.integer=0}  {
}

instruction::parameter::parameter( instruction::complex_address ca ) : reg(), mem( ca ), con{.integer= 0} {
}

instruction::parameter::parameter( int64_t i ) {
	con.integer = i;
}

instruction::instruction( instruction::id_t i, instruction::parameter a, instruction::parameter b ) : id(i), arg{ a, b } {
}

instruction::asm_reg::operator bool() const {
	return id != -1;
}

instruction::complex_address::operator bool() const {
	return scale != 0;
}

// ************************************************
// ASSEMBLY GENERATOR
// ************************************************

void assemblyGenerator::generateInstruction( iop_t op, std::string prefix, size_t instruction_index ) {
	std::cout << instruction_index << ": " << op << std::endl;
	// compute common parameters
	instruction::parameter par[4];
	if( op.usesResultParameter() and op.id != iop_t::id_t::IOP_FUNCTION )
		par[0] = instruction::parameter( instruction::asm_reg{ getRegister( op.r, instruction_index, not op.parameterIsRead( 0 ) ), 8 } );
	if( op.usesReadParameterA() ) {
		if( op.a == ERROR_VARIABLE )
			par[1] = instruction::parameter( op.c_a.integer );
		else
			par[1] = instruction::parameter( instruction::asm_reg{ getRegister( op.a, instruction_index, not op.parameterIsRead( 1 ) ), 8 } );
	}
	if( op.usesReadParameterB() ) {
		if( op.b == ERROR_VARIABLE )
			par[2] = instruction::parameter( op.c_b.integer );
		else
			par[2] = instruction::parameter( instruction::asm_reg{ getRegister( op.b, instruction_index, not op.parameterIsRead( 2 ) ), 8 } );
	}
	par[3] = instruction::parameter( op.label );

	static const instruction::parameter rsp( RSP );
	static const instruction::parameter rsi( RSI );
	static const instruction::parameter rax( RAX );
	static const instruction::parameter rdx( RDX );
	static const instruction::parameter r8( R8 );
	static const instruction::parameter r9( R9 );
	static const instruction::parameter r10( R10 );
	static const instruction::parameter rdi( RDI );

	switch( op.id ) {
		case iop_t::id_t::IOP_GARBAGE:
			break;
		case iop_t::id_t::IOP_NOP:
			instructions.emplace_back( instruction::id_t::NOP );
			break;
		case iop_t::id_t::IOP_INT_MOV: 	
			instructions.emplace_back( instruction::id_t::MOV, par[0], par[1] );
			break;
		case iop_t::id_t::IOP_STR_MOV:
			if( op.a == 0 ) 
				instructions.emplace_back( instruction::id_t::LEA, par[0], instruction::complex_address( ERROR_ASM_REG, ERROR_ASM_REG, 0, 8, addStringDefinition( std::string( op.c_a.string ) ) ) );
			else
				instructions.emplace_back( instruction::id_t::LEA, par[0], instruction::complex_address( par[1] ) );
			break;
		case iop_t::id_t::IOP_LABEL:
			instructions.emplace_back( instruction::id_t::LABEL, par[3] );
			break;
		case iop_t::id_t::IOP_JUMP:
			instructions.emplace_back( instruction::id_t::JMP, par[3] );
			break;
		case iop_t::id_t::IOP_JT: case iop_t::id_t::IOP_JF:
			instructions.emplace_back( instruction::id_t::TEST, par[1], instruction::parameter( 1 ) );
			instructions.emplace_back( op.id == iop_t::id_t::IOP_JT ? instruction::id_t::JNE : instruction::id_t::JE, par[3] );
			break;
		case iop_t::id_t::IOP_JE: case iop_t::id_t::IOP_JN: case iop_t::id_t::IOP_JL: case iop_t::id_t::IOP_JG: case iop_t::id_t::IOP_JLE: case iop_t::id_t::IOP_JGE:
			instructions.emplace_back( instruction::id_t::CMP, par[1], par[2] ); // parameter 2 could be constant, error
			instructions.emplace_back( instruction::id_t( op.id - iop_t::id_t::IOP_JE + instruction::id_t::JE ), par[3] );
			break;
		case iop_t::id_t::IOP_ADD_PARAM:
			instructions.emplace_back( instruction::id_t::PUSH, par[1] );
			break;
		case iop_t::id_t::IOP_RESERVE_RETURN:
			instructions.emplace_back( instruction::id_t::SUB, rsp, instruction::parameter( 8 ) );
			break;
		case iop_t::id_t::IOP_FUNCTION:
			evacuateRegisters( instruction_index );
			instructions.emplace_back( instruction::id_t::CALL, par[3] );
			if( op.r != 0 ) {
				par[0] = instruction::parameter( instruction::asm_reg{ getRegister( op.r, instruction_index, not op.parameterIsRead( 0 ) ), 8 } );
				instructions.emplace_back( instruction::id_t::POP, par[0] );
			}
			// restoreRegisters( instruction_index );
			break;
		case iop_t::id_t::IOP_RETURN:
			instructions.emplace_back( instruction::id_t::RET, 0 );
			break;
		case iop_t::id_t::IOP_INT_ANDEQ: case iop_t::id_t::IOP_INT_OREQ: case iop_t::id_t::IOP_INT_XOREQ: case iop_t::id_t::IOP_INT_ANDNEQ:
			instructions.emplace_back( instruction::id_t( instruction::id_t::AND + op.id - iop_t::id_t::IOP_INT_ANDEQ ), par[0], par[1] );
			break;
		case iop_t::id_t::IOP_INT_ADDEQ: case iop_t::id_t::IOP_INT_SUBEQ: case iop_t::id_t::IOP_INT_MULEQ:
			instructions.emplace_back( instruction::id_t( op.id - iop_t::id_t::IOP_INT_ADDEQ + instruction::id_t::ADD ), par[0], par[1] );
			break;
		case iop_t::id_t::IOP_INT_MODDIV:
			instructions.emplace_back( instruction::id_t::MOV, rax, par[0] );
			instructions.emplace_back( instruction::id_t::MOV, rdx, par[1] );
			instructions.emplace_back( instruction::id_t::IDIV, rdx );
			if( op.b != ERROR_VARIABLE )
				instructions.emplace_back( instruction::id_t::MOV, par[2], rdx );
			break;
		case iop_t::id_t::IOP_STR_CONEQ: // does not exist
			lerr << error_line() << "IOP_STR_CONEQ depricated" << std::endl; 
			break;
		case iop_t::id_t::IOP_INT_EQ: case iop_t::id_t::IOP_INT_NEQ: case iop_t::id_t::IOP_INT_L: case iop_t::id_t::IOP_INT_G: case iop_t::id_t::IOP_INT_LE: case iop_t::id_t::IOP_INT_GE: 
			instructions.emplace_back( instruction::id_t::CMP, par[1], par[2] );
			instructions.emplace_back( instruction::id_t( op.id - iop_t::id_t::IOP_INT_EQ + instruction::id_t::SETE ), instruction::parameter( instruction::asm_reg{ getRegister( op.r, instruction_index, not op.parameterIsRead( 0 ) ), 1 } ) );
			// add and for fixes
			break;
		case iop_t::id_t::IOP_INT_ARR_LOAD:
			instructions.emplace_back( instruction::id_t::MOV, par[0], instruction::complex_address( par[1], par[2] ) );
			break;
		case iop_t::id_t::IOP_INT_ARR_STORE:
			instructions.emplace_back( instruction::id_t::MOV, instruction::complex_address( par[0], par[1] ), par[2] );
			break;
		case iop_t::id_t::IOP_LIST_ALLOCATE:
			if( op.a != ERROR_VARIABLE )
				instructions.emplace_back( instruction::id_t::LEA, rsi, par[1] );
			else
				instructions.emplace_back( instruction::id_t::MOV, rsi, instruction::parameter( 8 + op.c_a.integer ) );
			evacuateRegisters( instruction_index );
			instructions.emplace_back( instruction::id_t::MOV, rax, instruction::parameter( 9 ) /*SYSCALL_MMAP*/ );
			instructions.emplace_back( instruction::id_t::XOR, rdi, rdi );
			instructions.emplace_back( instruction::id_t::MOV, rdx, instruction::parameter( 3 ) /* PROT_READ | PROT_WRITE */ );
			instructions.emplace_back( instruction::id_t::MOV, r10, instruction::parameter( 0x22 ) /* MAP_PRIVATE | MAP_ANON */ );
			instructions.emplace_back( instruction::id_t::MOV, r8, instruction::parameter( -1 ) );
			instructions.emplace_back( instruction::id_t::XOR, r9, r9 );
			instructions.emplace_back( instruction::id_t::SYSCALL );
			if( op.a == ERROR_VARIABLE )
				par[1] = instruction::parameter( op.c_a.integer );
			else
				par[1] = instruction::parameter( instruction::asm_reg{ getRegister( op.a, instruction_index, not op.parameterIsRead( 1 ) ), 8 } );
			instructions.emplace_back( instruction::id_t::MOV, instruction::complex_address( rax, ERROR_ASM_REG ), par[1] );
			instructions.emplace_back( instruction::id_t::LEA, par[0], instruction::complex_address( rax.reg, ERROR_ASM_REG, 8, 8, ERROR_LABEL ) );
			break;
		default:
			lerr << error_line() << "Unknown IR instruction " << op.id << std::endl;
	}
}


void assemblyGenerator::generateProgram( const intermediateCode& ic ) {
	definitions << "section .data\n";
	size_t index = 0;
	for( const intermediateCode::function& f : ic.getFunctions() ) {
		if( f.id < GLOBAL_FUNCTION ) {
			index++;
			continue;
		}
		std::string prefix = "_" + std::to_string( index ) + "_";
		generateFunction( f, prefix, index == GLOBAL_FUNCTION );
		++index;
	}
}

void assemblyGenerator::generateFunction( const intermediateCode::function& f, std::string prefix, bool main ) {
	std::cout << "Generating function " << prefix << std::endl;
	// Optimize code
	flowGraph graph( f );
	graph.optimize();
	std::cout << "Intermediate Code:" << std::endl << graph << std::endl;
	// Compute register allocation
	graph.computeLiveness();
	ra.reset( usable_register_count, graph.naiveLiveIntervals() );
	ra.linearScan();
	{
		auto live_intervals = ra.getRegisterAssignment();
		std::sort( live_intervals.begin(), live_intervals.end(), startCompare );
		for( auto li : live_intervals )
			std::cout << li.variable << " [" << li.begin << "," << li.end << "]\n";
	}

	auto code = graph.generateFunction().operations;
	// Generate stack frame
	std::vector<stackFrame::variable_storage> returns;
	std::vector<stackFrame::variable_storage> arguments;
	for( variable_t v : f.getArguments() )
		arguments.emplace_back( v, scptab->getVariableType( v ).rawSize() );
	sf.reset( returns, arguments );
	for( const auto& op : code ) {
		for( int i = 0; i < 3; ++i ) {
			if( op.parameterIsVariable( i ) ) {
				variable_t v = op.getParameterVariable( i );
				sf.addVariable( v, scptab->getVariableType( v ).rawSize() );
			}
		}
	}
	std::cout << "\033[1;32m";
	std::cout << sf << std::endl;
	std::cout << std::endl;
	
	std::cout << "\033[0m\n"; 
	/*for( auto x : live_intervals )
		variable_register[ x.variable ] = x.assigned_register+8;*/
	// Generate preamble
	instructions.emplace_back( instruction::id_t::LABEL, f.label );
	instructions.emplace_back( instruction::id_t::PUSH, RBP );
	// 	instructions.emplace_back( instruction::id_t::MOV, instruction::complex_address( instruction::complex_address( instruction::asm_reg( RSP_REG, 8 ), instruction::asm_reg(), -8, 8, ERROR_LABEL ) ), instruction::asm_reg( RBP_REG, 8 ) );
	instructions.emplace_back( instruction::id_t::MOV, RBP, RSP );
	instructions.emplace_back( instruction::id_t::ADD, RSP, sf.localVariableSize() );
	// Generate code
	/*for( const iop_t& iop : code )
		generateInstruction( iop, prefix );*/
	size_t bi = 1;
	for( auto b : graph.generateCodeByBlock() ) {
		size_t ioff = graph.getBlock( bi++ ).instruction_offset;
		for( variable_t& v : register_variables )
			v = ERROR_VARIABLE;
		std::cout << "active at " << ioff << std::endl;
		for( variable_t v : ra.activeVariables( ioff ) ) {
			std::cout << v << " ";
			register_variables.at( usable_registers.at( ra.getVariableRegister( v ) ) ) = v;
		}
		std::cout << std::endl;
		for( auto& o : b )
			generateInstruction( o, prefix, ioff++ );
		restoreRegisters( ioff-1 );
	}
	// Generate postamble
	if( main ) {
		instructions.emplace_back( instruction::id_t::MOV, RAX, 60 /*SYSCALL EXIT*/ );
		instructions.emplace_back( instruction::id_t::MOV, RDI, 0 /* error code */ );
		instructions.emplace_back( instruction::id_t::SYSCALL );
	} else {
		instructions.emplace_back( instruction::id_t::LEA, RSP, instruction::complex_address( RBP, ERROR_ASM_REG, -8, 8, ERROR_LABEL ) );
		instructions.emplace_back( instruction::id_t::MOV, RBP, instruction::complex_address( RSP, ERROR_ASM_REG, 8, 8, ERROR_LABEL ) );
		instructions.emplace_back( instruction::id_t::RET, sf.popCountOnReturn() );
	}
}

void assemblyGenerator::evacuateRegisters( size_t instruction_index ) {
	for( register_t r : usable_registers )
		storeRegister( r, instruction_index );
	for( register_t r : volatile_registers )
		storeRegister( r, instruction_index );
}

void assemblyGenerator::restoreRegisters( size_t instruction_index ) {
	for( variable_t v : ra.activeVariables( instruction_index ) ) {
		register_t r = usable_registers.at( ra.getVariableRegister( v ) );
		if( register_variables.at( r ) != v )
			loadVariable( v, r );
	}
}

register_t assemblyGenerator::getRegister( variable_t x, size_t instruction_index, bool define ) {
	std::cout << "getRegister(" << x << "," << instruction_index << "," << define << ");\n";
	// check if variable in volatile memory
	for( register_t r : volatile_registers )
		if( register_variables.at( r ) == x )
			return r;
	// if variable is non-volatile, get its register
	register_t i = ra.getVariableRegister( x );	
	if( i != ERROR_REGISTER ) {
		std::cout << "?";
		register_t r = usable_registers.at( i );
		if( register_variables.at( r ) != x )
			if( not define )
				loadVariable( x, r );
		register_variables.at( r ) = x;
		return r;
	}
	throw; // temp
	// variable is volatile and not in register, so find a register for it
	register_t ri = ( volatile_register_cycle++ ) % volatile_registers.size();
	register_t r = volatile_registers.at( ri );
	storeRegister( r, instruction_index );
	register_variables.at( r ) = x;
	// if it is read from, load it from stack
	if( not define )
		loadVariable( x, r );
	return r;
}

void assemblyGenerator::storeRegister( register_t r, size_t instruction_index ) {
	variable_t v = register_variables.at( r );
	if( v == ERROR_VARIABLE )
		return;
	if( ra.isActive( v, instruction_index ) ) {
		auto offset = sf.getVariableLocation( v );
		instructions.emplace_back( instruction::id_t::MOV, instruction::complex_address( RBP, ERROR_ASM_REG, offset, 8, ERROR_LABEL ), instruction::asm_reg( r, 8 ) );
	}
	register_variables.at( r ) = ERROR_VARIABLE;
}

void assemblyGenerator::loadVariable( variable_t v, register_t r ) {
	if( v == ERROR_VARIABLE )
		return;
	auto offset = sf.getVariableLocation( v );
	instructions.emplace_back( instruction::id_t::MOV, instruction::asm_reg( r, 8 ), instruction::complex_address( RBP, ERROR_ASM_REG, offset, 8, ERROR_LABEL ) );
	register_variables.at( r ) = v;
}

label_t assemblyGenerator::addStringDefinition( std::string str ) {
	std::string ret = "_def_" + std::to_string( ++extra_definition_offset );
	definitions << "\t" << ret << "_data db " << std::hex;
	bool comma = false;
	for( int i = 0; i < 8; ++i ) {
		unsigned char c = ( ( str.size() >> (8*i) ) & 255 );
		if( comma ) 
			definitions << ",";
		comma = true;
		definitions << "0x" << ((c<16)?"0":"") << int(c);
	}
	for( unsigned char c : str )
		definitions << ",0x" << ((c<16)?"0":"") << int(c);
	definitions << "\n\t" << ret << " equ " << ret << "_data+8\n";
	return extra_definition_offset;
}

void assemblyGenerator::print( std::ostream& os, bool h ) const {
	if( h ) {
		os << definitions.str() << std::endl;
		std::ifstream header_file("header.asm");
		os << header_file.rdbuf() << std::endl;
		header_file.close();
	}
	for( const auto& i : instructions )
		os << i << "\n";
}

std::ostream& operator<<( std::ostream& os, const assemblyGenerator& ag ) {
	ag.print( os );
	return os;
}

assemblyGenerator::assemblyGenerator( const intermediateCode& ic ) : assemblyGenerator() {
	generateProgram( ic );
}

assemblyGenerator::assemblyGenerator() {
	extra_definition_offset = 0;
	assert( instruction::id_t::COUNT == instruction::names.size() );
	assert( instruction::id_t::COUNT == instruction::parameter_count.size() );
	register_variables.resize( register_count, ERROR_VARIABLE );
	volatile_register_cycle = 0;
}