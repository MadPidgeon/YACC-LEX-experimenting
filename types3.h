#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <deque>
#include <utility>
#include <iostream>
#include <string>
#include <map>
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
			symbol_t symbol; // debugging purposes only
			typedef_t type; // FREE_TYPEDEF for free type, otherwise free immediate of given type
		};
		struct struct_member {
			symbol_t symbol;
			enum mode_t {
				fixed_type,
				free_type
			} mode;
			union {
				typedef_t fixed;
				uint64_t free;
			} type;
			std::vector<uint64_t> parameters;
		};
		// name
		symbol_t symbol; // debugging purposes only
		typedef_t id;
		std::vector<parameter> parameters;  // parameters are referenced by index in the struct_members
		std::vector<struct_member> struct_members;
		typedef_t base;
		bool operator==( const definition& ) const; 
	};
private:
	std::deque<definition> definitions;
	std::map<const definition&,typedef_t> inverse_definitions;
	typedef_t addIfNotExists( definition );
public:
	bool isFinal( typedef_t ) const;
	typedef_t addTypeDefinition( symbol_t ); // used for defining POD
	typedef_t addTypeDefinition( symbol_t, std::vector<std::pair<typedef_t,symbol_t>> ); // used for defining non-template structs
	typedef_t addTypeDefinition( symbol_t, std::vector<definition::parameter>, std::vector<definition::struct_member> ); // used for defining template structs
	typedef_t addTypeSpecialization( symbol_t, typedef_t, std::vector<definition::parameter> ); // used for defining specializations
	typedef_t findTypeDefinition( const definition& d ) const;
	typedefTable();
};

// global data
uint64_t structure_t;
class structureTable {
	struct structure {
		symbol_t symbol;
		structure_t id;
		struct member {
			symbol_t symbol;
			type_t type;
		};
		std::vector<type_t> parameters; // empty type for free type, otherwise fully specialized only
		std::vector<member> members;
	};
	std::vector<structure> structures;
public:
	// stuff
};

// scope data
class typedefTable {
	struct typedefinition {
		structure_t base;
		symbol_t symbol; // debug only
		std::vector<type_t> parameters; 
		std::vector<type_t> substitutions;
		// example: def yolo2(type x,int y) = yolo(int,lst(x),y)
		// base = yolo
		// parameters = (nil,int)
		// substitutions = (int,lst(par_0),par_1)
	};
	std::unordered_map<symbol_t,typedefinition> defintions;
public:
	// stuff
};

// everywhere data
class type_t {
	int64_t free_id; // -2 = no free members; -1 = not free variable; 0,1,2,... = free variable with index
	structure_t structure;
	std::vector<type_t*> parameters;
public:
	bool operator==( const type_t& ) const;
	type_t( structure_t, std::vector<type_t*> );
}