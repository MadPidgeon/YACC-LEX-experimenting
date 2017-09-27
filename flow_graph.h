#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include "intermediate_code.h"
#include "error_reporting.h"

#define OPTIMIZATION_DEBUG

typedef uint64_t basic_block_t;

#define NO_BASIC_BLOCK		0
#define ENTRY_BASIC_BLOCK	0	// cannot enter entry block

struct live_interval_t {
	uint64_t begin, end;
	variable_t variable;
	register_t assigned_register;
};

struct offset_variable_t {
	variable_t variable;
	int64_t offset; // in qwords
	offset_variable_t( variable_t, int64_t = -16 );
	bool operator<( offset_variable_t ) const;
	bool operator==( offset_variable_t ) const;
	static offset_variable_t highest_lower_bound( variable_t );
	static offset_variable_t lowest_upper_bound( variable_t );
};

class flowGraph {
	friend std::ostream& operator<<( std::ostream&, const flowGraph& );
public:	
	struct constant_data {
		type_t type;
		union data_t {
			int64_t integer;
			double floating;
			const char* string;
			constant_data* array; // change to map
		} data;
		bool operator==( const constant_data& other ) const;
		bool operator!=( const constant_data& other ) const;
		constant_data();
		constant_data( int64_t x );
		constant_data( double x );
		constant_data( const char* x );
	};
	struct node {
		basic_block_t id;
		basic_block_t direct_child;
		basic_block_t jump_child;
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
		std::map<offset_variable_t,constant_data> const_in;
		std::map<offset_variable_t,constant_data> const_out;
		std::vector<live_interval_t> computeLiveness() const;
		bool constantPropagation();
		int cyclicEquivalence( flowGraph* );
		void localCyclicEquivalence();
		void setJumpsToChildren( const flowGraph* );
		void upgradeJump();
	};
private:
	std::vector<node> basic_blocks;
	std::unordered_map<label_t,basic_block_t> labeled_blocks;
	const intermediateCode::function* original_function;
	void print( std::ostream& os, bool info = true ) const;
	void resolveCyclicEquivalence( node* n, int i, const std::set<variable_t>& eq, variable_t target );
	void walkGraph( basic_block_t i, std::set<basic_block_t>& visited );
public:
	void removeDeadCode();
	void computeLiveness();
	void cyclicEquivalence();
	void constantPropagation();
	void removeWriteOnlyMemory();
	void clearNOP();
	void removeMoveIdempotent();
	void removeUselessBooleans();
	void expandBlocks();
	void contractBlocks();
	void flipConstantCompares();
	const node& getBlock( basic_block_t ) const;
	void splitBranch( basic_block_t block, basic_block_t parent );
	std::vector<std::vector<iop_t>> generateCodeByBlock() const;
	void optimize( int );
	intermediateCode::function generateFunction() const;
	std::vector<live_interval_t> naiveLiveIntervals() const;
	flowGraph( const intermediateCode::function& f );
};

std::ostream& operator<<( std::ostream& os, const live_interval_t& i );
std::ostream& operator<<( std::ostream& os, const flowGraph& fg );