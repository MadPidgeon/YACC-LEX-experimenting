#include <iostream>
#include <fstream>
#include "error_reporting.h"
#include "interface.h"
#include "tokenizer.h"
#include "parse_tree.h"
#include "syntax_tree.h"
#include "intermediate_code.h"
#include "assembly.h"

tokenizer* tokgen;
parseTree* partree;
symbolTable* symtab;
structureTable* strtab;
scopeTable* scptab;
syntaxTree* syntree;

int main( int argc, char** argv ) {
	symtab = new symbolTable;
	strtab = new structureTable;
	scptab = new scopeTable( symtab, strtab );
	command_line_data settings;
	if( not settings.parse( argc, argv ) )
		return -1;

	// tokenizer
	std::vector<token_t> tokens;
	{
		std::ifstream infile( settings.infilenames[0] );
		if( !infile ) {
			printf("Couldn't open input file!\n");
			return -1;
		}
		tokgen = new tokenizer( infile, symtab );
		if( err_count > 0 )
			return -1;
		tokens = tokgen->getAllTokens();
	}
	lexer_out << "Tokens:" << std::endl;
	for( token_t t : tokens ) {
		lexer_out << t << " ";
	}
	lexer_out << std::endl;
	
	// parser
	partree = new parseTree( tokens );
	parse_out << "\nParse Tree:" << std::endl;
	parse_out << (*partree) << std::endl;
	if( err_count > 0 )
		return -1;
	parse_out << "Symbol Table:\n" << (*symtab) << std::endl;

	// syntaxer
	syntree = new syntaxTree( *partree, scptab );
	syntree_out << "\nSyntax Tree:";
	if( syntree_out.enabled )
		syntree->print( syntree_out.stream, true );
	syntree_out << "\n\nScope Table:" << std::endl << (*scptab) << std::endl;
	if( err_count > 0 )
		return -1;
	if( settings.output_format == command_line_data::output_format_t::PARSE )
		return 0;

	// intermediate code
	intermediateCode ic( scptab );
	ic.defineFunction( GLOBAL_FUNCTION, syntree->getRoot() );
	if( err_count > 0 )
		return -1;
	ic_out << "Intermediate Code:" << std::endl << ic << std::endl;

	// assembly
	assemblyGenerator ag( ic, settings.optimization_level );
	asm_out << "Assembly:" << std::endl;
	if( asm_out.enabled )
		ag.print( asm_out.stream, false );

	// output assembly
	std::string asmofn = current_directory + "tmp/out.asm";
	if( settings.output_format == command_line_data::output_format_t::ASSEMBLY )
		asmofn = settings.outfilename;
	{
		std::ofstream ofile( asmofn );
		ofile << ag << std::endl;
	}
	if( settings.output_format == command_line_data::output_format_t::ASSEMBLY )
		return 0;

	// compile to objects
	std::string objofn = current_directory + "tmp/out.o";
	if( settings.output_format == command_line_data::output_format_t::OBJECT )
		objofn = settings.outfilename;
	int asm_ret = system( ( "nasm -f elf64 " + asmofn + " -o " + objofn ).c_str() );
	if( asm_ret != 0 ) {
		std::cout << ERROR_FORMATTED_STRING << "Generated assembly won't compile (this is a bug in the compiler)" << std::endl;
		return asm_ret;
	}
	if( settings.output_format == command_line_data::output_format_t::OBJECT )
		return 0;

	// compile to binary
	int lnk_ret = system( ( "ld -o " + settings.outfilename + " " + objofn ).c_str() );
	if( lnk_ret != 0 ) {
		std::cout << ERROR_FORMATTED_STRING << "Generated assembly won't link (this is a bug in the compiler)" << std::endl;
		return lnk_ret;
	}
	return 0;
}