#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include "intermediate_code.h"
#include "error_reporting.h"

typedef uint64_t basic_block_t;

#define NO_BASIC_BLOCK		0
#define ENTRY_BASIC_BLOCK	0	// cannot enter entry block

struct live_interval_t {
	uint64_t begin, end;
	variable_t variable;
	register_t assigned_register;
};

class flowGraph {
	friend std::ostream& operator<<( std::ostream&, const flowGraph& );
public:	
	struct constant_data {
		type_t type;
		union data_t {
			int64_t integer;
			double floating;
			char* string;
			constant_data* array; // change to map
		} data;
		bool operator==( const constant_data& other ) const;
		bool operator!=( const constant_data& other ) const;
		constant_data();
		constant_data( int64_t x );
		constant_data( double x );
	};
	struct node {
		basic_block_t id;
		basic_block_t children[2];
		size_t instruction_offset;
		label_t label;
		std::vector<basic_block_t> parents;
		std::vector<iop_t> operations;
		std::set<variable_t> def;
		std::set<variable_t> use;
		std::set<variable_t> in;
		std::set<variable_t> out;
		std::map<variable_t,std::set<variable_t>> equal_in;
		std::map<variable_t,std::set<variable_t>> equal_out;
		std::unordered_map<variable_t,constant_data> const_in;
		std::unordered_map<variable_t,constant_data> const_out;
		std::vector<live_interval_t> computeLiveness() const;
		bool constantPropagation();
		int cyclicEquivalence( flowGraph* );
	};
private:
	std::vector<node> basic_blocks;
	std::unordered_map<label_t,basic_block_t> labeled_blocks;
	const intermediateCode::function* original_function;
	void computeLiveness();
	void resolveCyclicEquivalence( node* n, int i, const std::set<variable_t>& eq, variable_t target );
public:
	void cyclicEquivalence();
	void constantPropagation();
	void removeWriteOnlyMemory();
	void clearNOP();
	void removeMoveIdempotent();
	void optimize();
	intermediateCode::function generateFunction() const;
	std::vector<live_interval_t> naiveLiveIntervals() const;
	flowGraph( const intermediateCode::function& f );
};

std::ostream& operator<<( std::ostream& os, const live_interval_t& i );
std::ostream& operator<<( std::ostream& os, const flowGraph& fg );