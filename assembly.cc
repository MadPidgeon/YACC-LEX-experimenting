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
const std::vector<std::string> register_name_64 = { "rax", "rbx", "rcx", "rdx", "rbp", "rsi", "rdi", "rsp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" };
const std::vector<std::string> register_name_32 = { "eax", "ebx", "ecx", "edx", "ebp", "esi", "edi", "esp", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d" };
const std::vector<std::string> register_name_16 = { "ax", "bx", "cx", "dx", "bp", "si", "di", "sp", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w" };
const std::vector<std::string> register_name_8 = { "al", "bl", "cl", "dl", "bpl", "sil", "dil", "spl", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b" };

const std::string header_str = "_3:\n\tmov	rax, [rsp]\n\tlea rsi, [rax+8]\n\tmov rdx, [rax]\n\tmov rax, 1\n\tmov rdi, 1\n\tsyscall\n\tret";

const std::vector<std::string> direct_instruction_translation = {
	"nop",
	"!INT_MOV",
	"!STR_MOV",
	"!LABEL",
	"jmp", 
	"je", "jne", "jl", "jg", "jle", "jge",
	"!FJ",
	"push",
	"!call", 
	"!RET",
	"add", "sub", "imul", 
	"idiv",
	"fadd", "fsub", "fmul", "fdiv",
	"!STR_CONEQ",
	"!INT_EQ", "!INT_NEQ",
	"!INT_ARR_LOAD", "!INT_ARR_STORE"
};

void assemblyGenerator::generateProgram( const intermediateCode& ic ) {
	definitions << "section .data\n";
	std::ifstream header_file("header.asm");
	instructions << header_file.rdbuf() << std::endl;
	header_file.close();
	//instructions << "default rel\nsection .text\n" << header_str << "\nglobal _start\n_start:\nglobal start\nstart:\n";
	size_t index = 0;
	for( const intermediateCode::function& f : ic.getFunctions() ) {
		if( f.id == ERROR_FUNCTION or f.id == PRINT_FUNCTION or f.id == SCAN_FUNCTION ) {
			index++;
			continue;
		}
		std::string prefix = "_" + std::to_string( index ) + "_";
		generateFunction( f, prefix );
		if( index == GLOBAL_FUNCTION )
			instructions << "\tmov rax, 60\n\tmov rdi, 0\n\tsyscall\n";
		else
			instructions << "\tret\n";
		++index;
	}
}

void assemblyGenerator::generateFunction( const intermediateCode::function& f, std::string prefix ) {
	std::cout << "Generating function " << prefix << std::endl;
	instructions << "_" << f.label << ": ; " << symtab->getName( f.symbol ) << "\n";
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

void assemblyGenerator::generateInstruction( iop_t op, std::string prefix ) {
	std::string arguments[4];
	if( op.usesResultParameter() and op.r )
		arguments[0] = register_name_64.at( getRegister( op.r, op.id == iop_t::id_t::IOP_INT_MOV ) );
	if( op.usesReadParameterA() )
		arguments[1] = op.a ? register_name_64.at( getRegister( op.a ) ) : ( op.isFloating() ? std::to_string( op.c_a.floating ) : std::to_string( op.c_a.integer ) );
	if( op.usesReadParameterB() )
		arguments[2] = op.a ? register_name_64.at( getRegister( op.b, op.id == iop_t::id_t::IOP_INT_MODDIV ) ) : ( op.isFloating() ? std::to_string( op.c_b.floating ) : std::to_string( op.c_b.integer ) );
	arguments[3] = /*prefix +*/ "_" + std::to_string( op.label );
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
			instructions << "\tmov " << arguments[0] << ", " << arguments[1] << std::endl;
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
		default:
			lerr << error_line() << "Cannot translate instruction " << op.debugName() << " to assembly yet!" << std::endl;
	}
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

std::string assemblyGenerator::addStringDefinition( std::string str ) {
	std::string ret = "str_" + std::to_string( extra_definition_offset++ );
	definitions << "\t" << ret << " db " << std::hex;
	for( int i = 0; i < 8; ++i ) {
		unsigned char c = ( ( str.size() >> (8*i) ) & 255 );
		definitions << "0x" << ((c<16)?"0":"") << int(c) << ",";
	}
	for( unsigned char c : str )
		definitions << "0x" << ((c<16)?"0":"") << int(c) <<	",";
	definitions << "0x00\n";
	return ret;
}

void assemblyGenerator::storeRegister( register_t r ) {
	/*if( variable_register.at( register_variable.at(r) ).end <= line ) { // is alive, do a store

	}*/
}

void assemblyGenerator::loadVariable( variable_t x, register_t r ) {

}

void assemblyGenerator::print( std::ostream& os ) const {
	os << definitions.str() << std::endl << instructions.str() << std::endl;
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
	register_variable.resize( register_count, ERROR_VARIABLE );
}