#include "interface.h"
#include "error_reporting.h"
#include <cstring>

std::string current_directory;

bool command_line_data::parse( int argc, char** argv ) {
	int last_slash = -1;
	for( int i = 0; argv[0][i] != '\0'; ++i )
		if( argv[0][i] == '/' or argv[0][i] == '\\' )
			last_slash = i;
	current_directory = std::string( argv[0], last_slash+1 );
	if( last_slash == -1 ) {
		std::cout << ERROR_FORMATTED_STRING << "Cannot yet execute from $PATH" << std::endl;
		return false;
	}
	for( int i = 1; i < argc; ++i ) {
		if( argv[i][0] == '-' ) {
			if( strcmp( argv[i]+1, "p" ) == 0 ) {
				output_format = PARSE;
			} else if( strcmp( argv[i]+1, "S" ) == 0 ) {
				output_format = ASSEMBLY;
			} else if( strcmp( argv[i]+1, "c" ) == 0 ) {
				output_format = OBJECT;
			} else if( strcmp( argv[i]+1, "o" ) == 0 ) {
				outfilename = argv[++i];
			} else if( strcmp( argv[i]+1, "-version" ) == 0 ) {
				std::cout << "  Version 0.0.0" << std::endl << "  Copyright (C) 2017" << std::endl;
			} else if( strcmp( argv[i]+1, "-help" ) == 0 ) {
				std::cout << "Usage: " << argv[0] << " file... [options]" << std::endl;
				std::cout << "Options:" << std::endl;
				std::cout << "  -o <file>   Set output file name (default=\"a.out\")" << std::endl;
				std::cout << "  -p          Produce no output file, but do parse the input" << std::endl;
				std::cout << "  -S          Output assembly in Intel syntax" << std::endl;
				std::cout << "  -c          Output object file" << std::endl;
				std::cout << "  --version   Display version information" << std::endl;
				std::cout << "  --help      Display this help text" << std::endl;
				std::cout << "  -v[lsioa]   Display verbose (debug) information about compilation steps:" << std::endl;
				std::cout << "                l    Display token generation (lexer) information" << std::endl;
				std::cout << "                s    Display syntax tree generation information" << std::endl;
				std::cout << "                i    Display intermediate code generation information" << std::endl;
				std::cout << "                o    Display optimization information" << std::endl;
				std::cout << "                a    Display assembly generation information" << std::endl;
				std::cout << "  --verbose   Synonymous with -vlsioa" << std::endl;
				return false;
			} else if( argv[i][1] == 'v' ) {
				for( int j = 2; argv[i][j] != '\0'; ++j ) {
					if( argv[i][j] == 'l' )
						lexer_out.enabled = true;
					else if( argv[i][j] == 's' )
						syntree_out.enabled = true;
					else if( argv[i][j] == 'i' )
						ic_out.enabled = true;
					else if( argv[i][j] == 'o' )
						opt_out.enabled = true;
					else if( argv[i][j] == 'a' )
						asm_out.enabled = true;
					else {
						std::cout << ERROR_FORMATTED_STRING << "Unrecognised verbosity option \'" << argv[i][j] << "\'" << std::endl;
						return false;
					}
				}
			} else if( strcmp( argv[i]+1, "-verbose" ) == 0 ) {
				lexer_out.enabled = syntree_out.enabled = ic_out.enabled = opt_out.enabled = asm_out.enabled = true;
			} else {
				std::cout << ERROR_FORMATTED_STRING << "Unrecognised option \"" << argv[i] << "\"" << std::endl;
				return false;
			}
		} else
			infilenames.emplace_back( argv[i] );
	}
	if( infilenames.size() == 0 ) {
		std::cout << ERROR_FORMATTED_STRING << "No input files specified" << std::endl;
		return false;
	}
	if( infilenames.size() > 1 ) {
		std::cout << WARNING_FORMATTED_STRING << "Multiple input files not yet supported (only first file will be parsed)" << std::endl;
		return true;
	}
	return true;
}

command_line_data::command_line_data() {
	outfilename = "a.out";
}