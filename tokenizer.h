#pragma once
#include <string>
#include <deque>
#include <iostream>
#include "error_reporting.h"
#include "symbol_table.h"

#define EOF_TOKEN	token_t{ token_t::id_t::EOF, .integer = 0 }

struct position_t {
	size_t row, column;
};

struct token_t {
	enum id_t {
		EOF,

		LBRA, RBRA, LSEQ, RSEQ, LPAR, RPAR, COMMA, DOT, ELLIPSIS, SEMICOLON,

		ASSIGN, ID, INT, FLT, STR,

		DEFINE, MAPS_TO,

		IF, WHILE, FOR, ELSE,

		RETURN, BREAK, CONTINUE,

		LOGIC_OP, RELATION_OP, IN_OP, LATTICE_OP, SIZE_OP, ADD_OP, MULT_OP, EXPONENT_OP,

		LOGIC_ASSIGN_OP, LATTICE_ASSIGN_OP, ADD_ASSIGN_OP, MULT_ASSIGN_OP, EXPONENT_ASSIGN_OP,

		INCREMENT
	} id;
	union {
		symbol_t identifier;
		uint64_t op;
		int64_t integer;
		double floating;
		char* string;
	};
	position_t pos;
};

class tokenizer {
	std::istream& stream;
	position_t current_position;
	char c;
	std::deque<token_t> tokens;
	symbolTable* symtab;

	char getNextCharacter();
	void addToken( token_t::id_t id, int64_t i = 0 );
	void addFloating( double f );
	void addString( const std::string& );

	void generateTokens( int i );
	void generateBrackets();
	void generateAddition();
	void generateMultiplication();
	void generateLattice();
	void eatWhitespace();
	void generateIdentifier();
	void generateLineComment();
	void generateInlineComment();
	void generateDot();
	void generateMantissa( int64_t i = 0 );
	void generateInteger();
	void generateString();
	void generateTokens();
public:
	token_t getToken();
	token_t peekToken( int i );
	tokenizer( std::istream&, symbol_table* );
};
