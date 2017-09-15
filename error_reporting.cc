#include <iostream>
#include "error_reporting.h"

int err_line_number = 1;
int err_character_number = 1;
int err_line_character_number = 1;
int err_count = 0;
optional_output lexer_out( std::cout );
optional_output parse_out( std::cout );
optional_output syntree_out( std::cout );
optional_output ic_out( std::cout );
optional_output opt_out( std::cout );
optional_output asm_out( std::cout );

error_line::error_line( bool w ) {
	line = err_line_number;
	position = err_line_character_number;
	warning = w;
}

error_line::error_line( int l, int p, bool w ) {
	line = l;
	position = p;
	warning = w;
}

std::ostream& operator<<( std::ostream& os, error_line e ) {
	std::string s = std::to_string( e.line ) + ":" + std::to_string( e.position ) + ":";
	s.resize( 8, ' ' );
	os << s;
	if( e.warning )
		os << "\033[35mWarning:\033[0m ";
	else {
		err_count++;
		os << "\033[1;31mError:\033[0m ";
	}
	return os;
}

optional_output& operator<<( optional_output& os, decltype(std::endl<char, std::char_traits<char>> ) t ) {
	if( os.enabled )
		os.stream << t;
	return os;
}

optional_output::optional_output( std::ostream& os )
	: enabled( false ), stream( os ) {
}

