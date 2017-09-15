#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include "types.h"
#include "debug.h"
#include "scope_table.h"
#include "parse_tree.h"

#define SYNTAX_TREE_DEBUG

class syntaxTree {
public:
	struct node {
		enum id_t {
			INTEGER, FLOAT, STRING, VARIABLE,

			ASSIGN, GARBAGE,

			COMPARISON_CHAIN,

			EQUALS, NOT_EQUALS, LESSER, GREATER, LESSER_EQ, GREATER_EQ, 

			ADD, SUBTRACT, MULTIPLY, DIVIDE, REMAINDER, UMIN,

			JOIN, MEET, IN,

			ARGUMENT_LIST,

			IF, WHILE, ELSE, FOR, BREAK, CONTINUE, RETURN,

			SEQUENTIAL_BLOCK, PARALLEL_BLOCK, BLOCK_LIST,

			LIST, SET, SINGLE_TYPE_EXPRESSION_LIST,  SIZE_OF,

			TUPLE, TUPLE_LIST,

			FUNCTION_CALL, FUNCTION_DEFINITION,

			LIST_INDEXING, TUPLE_INDEXING,

			EMPTY,

			COUNT
		} id;
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
		void computeTyping();
		void setType( id_t t );
		bool isIntegral() const;
		void print( std::ostream&, bool print_nulls = true, int depth = 0 ) const;
		void setChild( int i, node* n );
		node* clone() const;
		node( id_t node_type, node* left = nullptr, node* right = nullptr, extra_data_t d = {0} );
		~node();
	};
private:
	node* root;
	std::vector<scope_t> scope_stack;
	typedef parseTree::node::id_t PN;
	typedef node::id_t SN;
	node* translateExpression( const parseTree::node* ) const;
	node* translateOperator( const parseTree::node* ) const;
	node* translateControlFlow( const parseTree::node* ) const;
	node* translateContainer( const parseTree::node* ) const;
	node* recursiveTranslateContainer( const parseTree::node* n, SN join ) const;
	node* translateComparisonChain( const parseTree::node* ) const;
	node* translateStatementList( const parseTree::node* ) const;
	node* translateStatement( const parseTree::node* ) const;
	node* translateExpressionList( const parseTree::node* ) const;
	node* translateIdentifier( const parseTree::node* ) const;
	node* translateFunctionCall( const parseTree::node* ) const;
	node* translateAbstractParseList( const parseTree::node* n, PN pjoiner, SN sjoiner, bool right_final_leaf ) const;
	node* makeErrorNode() const;
	type_t translateType( const parseTree::node* ) const;
	node* translateVariableDeclaration( const parseTree::node* n, type_t t ) const;
	node* translateVariableDeclaration( const parseTree::node* n ) const;
public:
	static const std::vector<std::string> node_name;
	static scopeTable* scopes; // ugly, plz fix
	syntaxTree( scopeTable* scptab );
	syntaxTree( const parseTree& pt, scopeTable* scptab );
	void computeTyping();
	void setRoot( node* );
	void print( std::ostream&, bool print_nulls = false ) const;
	const node* getRoot() const;
};

std::ostream& operator<<( std::ostream&, const syntaxTree::node& );
std::ostream& operator<<( std::ostream&, const syntaxTree& );
