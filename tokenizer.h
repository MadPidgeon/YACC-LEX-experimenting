#pragma once
#include <string>
#include <deque>
#include <vector>
#include <utility>
#include <iostream>
#include <cstdint>
#include "error_reporting.h"
#include "symbol_table.h"

#define DEBUG_TOKENIZER

#define EOF_TOKEN	token_t{ token_t::id_t::END_OF_FILE, {.integer = 0} }

#define DATA_BRACKET_TOKEN 		0
#define STATEMENT_BRACKET_TOKEN 1

union extra_data_t {
	int64_t integer;
	uint64_t op;
	symbol_t symbol;
	char* string;
	double floating;
};

struct position_t {
	size_t row, column;
};

struct token_t {
	enum id_t {
		END_OF_FILE, START_OF_FILE,

		LBRA, RBRA, LSEQ, RSEQ, LPAR, RPAR, COMMA, DOT, ELLIPSIS, SEMICOLON, SPACE_JOINER,

		ASSIGN, ID, FUNCTION, INT, FLT, STR,

		DEFINE, MAPS_TO,

		IF, WHILE, FOR, ELSE,

		RETURN, BREAK, CONTINUE,

		LOGIC_OP, RELATION_OP, IN_OP, LATTICE_OP, SIZE_OP, ADD_OP, MULT_OP, EXPONENT_OP,

		LOGIC_ASSIGN_OP, LATTICE_ASSIGN_OP, ADD_ASSIGN_OP, MULT_ASSIGN_OP, EXPONENT_ASSIGN_OP,

		INCREMENT,

		COUNT
	} id;
	static const std::vector<std::string> token_name;
	static const std::vector<int> token_precedence;
	static const std::vector<int> token_associativity;
	extra_data_t data;
	position_t pos;
	bool operator==( token_t other ) const;
	bool operator!=( token_t other ) const;
	token_t getDual() const;
	bool isAtom() const;
	bool isBinaryOperator() const;
	bool isUnaryOperator() const;
	bool isLeftBracket() const;
	bool isRightBracket() const;
	bool isControlFlow() const;
	bool isLeftAssociative() const;
	bool isRightAssociative() const;
	bool isStatement() const;
	int precedence() const;
};

std::ostream& operator<<( std::ostream& os, token_t );

class tokenizer {
	std::istream& stream;
	position_t current_position;
	char c;
	token_t last_added_token;
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
	void generateAssignment();
	void generateComparison();

	void recoverError();
public:
	typedef token_t::id_t TK;
	std::vector<token_t> getAllTokens();
	token_t getToken();
	token_t peekToken( int i );
	tokenizer( std::istream&, symbolTable* );
};
