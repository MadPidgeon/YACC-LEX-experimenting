#pragma once
#include <cstdint>

enum node_t {
	INTEGER, FLOAT, STRING, VARIABLE,

	ASSIGN, 

	ADD, SUBTRACT,

	ARGUMENT_LIST
};

class syntaxTree {
public:
	struct node {
		node_t type;
		union extra_data_t {
			int64_t integer;
			double floating;
		};
		extra_data_t data;
		node* parent;
		node* children[2];
		node( node_t node_type, node* left = nullptr, node* right = nullptr, extra_data_t d = {0} );
	};
private:
	node* root;
public:

};