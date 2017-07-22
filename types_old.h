#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <deque>
#include <utility>
#include <iostream>
#include <string>
#include "symbol_table.h"

#define ERROR_TYPE		0
#define VOID_TYPE		1
#define INT_TYPE		2
#define FLT_TYPE		3
#define STR_TYPE		4

// all non-POD types are considered tuples
#define ERROR_TYPEDEF	0	// ERR
#define FREE_TYPEDEF	1	// FTD
#define VOID_TYPEDEF	2	// POD ()
#define INT_TYPEDEF		3	// POD int
#define FLT_TYPEDEF		4	// POD flt
#define STR_TYPEDEF		5	// POD str
#define LIST_TYPEDEF	6	// POD lst(type)
#define SET_TYPEDEF		7	// POD set(type)
#define LAST_POD_TYPEDEF	SET_TYPEDEF

typedef uint64_t type_t;
typedef uint64_t typedef_t;

struct immediateValue {
	type_t type;
	std::vector<immediateValue*> sub_values;
	union {
		int64_t integer;
		double floating;
		char* string;
	} value;
};

std::ostream& operator<<( std::ostream& os, const immediateValue v );

// Type definitions may have variable parameters:
// class yolo( type t, int x ) = (
//    t a,
//    t b,
// );
// fnc to_str( yolo(t,x) y ) -> str [
//    return to_str(x) + ": " + to_str( y[0] ) + ", " + to_str( y[1] );
// ]
// Such type definition are named and stored in the typedefTable, like lst(type) and arr(type,int)
// Instantiations of such a type are unnamed and are stored in the typeTable, like lst(int) and arr(int,5)

class typedefTable {
public:
	struct definition {
		struct parameter {
			symbol_t symbol;
			enum mode_t {
				free_type,
				structure,
				free_immediate,
				fixed_immediate,
				rename
			} mode;
			union {
				uint64_t free_type;
				typedef_t structure;
				symbol_t free_immediate;
				immediateValue* fixed_immediate;
			} type;
		};
		struct struct_member {
			symbol_t symbol;
			typedef_t type;
			std::vector<parameter> parameters;
		};
		// name
		symbol_t symbol; // should this belong here?
		typedef_t id;
		std::vector<parameter> parameters;  // parameters are referenced by index in the struct_members
		std::vector<struct_member> struct_members;
		typedef_t base;
		std::vector<typedef_t> specializations;
		bool operator==( const definition& ) const;
	};
private:
	std::deque<definition> definitions;
	typedef_t addIfNotExists( definition );
public:
	bool isFinal( typedef_t ) const;
	typedef_t addTypeDefinition( symbol_t );
	typedef_t addTypeDefinition( symbol_t, std::vector<std::pair<typedef_t,symbol_t>> );
	typedef_t addTypeDefinition( symbol_t, std::vector<definition::parameter>, std::vector<definition::struct_member> );
	typedef_t addTypeSpecialization( symbol_t, typedef_t, std::vector<definition::parameter> );
	typedef_t findTypeDefinition( const definition& d ) const;
	typedefTable();
};


class typeTable {
	struct type {
		type_t id;
		typedef_t def;
		size_t size;
	};
	const typedefTable* definitions;
	std::deque<type> types;
	std::map<typedef_t,type_t> inverse_def;
public:
	type_t addType( typedef_t );
	// type_t getType( typedef_t );
	typedef_t getTypedef( type_t ) const;
	size_t getSize( type_t ) const;
	typeTable( const typedefTable* );
};