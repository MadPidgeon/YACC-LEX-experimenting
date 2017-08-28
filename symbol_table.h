#pragma once
#include <unordered_map>
#include <deque>
#include <string>

#define ERROR_SYMBOL	0
#define NONE_SYMBOL		1
#define INT_SYMBOL		2
#define FLT_SYMBOL		3
#define STR_SYMBOL		4
#define LST_SYMBOL		5
#define SET_SYMBOL		6
#define VOID_SYMBOL		7
#define TUP_SYMBOL		8
#define PRINT_SYMBOL	9
#define SCAN_SYMBOL		10
#define FNC_SYMBOL		11

typedef uint64_t symbol_t;

class symbolTable {
	std::unordered_map<std::string,symbol_t> str_to_int;
	std::deque<std::string> int_to_str;
	size_t temporaries_counter;
public:
	std::string getName( symbol_t ) const;
	symbol_t getSymbol( std::string ) const;
	bool symbolExists( std::string ) const;
	symbol_t addSymbol( std::string );
	symbol_t addTemporarySymbol();
	symbolTable();
};

extern symbolTable* symtab; // only to be used for I/O