#pragma once
#include <iostream>
#include <iomanip>

extern int err_line_number;
extern int err_character_number;
extern int err_line_character_number;

#define YY_USER_ACTION 		{ err_character_number+=yyleng; err_line_character_number+=yyleng; }
#define YY_READ_NEWLINE		(++err_line_number,err_line_character_number=0)
#define lerr				std::cout
#define ERROR_FORMATTED_STRING    "\033[1;31mError:\033[0m "
#define WARNING_FORMATTED_STRING  "\033[35mWarning:\033[0m "

struct error_line {
	int line, position;
	bool warning;
	error_line( bool w = false );
	error_line( int l, int p, bool w = false );
};

std::ostream& operator<<( std::ostream& os, error_line e );

struct optional_output {
	bool enabled;
	std::ostream& stream;
	optional_output( std::ostream& os );
};

template<typename T> optional_output& operator<<( optional_output& os, T x ) {
	if( os.enabled )
		os.stream << x;
	return os;
}

extern optional_output lexer_out;
extern optional_output syntree_out;