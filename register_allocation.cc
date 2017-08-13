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

void registerAllocation::reset( size_t r, std::vector<live_interval_t> l ) {
	registers.resize( r, ERROR_VARIABLE );
	liveness = l;
	std::sort( liveness.begin(), liveness.end(), startCompare );
	//assigned_registers.resize( liveness.size(), -1 );
}


registerAllocation::registerAllocation( size_t r, std::vector<live_interval_t> l ) : active( liveness ) {
	reset( r, l );
}

registerAllocation::registerAllocation() : active( liveness ) {
}