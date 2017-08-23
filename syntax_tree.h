#pragma once
#include <cstdint>
#include <iostream>
#include "types.h"
#include "debug.h"
#include "scope_table.h"

// #define SYNTAX_TREE_DEBUG

enum node_t {
	N_INTEGER, N_FLOAT, N_STRING, N_VARIABLE,

	N_ASSIGN, N_GARBAGE,

	N_COMPARISON_CHAIN,

	N_EQUALS, N_NOT_EQUALS, N_LESSER, N_GREATER, N_LESSER_EQ, N_GREATER_EQ, 

	N_ADD, N_SUBTRACT, N_MULTIPLY, N_DIVIDE, N_REMAINDER, N_UMIN,

	N_JOIN, N_MEET,

	N_ARGUMENT_LIST,

	N_IF, N_WHILE, N_ELSE, N_FOR, N_BREAK, N_CONTINUE,

	N_SEQUENTIAL_BLOCK, N_PARALLEL_BLOCK, N_BLOCK_LIST,

	N_LIST, N_SET, N_SINGLE_TYPE_EXPRESSION_LIST,

	N_TUPLE, N_TUPLE_LIST,

	N_FUNCTION_CALL,

	N_LIST_INDEXING,

	N_EMPTY,

	N_COUNT
};

class syntaxTree {
public:
	struct node {
		node_t type;
		union extra_data_t {
			int64_t integer;
			double floating;
			char* string;
		};
		extra_data_t data;
		node* parent;
		node* children[2];
		type_t data_type;
		type_t computeDatatype();
		bool isIntegral() const;
		void setType( node_t );
		void print( std::ostream&, bool print_nulls = true, int depth = 0 ) const;
		node( node_t node_type, node* left = nullptr, node* right = nullptr, extra_data_t d = {0} );
		~node();
	};
private:
	node* root;
public:
	static scopeTable* scopes; // ugly, plz fix
	syntaxTree( scopeTable* );
	void setRoot( node* );
	const node* getRoot() const;
};

std::ostream& operator<<( std::ostream&, const syntaxTree& );
