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

			LIST, SET, SINGLE_TYPE_EXPRESSION_LIST, SIZE_OF,

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
	node* translateExpression( const parseTree::node* );
	node* translateOperator( const parseTree::node* );
	node* translateControlFlow( const parseTree::node* );
	node* translateContainer( const parseTree::node* );
	node* recursiveTranslateContainer( const parseTree::node* n, SN join );
	node* translateComparisonChain( const parseTree::node* );
	node* recursiveTranslateTypedArguments( const parseTree::node* );
	node* translateTypedArguments( const parseTree::node* );
	node* recursiveTranslateStatementList( const parseTree::node* );
	node* translateStatementList( const parseTree::node* );
	node* translateStatement( const parseTree::node* );
	node* translateExpressionList( const parseTree::node* );
	node* translateIdentifier( const parseTree::node* );
	node* translateFunctionCall( const parseTree::node* );
	node* translateInlineFunctionDefinition( const parseTree::node* );
	node* translateAbstractParseList( const parseTree::node* n, PN pjoiner, SN sjoiner, bool right_final_leaf );
	node* makeErrorNode() const;
	type_t translateType( const parseTree::node* );
	node* translateVariableDeclaration( const parseTree::node* n, type_t t );
	node* translateVariableDeclaration( const parseTree::node* n );
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
