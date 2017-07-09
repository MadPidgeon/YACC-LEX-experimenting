#include "symbol_table.h"
#include <unordered_map>
#include <deque>
#include <string>

std::string symbolTable::getName( symbol_t num ) const {
	return int_to_str.at(num);
}

symbol_t symbolTable::getSymbol( std::string str ) const {
	return str_to_int.at(str);
}

bool symbolTable::symbolExists( std::string str ) const {
	return str_to_int.find( str ) != str_to_int.end();
}

symbol_t symbolTable::addSymbol( std::string str ) {
	if( symbolExists( str ) )
		return getSymbol( str );
	symbol_t num = int_to_str.size();
	int_to_str.push_back( str );
	str_to_int[str] = num;
	return num;
}

symbolTable::symbolTable() {
	addSymbol("-ERROR-"); // note that "-ERROR-" is not a legal token by the grammar, hence no conflict arrises
}