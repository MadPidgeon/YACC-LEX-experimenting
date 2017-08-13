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
	struct node {
		basic_block_t id;
		basic_block_t children[2];
		size_t instruction_offset;
		std::vector<iop_t> operations;
		std::set<variable_t> def;
		std::set<variable_t> use;
		std::set<variable_t> in;
		std::set<variable_t> out;
		std::vector<live_interval_t> computeLiveness() const;
	};
private:
	std::vector<node> basic_blocks;
	std::unordered_map<label_t,basic_block_t> labeled_blocks;
	void computeLiveness();
public:
	std::vector<live_interval_t> naiveLiveIntervals() const;
	flowGraph( const intermediateCode::function& f );
};

std::ostream& operator<<( std::ostream& os, const live_interval_t& i );
std::ostream& operator<<( std::ostream& os, const flowGraph& fg );