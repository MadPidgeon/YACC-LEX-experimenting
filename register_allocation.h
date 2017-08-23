#pragma once
#include <vector>
#include <set>
#include <unordered_map>
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
	std::unordered_map<variable_t,register_t> var_to_reg;
public:
	register_t getFreeRegister() const;
	void linearScan();
	void expireOldIntervals( live_interval_index_t i );
	void spillAtInterval( live_interval_index_t i );
	register_t getVariableRegister( variable_t v ) const;
	std::vector<live_interval_t> getRegisterAssignment() const;
	bool isActive( variable_t v, size_t time ) const; 
	std::vector<variable_t> activeVariables( size_t time ) const;
	const std::vector<variable_t>& getStackVariables() const;
	void reset( size_t r, std::vector<live_interval_t> l );
	registerAllocation( size_t r, std::vector<live_interval_t> l );
	registerAllocation();
};