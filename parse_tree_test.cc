#include <iostream>
#include <sstream>
#include "symbol_table.h"
#include "tokenizer.h"
#include "parse_tree.h"

std::string code = "int x = 0, y = 2;\nif( x == 0 ) [ x = x + 1; ]\nint y = 3*x+1;\nif( f(x,y,z) == x ) x = y / 2;\nif( x == 0 ) if( y == 0 ) z = 0; else z = 1;\nfor( z in {1,2,3} ) { [ ( x, y, z ) = (1,2,3) ] }";

int main() {
	symbolTable symtab;
	std::istringstream input( code );
	tokenizer tokgen( input, &symtab );
	auto tokens = tokgen.getAllTokens();
	/*for( token_t t : tokens )
		std::cout << t << " ";
	std::cout << std::endl;*/
	parseTree partree( tokens );
	std::cout << partree << std::endl;
}