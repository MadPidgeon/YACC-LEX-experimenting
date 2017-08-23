#include "stack_frame.h"

stackFrame::variable_storage::variable_storage( variable_t v, size_t s, stack_offset_t o ) {
	variable = v;
	size = s;
	offset = o;
}

stackFrame::stackFrame() : 
		return_address( ERROR_VARIABLE, 8, -8 ),
		old_bp( ERROR_VARIABLE, 8, 0 ), 
		byte_count( 8 ) {
}

stackFrame::stackFrame( std::vector<variable_storage> returns, std::vector<variable_storage> arguments ) {
	reset( returns, arguments );
}

void stackFrame::reset( std::vector<variable_storage> returns, std::vector<variable_storage> arguments ) {
	return_values = std::move( returns );
	function_arguments = std::move( arguments );
	return_address = variable_storage( ERROR_VARIABLE, 8, 8 );
	old_bp = variable_storage( ERROR_VARIABLE, 8, 0 );
	stack_offset_t off = 8; 
	size_t index = function_arguments.size()-1;
	for( auto itr = function_arguments.rbegin(); itr != function_arguments.rend(); ++itr ) {
		off += itr->size;
		itr->offset = off;
		variable_location[itr->variable] = std::make_pair( 1, index-- );
	}
	index = return_values.size()-1;
	for( auto itr = return_values.rbegin(); itr != return_values.rend(); ++itr ) {
		off += itr->size;
		itr->offset = off;
		variable_location[itr->variable] = std::make_pair( 2, index-- );
	}
	byte_count = off;
}

stack_offset_t stackFrame::addVariable( variable_t v, size_t s ) {
	if( variable_location.count(v) == 1 )
		return getVariableLocation( v );
	byte_count += s;
	stack_offset_t o = saved_variables.size() == 0 ? -8 : (saved_variables.back().offset-saved_variables.back().size);
	saved_variables.emplace_back( v, s, o );
	variable_location[v] = std::make_pair( 0, saved_variables.size()-1 );
	return saved_variables.back().offset;
}

stack_offset_t stackFrame::getVariableLocation( variable_t v ) const {
	auto p = variable_location.at( v );
	if( p.first == 0 )
		return saved_variables.at( p.second ).offset;
	else if( p.first == 1 )
		return function_arguments.at( p.second ).offset;
	else if( p.first == 2 )
		return return_values.at( p.second ).offset;
	throw;
}

bool stackFrame::isLocalVariable( variable_t v ) const {
	return variable_location.at( v ).first == 0;
}

size_t stackFrame::popCountOnReturn() const {
	size_t s = 0;
	for( auto vs : function_arguments )
		s += vs.size;
	return s;
}

size_t stackFrame::localVariableSize() const {
	size_t s = 0;
	for( auto vs : saved_variables )
		s += vs.size;
	return s;
}

void stackFrame::print( std::ostream& os ) const {
	os << "------+------------------" << std::endl;
	for( const auto& vs : return_values ) {
		std::string o = std::to_string( vs.offset );
		o.resize( 6, ' ' );
		os << o << "| " << symtab->getName( scptab->getVariableSymbol( vs.variable ) ) << std::endl;
	}
	os << "------+------------------" << std::endl;
	for( const auto& vs : function_arguments ) {
		std::string o = std::to_string( vs.offset );
		o.resize( 6, ' ' );
		os << o << "| " << symtab->getName( scptab->getVariableSymbol( vs.variable ) ) << std::endl;
	}
	os << "------+------------------" << std::endl;
	os << "+8    | Return address   " << std::endl;
	os << "0     | Base pointer     " << std::endl;
	os << "------+------------------" << std::endl;
	for( const auto& vs : saved_variables ) {
		std::string o = std::to_string( vs.offset );
		o.resize( 6, ' ' );
		os << o << "| ";
		o = std::to_string( vs.variable );
		o.resize( 3, ' ' );
		os << "(" << o << ") " << symtab->getName( scptab->getVariableSymbol( vs.variable ) ) << std::endl;
	}
	os << "------+------------------" << std::endl;
}

std::ostream& operator<<( std::ostream& os, const stackFrame& sf ) {
	sf.print( os );
	return os;
}