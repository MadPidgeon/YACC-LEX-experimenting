#include "tokenizer.h"
#include <cstring>
#include <cassert>
#include <stdexcept>
#include <limits>

const std::vector<std::string> token_t::token_name = {
	"END_OF_FILE", "START_OF_FILE",

	"LBRA", "RBRA", "LSEQ", "RSEQ", "LPAR", "RPAR", "COMMA", "DOT", "ELLIPSIS", "SEMICOLON", "SPACE_JOINER",

	"ASSIGN", "ID", "FUNCTION", "INT", "FLT", "STR",

	"DEFINE", "MAPS_TO",

	"IF", "WHILE", "FOR", "ELSE",

	"RETURN", "BREAK", "CONTINUE",

	"LOGIC_OP", "RELATION_OP", "IN_OP", "LATTICE_OP", "SIZE_OP", "ADD_OP", "MULT_OP", "EXPONENT_OP",

	"LOGIC_ASSIGN_OP", "LATTICE_ASSIGN_OP", "ADD_ASSIGN_OP", "MULT_ASSIGN_OP", "EXPONENT_ASSIGN_OP",

	"INCREMENT"
};

bool token_t::operator==( token_t other ) const {
	return id == other.id;
}

bool token_t::operator!=( token_t other ) const {
	return id != other.id;
}

token_t token_t::getDual() const {
	if( id == id_t::LBRA )
		return token_t{ id_t::RBRA };
	else if( id == id_t::RBRA )
		return token_t{ id_t::LBRA };
	else if( id == id_t::LSEQ )
		return token_t{ id_t::RSEQ };
	else if( id == id_t::RSEQ )
		return token_t{ id_t::LSEQ };
	else if( id == id_t::LPAR )
		return token_t{ id_t::RPAR };
	else if( id == id_t::RPAR )
		return token_t{ id_t::LPAR };
	else
		return *this;
}

bool token_t::isBinaryOperator() const {
	return ( id >= id_t::LOGIC_OP and id <= EXPONENT_OP and id != SIZE_OP ) or ( id >= id_t::LOGIC_ASSIGN_OP and id <= id_t::EXPONENT_ASSIGN_OP ) or id == id_t::ASSIGN or id == id_t::SPACE_JOINER or id == id_t::COMMA or id == id_t::MAPS_TO;
}

bool token_t::isUnaryOperator() const {
	return id == id_t::SIZE_OP;
}

bool token_t::isAtom() const {
	return id == id_t::ID or id == id_t::INT or id == id_t::FLT or id == id_t::STR;
}

bool token_t::isLeftBracket() const {
	return id == id_t::LBRA or id == id_t::LSEQ or id == id_t::LPAR;
}

bool token_t::isRightBracket() const {
	return id == id_t::RBRA or id == id_t::RSEQ or id == id_t::RPAR;
}

bool token_t::isControlFlow() const {
	return id == id_t::IF or id == id_t::FOR or id == id_t::WHILE or id == id_t::ELSE or id == id_t::CONTINUE or id == id_t::BREAK or id == id_t::RETURN;
}

bool token_t::isLeftAssociative() const {
	return ( id == id_t::LOGIC_OP or id == id_t::LATTICE_OP or id == id_t::ADD_OP or id == id_t::MULT_OP or id == id_t::SPACE_JOINER );
}

bool token_t::isRightAssociative() const {
	return not isLeftAssociative(); // temporary
}

bool token_t::isStatement() const {
	return isControlFlow(); // temporary (includes declarations)
}

int token_t::precedence() const {
	if( id == id_t::SPACE_JOINER )
		return 1000;
	if( id == id_t::MAPS_TO )
		return 500;
	if( id == id_t::INCREMENT )
		return 100;
	if( id >= id_t::LOGIC_OP and id <= id_t::EXPONENT_OP )
		return 10 + id - id_t::LOGIC_OP;
	if( id >= id_t::LOGIC_ASSIGN_OP and id <= id_t::EXPONENT_ASSIGN_OP )
		return 5;
	if( id == id_t::ASSIGN )
		return 5;
	if( id == id_t::COMMA )
		return 2;
	lerr << error_line() << "Token " << (*this) << " is not an operator" << std::endl;
	throw; 
}

std::ostream& operator<<( std::ostream& os, token_t t ) {
	extern symbolTable* symtab;
	if( t.id == token_t::id_t::ID )
		return os << token_t::token_name.at( t.id ) << "(" << symtab->getName( t.data.symbol ) << ")";
	return os << token_t::token_name.at( t.id );
}

char tokenizer::getNextCharacter() {
	char character;
	stream.get( character );
	if( stream.eof() )
		return 0;
	++current_position.column;
	if( character == '\n' ) {
		++current_position.row;
		current_position.column = 0;
	}
	return character;
}

void tokenizer::addToken( token_t::id_t id, int64_t i ) {
	last_added_token = token_t{ id, {.integer=i}, current_position };
	tokens.push_back( last_added_token );
}

void tokenizer::addFloating( double f ) {
	tokens.push_back( token_t{ TK::FLT, {.floating=f}, current_position } );
}

void tokenizer::addString( const std::string& s ) {
	tokens.push_back( token_t{ TK::STR, {.string=strdup(s.c_str())}, current_position } );
}

void tokenizer::generateBrackets() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateBrackets()" << std::endl;
	#endif
	assert( c == '(' or c == ')' or c == '[' or c == ']' or c == '{' or c == '}' );
	// a_(...), [...]_(...), {...}_(...)
	// int -> (int,str)_[ yolo ]
	if( ( last_added_token.id == TK::ID or last_added_token.isRightBracket() ) and ( c == '(' or c == '[' or c == '{' ) )
		addToken( TK::SPACE_JOINER );
	if( c == '(' )
		addToken( TK::LBRA );
	else if( c == ')' )
		addToken( TK::RBRA );
	else if( c == '[' )
		addToken( TK::LSEQ );
	else if( c == ']' )
		addToken( TK::RSEQ );
	else if( c == '{' )
		addToken( TK::LPAR );
	else if( c == '}' )
		addToken( TK::RPAR );
	c = getNextCharacter();
}

void tokenizer::generateAddition() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateAddition()" << std::endl;
	#endif
	assert( c == '+' or c == '-' );
	char d;
	uint64_t o = ( c == '-' );
	d = getNextCharacter();
	if( d == '=' ) {
		addToken( TK::ADD_ASSIGN_OP, o );
		c = getNextCharacter();
	} else if( c == d ) {
		addToken( TK::INCREMENT, o );
		c = getNextCharacter();
	} else if( c == '-' and d == '>' ) {
		addToken( TK::MAPS_TO );
		c = getNextCharacter();
	} else {
		addToken( TK::ADD_OP, o );
		c = d;
	}
}

void tokenizer::generateMultiplication() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateMultiplication()" << std::endl;
	#endif
	assert( c == '*' or c == '/' or c == '%' );
	char d;
	uint64_t o = 2*( c == '%' ) + ( c == '/' );
	d = getNextCharacter();
	if( d == '=' ) {
		addToken( TK::MULT_ASSIGN_OP, o );
		c = getNextCharacter();
	} else if( c == '*' and d == '*' ) {
		char e = getNextCharacter();
		if( e == '=' ) {
			addToken( TK::EXPONENT_ASSIGN_OP );
			c = getNextCharacter();
		} else {
			addToken( TK::EXPONENT_OP, o );
			c = e;
		}
	} else if( c == '/' and d == '/' ) {
		generateLineComment();
	} else if( c == '/' and d == '*' ) {
		c = getNextCharacter();
		generateInlineComment();
	} else {
		addToken( TK::MULT_OP, o );
		c = d;
	}
}

void tokenizer::generateLattice() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateLattice()" << std::endl;
	#endif
	assert( c == '|' or c == '&' );
	uint64_t o = ( c == '&' );
	char d = getNextCharacter();
	if( d == '=' ) {
		addToken( TK::LATTICE_ASSIGN_OP, o );
		c = getNextCharacter();
	} else if( c == d ) {
		char e = getNextCharacter();
		if( e == '=' ) {
			addToken( TK::LOGIC_ASSIGN_OP );
			c = getNextCharacter();
		} else {
			addToken( TK::LOGIC_OP, o );
			c = e;
		}
	} else {
		addToken( TK::LATTICE_OP, o );
		c = d;
	}
}

void tokenizer::eatWhitespace() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "eatWhitespace()" << std::endl;
	#endif
	while( c == ' ' or c == '\n' or c == '\t' )
		c = getNextCharacter();
}

void tokenizer::generateIdentifier() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateIdentifier()" << std::endl;
	#endif
	assert( isalpha( c ) );
	std::string id;
	while( isalnum( c ) ) {
		id.push_back( c );
		c = getNextCharacter();
	}
	if( id == "if" )
		addToken( TK::IF );
	else if( id == "else" )
		addToken( TK::ELSE );
	else if( id == "while" )
		addToken( TK::WHILE );
	else if( id == "for" )
		addToken( TK::FOR );
	else if( id == "break" )
		addToken( TK::BREAK );
	else if( id == "continue" )
		addToken( TK::CONTINUE );
	else if( id == "return" )
		addToken( TK::RETURN );
	else if( id == "in" )
		addToken( TK::IN_OP );
	else {
		symbol_t s = symtab->addSymbol( id );
		// (...) a, b a
		if( last_added_token.id == TK::RBRA or last_added_token.id == TK::ID )
			addToken( TK::SPACE_JOINER );
		addToken( TK::ID, s );
	}
}

void tokenizer::generateLineComment() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateLineComment()" << std::endl;
	#endif
	while( c != '\n' and c != 0 )
		c = getNextCharacter();
}

void tokenizer::generateInlineComment() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateInlineComment()" << std::endl;
	#endif
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
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateDot()" << std::endl;
	#endif
	assert( c == '.' );
	char d;
	d = getNextCharacter();
	if( d == '.' ) {
		char e = getNextCharacter();
		c = getNextCharacter();
		if( e == '.' )
			addToken( TK::ELLIPSIS );
		else {
			addToken( TK::DOT );
			generateMantissa();
		}
	} else if( isdigit( d ) ) {
		c = d;
		generateMantissa();
	} else {
		c = d;
		addToken( TK::DOT );
	}
}

void tokenizer::generateMantissa( int64_t integer ) {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateMantissa()" << std::endl;
	#endif
	assert( c != '.' );
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
		addFloating( f );
	}	
}

void tokenizer::generateInteger() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateInteger()" << std::endl;
	#endif
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
		} else if( isdigit( d ) ) {
			lerr << error_line() << "Octal integers must be written with prefix \"0o\" instead of \"0\"" << std::endl;
			recoverError();
		} else {
			s += "0";
			c = d;
		}
	}
	while( isdigit( c ) ) {
		s.push_back( c );
		c = getNextCharacter();
	}
	int64_t i;
	try {
		i = std::stoll( s, nullptr, base );
	} catch( const std::invalid_argument& ) {
		lerr << error_line() << "Invalid number \"" << s << "\"" << std::endl;
		recoverError();
		return;
	} catch( const std::out_of_range& ) {
		lerr << error_line() << "Number \"" << s << "\" is too big" << std::endl;
		recoverError();
		return;
	}
	if( c == '.' ) {
		char d = getNextCharacter();
		if( isdigit( d ) ) {
			if( base == 10 ) {
				c = d;
				generateMantissa( i );
			} else {
				lerr << error_line() << "Floating point values must be written in decimal" << std::endl;
				recoverError();
			}
		} else {
			c = d;
			position_t p = current_position;
			eatWhitespace();
			if( isalpha( c ) ) {
				generateIdentifier();
				if( tokens.back().id == TK::ID ) {
					addToken( TK::DOT );
					addToken( TK::INT, i );
					tokens.back().pos = p;
					std::swap( tokens.back(), tokens.at( tokens.size()-3 ) );
				} else {
					addFloating( double(i) );
					tokens.back().pos = p;
					std::swap( tokens.back(), tokens.at( tokens.size()-2 ) );
				}
			} else
				addFloating( double(i) );
		}
	} else if( isalpha( c ) ) {
		lerr << error_line() << "Integer cannot contain alphabetic characters" << std::endl;
		recoverError();
	} else
		addToken( TK::INT, i );
}

void tokenizer::generateString() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "generateString()" << std::endl;
	#endif
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
			else if( c == '"' )
				s.push_back('"');
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

void tokenizer::generateAssignment() {
	char d = getNextCharacter();
	if( d == '=' ) {
		addToken( TK::RELATION_OP, 0 );
		c = getNextCharacter();
	} else {
		addToken( TK::ASSIGN, 0 );
		c = d;
	}
}

void tokenizer::generateComparison() {
	char d = getNextCharacter();
	if( d == '=' ) {
		if( c == '<' )
			addToken( TK::RELATION_OP, 4 );
		else if( c == '>' )
			addToken( TK::RELATION_OP, 5 );
		c = getNextCharacter();
	} else {
		if( c == '<' )
			addToken( TK::RELATION_OP, 2 );
		else if( c == '>' )
			addToken( TK::RELATION_OP, 3 );
		c = d;
	}
}

void tokenizer::generateTokens( int i ) {
	while( i --> 0 and c != 0 ) {
		eatWhitespace();
		// lexer_out << "(" << c << ") " << current_position.column << std::endl;
		if( c == ';' ) {
			addToken( TK::SEMICOLON );
			c = getNextCharacter();
		} else if( c == '=' )
			generateAssignment();
		else if( c == '#' ) {
			addToken( TK::SIZE_OP );
			c = getNextCharacter();
		} else if( c == ',' ) {
			addToken( TK::COMMA );
			c = getNextCharacter();
		} else if( c == '+' or c == '-' )
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
		else if( c == '<' or c == '>' )
			generateComparison();
	}
}

void tokenizer::recoverError() {
	#ifdef DEBUG_TOKENIZER
	lexer_out << "recoverError()" << std::endl;
	#endif
	while( c != ';' and c != ']' and c != 0 )
		c = getNextCharacter();
}

token_t tokenizer::getToken() {
	if( tokens.size() == 0 )
		generateTokens( 10 );
	if( tokens.size() == 0 )
		return EOF_TOKEN;
	token_t t = tokens.front();
	tokens.pop_front();
	return t;
}

std::vector<token_t> tokenizer::getAllTokens() {
	generateTokens( std::numeric_limits<int>::max() );
	std::vector<token_t> r;
	token_t t;
	do {
		t = getToken();
		r.push_back( t );
	} while( t != EOF_TOKEN );
	return r;
}

token_t tokenizer::peekToken( int i ) {
	generateTokens( i - tokens.size() + 1 );
	return tokens.at( i );
}

tokenizer::tokenizer( std::istream& is, symbolTable* st ) 
	: stream( is ), symtab( st ) {
	assert( TK::COUNT == token_t::token_name.size() );
	c = getNextCharacter();
	current_position.row = 1;
	current_position.column = 0;
	addToken( TK::START_OF_FILE );
}

// 1.234.yolo() + 1337.yolo() + .1234 * 1. + 999*666
// +1.in[1,2,3,4];break4.5*1+a+b+c+d+e