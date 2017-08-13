#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <deque>
#include <utility>
#include <iostream>
#include <string>
#include <map>
#include <cassert>
#include "symbol_table.h"
#include "error_reporting.h"

// all non-POD types are considered tuples
#define ERROR_STRUCTURE		0	// ERR
#define FREE_STRUCTURE		1	// FTD
#define VOID_STRUCTURE		2	// POD ()
#define INT_STRUCTURE		3	// POD int
#define FLT_STRUCTURE		4	// POD flt
#define STR_STRUCTURE		5	// POD str
#define LST_STRUCTURE		6	// POD lst(type)
#define SET_STRUCTURE		7	// POD set(type)
#define TUP_STRUCTURE		8	// POD tup(type,...)
#define LAST_POD_TYPEDEF	TUP_TYPEDEF

class type_t;
class typedefTable;
class structureTable;
typedef uint64_t structure_t;


// anywhere data
class type_t {
public:
	struct node {
		int64_t free_id; // -2 = no free members; -1 = not free type; 0,1,2,... = free variable with index
		structure_t structure;
		std::vector<node*> parameters;
		bool operator==( const node& ) const;
		bool operator!=( const node& ) const;
		node* clone() const;
		node* intersect( const node* ) const;
		node* applySubstitution( size_t, type_t );
		void print( std::ostream& os ) const;
		~node();
	};
private:
	node* root;
	static int64_t free_type_counter;
	type_t( node* );
	node* peel();
	std::vector<node*>& getBaseParameters();
public:
	const node* getRoot() const;
	bool isFinal() const;
	bool operator==( const type_t& ) const;
	bool operator!=( const type_t& ) const;
	type_t intersect( const type_t& ) const;
	structure_t getBase() const;
	type_t getChildType() const;
	void applySubstitution( size_t, type_t );
	type_t rightFlattenTypeProduct( type_t left ) const;
	type_t& operator=( const type_t& );
	type_t( const type_t& );
	type_t( type_t&& );
	// type_t( structure_t, std::vector<const type_t*> );
	type_t( structure_t, std::vector<type_t> );
	type_t( int64_t f );
	type_t();
	~type_t();
};

type_t rightFlattenTypeProduct( type_t left, type_t right );

std::ostream& operator<<( std::ostream&, const type_t& );

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
	std::unordered_map<symbol_t,typedefinition> definitions;
public:
	bool addType( symbol_t, structure_t, std::vector<type_t> par = {}, std::vector<type_t> sub = {} );
	type_t getType( symbol_t, std::vector<type_t> pars = {} ) const;
};

// global data
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
	symbol_t getName( structure_t s ) const;
	type_t getType( structure_t, std::vector<type_t> pars ) const;
	structure_t addStructure( symbol_t, std::vector<type_t> param = {} );
	structureTable();
};

extern const type_t ERROR_TYPE;
extern const type_t VOID_TYPE;
extern const type_t INT_TYPE;
extern const type_t FLT_TYPE;
extern const type_t STR_TYPE;
extern const type_t TUP_TYPE;

extern structureTable* strtab; // only to be used for I/O