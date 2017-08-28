#include <algorithm>
#include "register_allocation.h"

bool startCompare( const live_interval_t& a, const live_interval_t& b ) {
	return a.begin < b.begin;
}

bool endCompare( const live_interval_t& a, const live_interval_t& b ) {
	return a.end < b.end;
}

bool registerAllocation::relative_sort::operator()( live_interval_index_t a, live_interval_index_t b ) const {
	return endCompare( liveness.at( a ), liveness.at( b ) );
}

registerAllocation::relative_sort::relative_sort( const std::vector<live_interval_t>& l ) : liveness( l ) {
}

register_t registerAllocation::getFreeRegister() const {
	register_t s = registers.size();
	for( register_t i = 0; i < s; ++i )
		if( registers.at(i) == ERROR_VARIABLE )
			return i;
	return ERROR_REGISTER;
}

void registerAllocation::linearScan() {
	live_interval_index_t s = liveness.size();
	for( live_interval_index_t i = 0; i < s; ++i ) {
		expireOldIntervals( i );
		register_t r = getFreeRegister();
		if( r == ERROR_REGISTER )
			spillAtInterval( i );
		else {
			liveness.at( i ).assigned_register = r;
			registers.at( r ) = liveness.at( i ).variable;
			active.insert( i );
		}
	}
	for( const auto& li : liveness )
		var_to_reg[li.variable] = li.assigned_register;
}

void registerAllocation::expireOldIntervals( live_interval_index_t i ) {
	while( active.size() > 0 and liveness.at( *active.begin() ).end < liveness.at( i ).begin ) {
		registers.at( liveness.at( *active.begin() ).assigned_register ) = ERROR_VARIABLE;
		active.erase( active.begin() );
	}
}

void registerAllocation::spillAtInterval( live_interval_index_t i ) {
	live_interval_index_t spill = *active.end();
	if( liveness.at( spill ).end > liveness.at( i ).end ) {
		liveness.at( i ).assigned_register = liveness.at( spill ).assigned_register;
		variables_to_stack.push_back( liveness.at( spill ).variable );
		active.erase( active.end() /*spill*/ );
		active.insert( i );
	} else
		variables_to_stack.push_back( liveness.at( i ).variable );
}

std::vector<live_interval_t> registerAllocation::getRegisterAssignment() const {
	return liveness;
}

const std::vector<variable_t>& registerAllocation::getStackVariables() const {
	return variables_to_stack;
}

register_t registerAllocation::getVariableRegister( variable_t v ) const {
	auto itr = var_to_reg.find( v );
	return itr == var_to_reg.end() ? ERROR_REGISTER : itr->second;
}

bool registerAllocation::isActive( variable_t v, size_t time ) const {
	for( const auto& li : liveness )
		if( li.variable == v )
			return li.begin <= time and time < li.end;
	return false;
}


std::vector<variable_t> registerAllocation::activeVariables( size_t time ) const {
	std::vector<variable_t> ret;
	for( const auto& li : liveness )
		if( li.begin <= time and time < li.end )
			ret.push_back( li.variable );
	return ret;
}

void registerAllocation::reset( size_t r, std::vector<live_interval_t> l ) {
	registers.clear();
	registers.resize( r, ERROR_VARIABLE );
	liveness = l;
	std::sort( liveness.begin(), liveness.end(), startCompare );
	variables_to_stack.clear();
	active.clear();
	var_to_reg.clear();
}


registerAllocation::registerAllocation( size_t r, std::vector<live_interval_t> l ) : active( liveness ) {
	reset( r, l );
}

registerAllocation::registerAllocation() : active( liveness ) {
}