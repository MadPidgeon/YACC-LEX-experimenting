#include "tokenizer.h"
#include <cstring>

char tokenizer::getNextCharacter() {
	char character;
	if( not stream )
		return 0;
	stream.get( character );
	++current_position.column;
	if( character == '\n' ) {
		++current_position.row;
		current_position.column = 0;
	}
	return character;
}

void tokenizer::addToken( token_t::id_t id, int64_t i ) {
	tokens.push_back( token_t{ id, {.integer=i}, current_position } );
}

void tokenizer::addFloating( double f ) {
	tokens.push_back( token_t{ token_t::id_t::FLT, {.floating=f}, current_position } );
}

void tokenizer::addString( std::string s ) {
	tokens.push_back( token_t{ token_t::id_t::FLT, {.string=strcpy(s.c_str())}, current_position } );
}

void tokenizer::generateBrackets() {
	assert( c == '(' or c == ')' or c == '[' or c == ']' or c == '{' or c == '}' );
	if( c == '(' )
		addToken( token_t::id_t::LBRA );
	else if( c == ')' )
		addToken( token_t::id_t::RBRA );
	else if( c == '[' )
		addToken( token_t::id_t::LSEQ );
	else if( c == ']' )
		addToken( token_t::id_t::RSEQ );
	else if( c == '{' )
		addToken( token_t::id_t::LPAR );
	else if( c == '}' )
		addToken( token_t::id_t::RPAR );
	c = getNextCharacter();
}

void tokenizer::generateAddition() {
	assert( c == '+' or c == '-' );
	char d;
	uint64_t o = ( c == '-' );
	d = getNextCharacter();
	if( d == '=' ) {
		addToken( token_t::id_t::ADD_ASSIGN_OP, o );
		c = getNextCharacter();
	} else if( c == d ) {
		addToken( token_t::id_t::INCREMENT, o );
		c = getNextCharacter();
	} else if( c == '-' and d == '>' ) {
		addToken( token_t::id_t::MAPS_TO );
		c = getNextCharacter();
	} else {
		addToken( token_t::id_t::ADD_OP, o );
		c = d;
	}
}

void tokenizer::generateMultiplication() {
	assert( c == '*' or c == '/' or c == '%' );
	char d;
	uint64_t o = 2*( c == '%' ) + ( c == '/' );
	d = getNextCharacter();
	if( d == '=' ) {
		addToken( token_t::id_t::MULT_ASSIGN_OP, o );
		c = getNextCharacter();
	} else if( c == '*' and d == '*' ) {
		char e;
		stream.get( e );
		if( e == '=' ) {
			addToken( token_t::id_t::EXPONENT_ASSIGN_OP );
			c = getNextCharacter();
		} else {
			addToken( token_t::id_t::EXPONENT_OP, o );
			c = e;
		}
	} if( c == '/' and d == '/' ) {
		generateLineComment();
	} if( c == '/' and d == '*' ) {
		c = getNextCharacter();
		generateInlineComment();
	} else {
		addToken( token_t::id_t::MULT_OP, o );
		c = d;
	}
}

void tokenizer::generateLattice() {
	assert( c == '|' or c == '&' );
	uint64_t o = ( c == '&' );
	char d = getNextCharacter();
	if( d == '=' ) {
		addToken( token_t::id_t::LATTICE_ASSIGN_OP, o );
		c = getNextCharacter();
	} else if( c == d ) {
		char e = getNextCharacter();
		if( e == '=' ) {
			addToken( token_t::id_t::LOGIC_ASSIGN_OP );
			c = getNextCharacter();
		} else {
			addToken( token_t::id_t::LOGIC_OP, o );
			c = e;
		}
	} else {
		addToken( token_t::id_t::LATTICE_OP, o );
		c = d;
	}
}

void tokenizer::eatWhitespace() {
	while( c == ' ' or c == '\n' or c == '\t' )
		c = getNextCharacter();
}

void tokenizer::generateIdentifier() {
	std::string id;
	while( isalnum( c ) ) {
		id.push_back( c );
		c = getNextCharacter();
	}
	if( id == "if" )
		addToken( token_t::id_t::IF );
	else if( id == "else" )
		addToken( token_t::id_t::ELSE );
	else if( id == "while" )
		addToken( token_t::id_t::WHILE );
	else if( id == "for" )
		addToken( token_t::id_t::FOR );
	else if( id == "break" )
		addToken( token_t::id_t::BREAK );
	else if( id == "continue" )
		addToken( token_t::id_t::CONTINUE );
	else if( id == "return" )
		addToken( token_t::id_t::RETURN );
	else {
		symbol_t s = symtab->addSymbol( id );
		addToken( token_t::id_t::ID, s );
	}
}

void tokenizer::generateLineComment() {
	while( c != '\n' and c != 0 )
		c = getNextCharacter();
}

void tokenizer::generateInlineComment() {
	char d = c;
	c = getNextCharacter();
	int i = 1;
	while( i > 0 and c != 0 ) {
		if( d == '/' and c == '*' ) {
			d = getNextCharacter();
			c = getNextCharacter();
			++i;
		} else if( d == '*' and c == '/' ) {
			d = getNextCharacter();
			c = getNextCharacter();
			--i;
		} else {
			d = c;
			c = getNextCharacter();
		}		
	}
}

void tokenizer::generateDot() {
	assert( c == '.' );
	char d;
	d = getNextCharacter();
	if( d == '.' ) {
		char e = getNextCharacter();
		c = getNextCharacter();
		if( e == '.' )
			addToken( token_t::id_t::ELLIPSIS );
		else {
			addToken( token_t::id_t::DOT );
			generateMantissa();
		}
	} else if( isdigit( d ) ) {
		c = d;
		generateMantissa();
	} else {
		c = d;
		addToken( token_t::id_t::DOT );
	}
}

void tokenizer::generateMantissa( int64_t integer ) {
	assert( c != '.' )
	std::string s = std::to_string( integer ) + ".";
	while( isdigit( c ) ) {
		s.push_back( c );
		c = getNextCharacter();
	}
	if( c == 'e' ) {
		s.push_back( c );
		c = getNextCharacter();
		if( c == '-' ) {
			s.push_back( c );
			c = getNextCharacter();
		}
		while( isdigit( c ) ) {
			s.push_back( c );
			c = getNextCharacter();
		}
	}
	if( isalpha( c ) ) {
		lerr << error_line() << "Identifier name cannot begin with a digit" << std::endl;
		recoverError();
	} else {
		double f = stod( s, nullptr );
		addToken( token_t::id_t::FLT )
	}	
}

void tokenizer::generateInteger() {
	assert( isdigit( c ) );
	int base = 10;
	std::string s;
	if( c == '0' ) {
		char d = getNextCharacter();
		if( d == 'b' ) {
			base = 2;
			c = getNextCharacter();
		} else if( d == 'x' ) {
			base = 16;
			c = getNextCharacter();
		} else if( d == 'o' ) {
			base = 8;
			c = getNextCharacter();
		} else {
			lerr << error_line() << "Octal integers must be written with prefix \"0o\" instead of \"0\"" << std::endl;
			recoverError();
		}
	}
	while( isdigit( c ) ) {
		s.push_back( c );
		c = getNextCharacter();
	}
	int64_t i = std::stoll( s, nullptr, base );
	if( c == '.' ) {
		char d = getNextCharacter();
		if( isdigit( d ) ) {
			if( base == 10 )
				generateMantissa( i );
			else {
				lerr << error_line() << "Floating point values must be written in decimal" << std::endl;
				recoverError();
			}
		} else {
			c = d;
			position_t p = current_position;
			eatWhitespace();
			if( isalpha( c ) ) {
				generateIdentifier();
				if( tokens.back().id == token_t::id_t::ID )
					addToken( token_t::id_t::INT, i );
				else
					addDouble( double(i) );
				tokens.back().pos = p;
				std::swap( tokens.back(), tokens.at( tokens.size()-2 ) );
			} else
				addDouble( double(i) );
		}
	} else if( isalpha( c ) ) {
		lerr << error_line() << "Integer cannot contain alphabetic characters" << std::endl;
		recoverError();
	} else
		addToken( token_t::id_t::INT, i );
}

void tokenizer::generateString() {
	assert( c == '"' );
	std::string s;
	c = getNextCharacter();
	while( c != '"' ) {
		if( c == '\\' ) {
			c = getNextCharacter();
			if( c == 'n' )
				s.push_back('\n');
			else if( c == 't' )
				s.push_back('\t');
			else if( c == '\\' )
				s.push_back('\\');
			else
				lerr << error_line() << "Unknown escape sequence '\\" << c << "'" << std::endl;
		} else if( c == 0 ) {
			lerr << error_line() << "Missing end of string character" << std::endl;
			break;
		} else
			s.push_back( c );
		c = getNextCharacter();
	}
	c = getNextCharacter();
	addString( s );
}

void tokenizer::generateTokens( int i ) {
	char c;
	while( i --> 0 and c != 0 ) {
		eatWhitespace();
		if( c == ';' )
			addToken( token_t::id_t::SEMICOLON );
		else if( c == '=' ) 
			addToken( token_t::id_t::ASSIGN );
		else if( c == '#' )
			addToken( token_t::id_t::SIZE_OP );
		else if( c == ',' )
			addToken( token_t::id_t::COMMA );
		else if( c == '+' or c == '-' )
			generateAddition();
		else if( c == '|' or c == '&' )
			generateLattice();
		else if( c == '*' or c == '/' or c == '%' )
			generateMultiplication();
		else if( c == '(' or c == ')' or c == '[' or c == ']' or c == '{' or c == '}' )
			generateBrackets();
		else if( c == '.' )
			generateDot();
		else if( isdigit( c ) )
			generateInteger();
		else if( isalpha( c ) )
			generateIdentifier();
		else if( c == '"' )
			generateString();
	}
}

token_t getToken() {
	if( tokens.size() == 0 )
		generateTokens( 10 );
	if( tokens.size() == 0 )
		return EOF_TOKEN;
	token_t t = tokens.front();
	tokens.pop_front();
	return t;
}

token_t peekToken( int i ) {
	generateTokens( i - tokens.size() + 1 );
	return tokens.at( i );
}

tokenizer::tokenizer( std::istream& is, symbolTable* st ) 
	: stream( is ), symtab( st ) {
}

