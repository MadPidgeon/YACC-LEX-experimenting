#pragma once
#include <unordered_map>
#include <deque>
#include <string>

#define ERROR_SYMBOL	0

typedef uint64_t symbol_t;

class symbolTable {
	std::unordered_map<std::string,symbol_t> str_to_int;
	std::deque<std::string> int_to_str;
public:
	std::string getName( symbol_t ) const;
	symbol_t getSymbol( std::string ) const;
	bool symbolExists( std::string ) const;
	symbol_t addSymbol( std::string );
	symbolTable();
};