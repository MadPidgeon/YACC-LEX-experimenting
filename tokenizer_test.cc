#include <iostream>
#include "tokenizer.h"
#include "symbol_table.h"

int main() {
	symbolTable symtab;
	tokenizer tok( std::cin, &symtab );
	token_t t = tok.getToken();
	while( t != EOF_TOKEN ) {
		std::cout << t << " ";
		t = tok.getToken();
	}
	return 0;
}