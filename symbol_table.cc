#include "symbol_table.h"
#include <unordered_map>
#include <deque>
#include <string>
#include <cassert>

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

symbol_t symbolTable::addTemporarySymbol() {
	return addSymbol("$t" + std::to_string( temporaries_counter++ ));
}

symbolTable::symbolTable() {
	temporaries_counter = 0;
	assert( addSymbol("-ERROR-") == ERROR_SYMBOL ); // note that "-ERROR-" is not a legal token by the grammar, hence no conflict arrises
	assert( addSymbol("-NONE-") == NONE_SYMBOL );
	assert( addSymbol("int") == INT_SYMBOL );
	assert( addSymbol("flt") == FLT_SYMBOL );
	assert( addSymbol("str") == STR_SYMBOL );
	assert( addSymbol("lst") == LST_SYMBOL );
	assert( addSymbol("set") == SET_SYMBOL );
	assert( addSymbol("nil") == VOID_SYMBOL );
	assert( addSymbol("tup") == TUP_SYMBOL );
	assert( addSymbol("print") == PRINT_SYMBOL );
	assert( addSymbol("scan") == SCAN_SYMBOL );
	assert( addSymbol("fnc") == FNC_SYMBOL );
}