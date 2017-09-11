#pragma once
#include <cstdint>
#include <vector>
#include "tokenizer.h"

#define PARSE_TREE_DEBUG

struct interval_t {
	size_t begin, end;
	size_t size() const;
};

std::ostream& operator<<( std::ostream& os, interval_t i );

class parseTree {
	struct token_data_t {
		uint64_t pair;
	};
	std::vector<token_t> tokens;
	std::vector<token_data_t> token_data;
public:
	struct node {
		enum id_t {
			NOP,

			TUPLE, LIST, SET, SEQUENCE_PART, ELLIPSIS, SPACE_JOINER, COMMA,

			MEMBER_OF, ASSIGN, ID, INT, FLT, STR, FUNCTION_CALL,

			IF, WHILE, FOR, ELSE, 

			RETURN, BREAK, CONTINUE,

			LOGIC_OP, RELATION_OP, IN_OP, LATTICE_OP, SIZE_OP, ADD_OP, MULT_OP, EXPONENT_OP,

			LOGIC_ASSIGN_OP, LATTICE_ASSIGN_OP, ADD_ASSIGN_OP, MULT_ASSIGN_OP, EXPONENT_ASSIGN_OP,

			INCREMENT,

			COUNT
		} id;
		static const std::vector<std::string> node_name;
		node* children[2];
		node( id_t, node* a = nullptr, node* b = nullptr );
		void print( std::ostream& os, bool extra = false, int depth = 0 ) const;
	};
private:
	node* root;
public:
	void generateTokenData();
	node* parseStatementList( interval_t );
	node* parseStatement( interval_t& );
	node* parseExpression( interval_t& );
	node* parseControlFlow( interval_t& );
	static node* parseAtom( token_t t );
	static void parseOperation( std::deque<node*>& output, token_t t );
	static void parseSequence( std::deque<node*>& output, token_t t );
	void printTokens( interval_t, bool show_extra = false ) const;
	void print( std::ostream&, bool extra = false ) const;
	parseTree( const std::vector<token_t>& );
};

std::ostream& operator<<( std::ostream& os, const parseTree& );
