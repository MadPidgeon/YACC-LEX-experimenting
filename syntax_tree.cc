#include "syntax_tree.h"

syntaxTree::node::node( node_t node_type, node* left, node* right, extra_data_t d ) {
	type = node_type;
	children[0] = left;
	children[1] = right;
	parent = nullptr;
	for( int i = 0; i < 2; ++i )
		if( children[i] )
			children[i]->parent = this;
	data = d;
}
