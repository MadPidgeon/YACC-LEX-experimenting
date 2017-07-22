#include "error_reporting.h"

int err_line_number = 0;
int err_character_number = 0;
int err_line_character_number = 0;

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
	os << e.line << ":" << e.position << ": ";
	if( e.warning )
		os << "\033[35mWarning:\033[0m ";
	else
		os << "\033[1;31mError:\033[0m ";
	return os;
}