#pragma once
#include <iostream>
#include <iomanip>

extern int err_line_number;
extern int err_character_number;
extern int err_line_character_number;

#define YY_USER_ACTION 		{ err_character_number+=yyleng; err_line_character_number+=yyleng; }
#define YY_READ_NEWLINE		(++err_line_number,err_line_character_number=0)
#define lerr				std::cout

struct error_line {
	int line, position;
	bool warning;
	error_line( bool w = false );
	error_line( int l, int p, bool w = false );
};

std::ostream& operator<<( std::ostream& os, error_line e );
