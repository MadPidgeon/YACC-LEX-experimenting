#pragma once
#include <vector>
#include <set>
#include "flow_graph.h"

#define ERROR_REGISTER -1

// typedef int register_t;

bool startCompare( const live_interval_t&, const live_interval_t& );
bool endCompare( const live_interval_t&, const live_interval_t& );

class registerAllocation {
	typedef size_t live_interval_index_t;
	std::vector<variable_t> registers;
	std::vector<live_interval_t> liveness;
	std::vector<variable_t> variables_to_stack;
	struct relative_sort {
		const std::vector<live_interval_t>& liveness;
		relative_sort( const std::vector<live_interval_t>& );
		bool operator()( live_interval_index_t, live_interval_index_t ) const;
	};
	std::set<live_interval_index_t,relative_sort> active;
public:
	register_t getFreeRegister() const;
	void linearScan();
	void expireOldIntervals( live_interval_index_t i );
	void spillAtInterval( live_interval_index_t i );
	std::vector<live_interval_t> getRegisterAssignment() const;
	void reset( size_t r, std::vector<live_interval_t> l );
	registerAllocation( size_t r, std::vector<live_interval_t> l );
	registerAllocation();
};