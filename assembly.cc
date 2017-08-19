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
const std::vector<register_t> usable_registers = { 8, 9, 10, 11, 12, 13, 14, 15 };



const std::vector<std::string> direct_instruction_translation = {
	"nop",
	"!INT_MOV",
	"!STR_MOV",
	"!LABEL",
	"jmp", 
	"je", "jne", "jl", "jg", "jle", "jge",
	"!FJ",
	"push", "!RESERVE_RETURN",
	"!call", 
	"!RET",
	"add", "sub", "imul", 
	"idiv",
	"fadd", "fsub", "fmul", "fdiv",
	"!STR_CONEQ",
	"!INT_EQ", "!INT_NEQ",
	"!INT_ARR_LOAD", "!INT_ARR_STORE",
	"!LIST_ALLOCATE"
};

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
	"sete", "setne",
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
	1, 1,
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

void assemblyGenerator::generateInstruction( iop_t op, std::string prefix ) {
	// compute common parameters
	instruction::parameter par[4];
	if( op.usesResultParameter() )
		par[0] = instruction::parameter( instruction::asm_reg{ getRegister( op.r ), 8 } );
	if( op.usesReadParameterA() ) {
		if( op.a == ERROR_VARIABLE )
			par[1] = instruction::parameter( op.c_a.integer );
		else
			par[1] = instruction::parameter( instruction::asm_reg{ getRegister( op.a ), 8 } );
	}
	if( op.usesReadParameterB() ) {
		if( op.b == ERROR_VARIABLE )
			par[2] = instruction::parameter( op.c_b.integer );
		else
			par[2] = instruction::parameter( instruction::asm_reg{ getRegister( op.b ), 8 } );
	}
	par[3] = instruction::parameter( op.label );

	static const instruction::parameter rsp( instruction::asm_reg( RSP_REG, 8 ) );
	static const instruction::parameter rsi( instruction::asm_reg( RSI_REG, 8 ) );
	static const instruction::parameter rax( instruction::asm_reg( RAX_REG, 8 ) );
	static const instruction::parameter rdx( instruction::asm_reg( RDX_REG, 8 ) );
	static const instruction::parameter r8( instruction::asm_reg( R8_REG, 8 ) );
	static const instruction::parameter r9( instruction::asm_reg( R9_REG, 8 ) );
	static const instruction::parameter r10( instruction::asm_reg( R10_REG, 8 ) );
	static const instruction::parameter edi( instruction::asm_reg( RDI_REG, 4 ) );
	static const instruction::parameter r9d( instruction::asm_reg( R9_REG, 4 ) );


	switch( op.id ) {
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
		case iop_t::id_t::IOP_JE: case iop_t::id_t::IOP_JN: case iop_t::id_t::IOP_JL: case iop_t::id_t::IOP_JG: case iop_t::id_t::IOP_JLE: case iop_t::id_t::IOP_JGE:
			instructions.emplace_back( instruction::id_t::CMP, par[1], par[2] );
			instructions.emplace_back( instruction::id_t( op.id - iop_t::id_t::IOP_JE + instruction::id_t::JE ), par[3] );
			break;
		case iop_t::id_t::IOP_ADD_PARAM:
			instructions.emplace_back( instruction::id_t::PUSH, par[1] );
			break;
		case iop_t::id_t::IOP_RESERVE_RETURN:
			instructions.emplace_back( instruction::id_t::SUB, rsp, instruction::parameter( 8 ) );
			break;
		case iop_t::id_t::IOP_FUNCTION:
			instructions.emplace_back( instruction::id_t::CALL, par[3] );
			if( op.r != 0 )
				instructions.emplace_back( instruction::id_t::POP, par[0] );
			break;
		case iop_t::id_t::IOP_RETURN:
			instructions.emplace_back( instruction::id_t::RET, 0 );
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
		case iop_t::id_t::IOP_INT_EQ: case iop_t::id_t::IOP_INT_NEQ:
			instructions.emplace_back( instruction::id_t::CMP, par[1], par[2] );
			instructions.emplace_back( instruction::id_t( op.id - iop_t::id_t::IOP_INT_EQ + instruction::id_t::SETE ), par[0] );
			break;
		case iop_t::id_t::IOP_INT_ARR_LOAD:
			instructions.emplace_back( instruction::id_t::MOV, par[0], instruction::complex_address( par[1], par[2] ) );
			break;
		case iop_t::id_t::IOP_INT_ARR_STORE:
			instructions.emplace_back( instruction::id_t::MOV, instruction::complex_address( par[0], par[1] ), par[2] );
			break;
		case iop_t::id_t::IOP_LIST_ALLOCATE:
			instructions.emplace_back( instruction::id_t::PUSH, r8 );
			instructions.emplace_back( instruction::id_t::PUSH, r9 );
			instructions.emplace_back( instruction::id_t::PUSH, r10 );
			instructions.emplace_back( instruction::id_t::MOV, rax, instruction::parameter( 9 ) /*SYSCALL_MMAP*/ );
			if( op.a )
				instructions.emplace_back( instruction::id_t::LEA, rsi, par[1] );
			else
				instructions.emplace_back( instruction::id_t::MOV, rsi, instruction::parameter( 8 +  op.c_a.integer ) );
			instructions.emplace_back( instruction::id_t::XOR, edi, edi );
			instructions.emplace_back( instruction::id_t::MOV, rdx, instruction::parameter( 3 ) /* PROT_READ | PROT_WRITE */ );
			instructions.emplace_back( instruction::id_t::MOV, r10, instruction::parameter( 0x22 ) /* MAP_PRIVATE | MAP_ANON */ );
			instructions.emplace_back( instruction::id_t::MOV, r8, instruction::parameter( -1 ) );
			instructions.emplace_back( instruction::id_t::XOR, r9d, r9d );
			instructions.emplace_back( instruction::id_t::SYSCALL );
			instructions.emplace_back( instruction::id_t::POP, r10 );
			instructions.emplace_back( instruction::id_t::POP, r9 );
			instructions.emplace_back( instruction::id_t::POP, r8 );
			instructions.emplace_back( instruction::id_t::MOV, instruction::complex_address( rax, ERROR_ASM_REG ), par[1] );
			instructions.emplace_back( instruction::id_t::LEA, par[0], instruction::complex_address( rax.reg, ERROR_ASM_REG, 8, 8, ERROR_LABEL ) );
			/*instructions << "\tpush r8\n\tpush r9\n\tpush r10\n";
			instructions << "\tmov rax, SYSCALL_MMAP\n\txor edi, edi\n";
			if( op.a )
				instructions << "\tlea rsi, [" << arguments[1] << "+8]\n";
			else
				instructions << "\tmov rsi, " << (8+op.c_a.integer) << "\n";
			instructions << "\tmov rdx, PROT_READ | PROT_WRITE\n\tmov r10, MAP_PRIVATE | MAP_ANON\n\tmov r8, -1\n\txor r9d, r9d\n\tsyscall\n";
			instructions << "\tpop r10\n\tpop r9\n\tpop r8\n";
			instructions << "\tmov qword [rax], " << arguments[1] << "\n";
			instructions << "\tlea " << arguments[0] << ", [rax+8]\n";*/
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
		generateFunction( f, prefix );
		if( index == GLOBAL_FUNCTION ) {
			instructions.emplace_back( instruction::id_t::MOV, instruction::asm_reg( RAX_REG, 8 ), 60 /*SYSCALL EXIT*/ );
			instructions.emplace_back( instruction::id_t::MOV, instruction::asm_reg( RDI_REG, 8 ), 0 /* error code */ );
			instructions.emplace_back( instruction::id_t::SYSCALL );
		}
		else
			instructions.emplace_back( instruction::id_t::RET, 0 /*FIX: argument size*/ );
		++index;
	}
}

void assemblyGenerator::generateFunction( const intermediateCode::function& f, std::string prefix ) {
	std::cout << "Generating function " << prefix << std::endl;
	instructions.emplace_back( instruction::id_t::LABEL, f.label );
	// instructions << "_" << f.label << ": ; " << symtab->getName( f.symbol ) << "\n";
	flowGraph graph( f );
	// graph.computeLiveness();
	ra.reset( usable_register_count, graph.naiveLiveIntervals() );
	ra.linearScan();
	auto live_intervals = ra.getRegisterAssignment();
	std::sort( live_intervals.begin(), live_intervals.end(), startCompare );
	for( auto x : live_intervals )
		variable_register[ x.variable ] = x.assigned_register+8;
	for( const iop_t& iop : f.operations )
		generateInstruction( iop, prefix );
}

/*void assemblyGenerator::generateInstruction( iop_t op, std::string prefix ) {
	std::string arguments[4];
	if( op.usesResultParameter() and op.r )
		arguments[0] = register_name_64.at( getRegister( op.r, op.id == iop_t::id_t::IOP_INT_MOV ) );
	if( op.usesReadParameterA() )
		arguments[1] = op.a ? register_name_64.at( getRegister( op.a ) ) : ( op.isFloating() ? std::to_string( op.c_a.floating ) : std::to_string( op.c_a.integer ) );
	if( op.usesReadParameterB() )
		arguments[2] = op.b ? register_name_64.at( getRegister( op.b, op.id == iop_t::id_t::IOP_INT_MODDIV ) ) : ( op.isFloating() ? std::to_string( op.c_b.floating ) : std::to_string( op.c_b.integer ) );
	arguments[3] =  "_" + std::to_string( op.label );
	if( op.id >= iop_t::id_t::IOP_JE and op.id <= iop_t::id_t::IOP_JGE ) {
		instructions << "\tcmp " << arguments[1] << ", 0\n\t" << direct_instruction_translation.at(op.id) << " " << arguments[3] << std::endl;
	} else if( direct_instruction_translation.at(op.id)[0] != '!' ) {
		instructions << "\t" << direct_instruction_translation.at(op.id) << " ";
		bool comma = false;
		for( int i = 0; i < 4; ++i ) {
			if( iop_fields.at( op.id ) & (1<<i) ) {
				if( comma )
					instructions << ", ";
				comma = true;
				instructions << arguments[i];
			}
		}
		instructions << std::endl;
	} else switch( op.id ) {
		case iop_t::id_t::IOP_INT_MOV: // I anticipate this operation to get more complex, so it is listed as exception
			instructions << "\tmov " << arguments[0] << ", " << arguments[1] << std::endl;
			break;
		case iop_t::id_t::IOP_STR_MOV:
			if( op.a == 0 )
				arguments[1] = addStringDefinition( std::string( op.c_a.string ) );
			instructions << "\tlea " << arguments[0] << ", [" << arguments[1] << "]\n";
			break;
		case iop_t::id_t::IOP_FUNCTION:
			instructions << "\tcall " << arguments[3] << "\n";
			if( op.r != 0 )
				instructions << "\tpop " << arguments[0] << "\n";
			break;
		case iop_t::id_t::IOP_LABEL:
			instructions << arguments[3] << ":" << std::endl;
			break;
		case iop_t::id_t::IOP_INT_EQ:
			instructions << "\tcmp " << arguments[0] << ", " << arguments[1] << "\n\tsete " << register_name_8.at( getRegister( op.r ) ) << std::endl;
			break;
		case iop_t::id_t::IOP_INT_NEQ:
			instructions << "\tcmp " << arguments[0] << ", " << arguments[1] << "\n\tsetne " << register_name_8.at( getRegister( op.r ) ) << std::endl;
			break;
		case iop_t::id_t::IOP_RESERVE_RETURN:
			instructions << "\tsub rsp, " << arguments[1] << std::endl;
			break;
		case iop_t::id_t::IOP_LIST_ALLOCATE:
			instructions << "\tpush r8\n\tpush r9\n\tpush r10\n";
			instructions << "\tmov rax, SYSCALL_MMAP\n\txor edi, edi\n";
			if( op.a )
				instructions << "\tlea rsi, [" << arguments[1] << "+8]\n";
			else
				instructions << "\tmov rsi, " << (8+op.c_a.integer) << "\n";
			instructions << "\tmov rdx, PROT_READ | PROT_WRITE\n\tmov r10, MAP_PRIVATE | MAP_ANON\n\tmov r8, -1\n\txor r9d, r9d\n\tsyscall\n";
			instructions << "\tpop r10\n\tpop r9\n\tpop r8\n";
			instructions << "\tmov qword [rax], " << arguments[1] << "\n";
			instructions << "\tlea " << arguments[0] << ", [rax+8]\n"; 
			break;
		case iop_t::id_t::IOP_INT_ARR_LOAD:
			instructions << "\tmov " << arguments[0] << ", [" << arguments[1] << "+8*" << arguments[2]  << "]" << std::endl;
			break;
		case iop_t::id_t::IOP_INT_ARR_STORE:
			instructions << "\tmov qword [" << arguments[0] << "+8*" << arguments[1] << "], " << arguments[2] << std::endl;
			break;
		default:
			lerr << error_line() << "Cannot translate instruction " << op.debugName() << " to assembly yet!" << std::endl;
	}
}*/

void assemblyGenerator::evacuateRegisters() {
	
}

register_t assemblyGenerator::getRegister( variable_t x, bool define ) {
	if( variable_register.count( x ) == 0 )
		return ERROR_REGISTER;
	register_t r = variable_register.at( x );
	if( register_variable.at( r ) == x )
		return r;
	if( register_variable.at( r ) != ERROR_VARIABLE )
		storeRegister( r );
	if( not define )
		loadVariable( x, r );
	register_variable.at( r ) = x;
	return r;
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

void assemblyGenerator::storeRegister( register_t r ) {
	/*if( variable_register.at( register_variable.at(r) ).end <= line ) { // is alive, do a store

	}*/
}

void assemblyGenerator::loadVariable( variable_t x, register_t r ) {

}

void assemblyGenerator::print( std::ostream& os ) const {
	os << definitions.str() << std::endl;
	std::ifstream header_file("header.asm");
	os << header_file.rdbuf() << std::endl;
	header_file.close();
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
	assert( direct_instruction_translation.size() == iop_t::id_t::COUNT );
	assert( instruction::id_t::COUNT == instruction::names.size() );
	assert( instruction::id_t::COUNT == instruction::parameter_count.size() );
	register_variable.resize( register_count, ERROR_VARIABLE );
}