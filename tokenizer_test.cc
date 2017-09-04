#include <iostream>
#include "tokenizer.h"
#include "symbol_table.h"

int main() {
	symbolTable symtab;
	tokenizer( std::cin, &symtab );
	token_t t = tokenizer.getToken();
	while( t != EOF_TOKEN ) {
		std::cout << int( t.id ) << " ";
	}
	return 0;
}