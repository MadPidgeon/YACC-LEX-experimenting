#pragma once

typedef int64_t stack_offset_t;

class stackFrame {
	struct register_storage {
		register_t reg;
		stack_offset_t offset;
	};
	struct variable_storage {
		variable_t variable;
		stack_offset_t offset;
		size_t size;
	};
	stack_offset_t return_value;
	std::vector<variable_storage> function_arguments;
	stack_offset_t old_instruction_pointer;
	std::vector<register_storage> saved_registers;
	std::vector<variable_storage> saved_variables;

	size_t byte_count; 
public:
	size_t popCountOnReturn() const;
	stackFrame();
	stackFrame( type_t return_type, const std::vector<variable_t>& arguments_type, std::vector<bool> saved_registers, std::vector<variable_t> saved_variables );
};

stackFrame::stackFrame() {
	return_value = -8;
	old_instruction_pointer = 0;
	byte_count = 16;
}

stackFrame::stackFrame( type_t return_type, const std::vector<variable_t>& arguments_type, std::vector<bool> saved_registers, std::vector<variable_t> saved_variables ) {
	stack_offset_t o = old_instruction_pointer = 0;
	byte_count = 8;
	int count = arguments_type.size();
	size_t s;
	function_arguments.resize( count-- );
	for( auto itr = arguments_type.rbegin(); itr != arguments_type.rend(); ++itr ) {
		auto& a = function_arguments.at( count );
		a.variable = *itr;
		a.size = scptab->getType( a.variable ).rawSize();
		a.offset = ( o -= a.size );
		byte_count += a.size;
	}
	s = return_type.rawSize();
	return_value = o - s;
	byte_count += s;
	o = 0;
	register_t r = 0;
	for( bool b : saved_registers ) {
		if( b ) {
			saved_registers.push_back( { r, o += 8 } );
			byte_count += 8;
		}
		++r;
	}
	o += 8;
	for( variable_t v : saved_variables ) {
		size_t s = scptab->getType( v ).rawSize();
		saved_variables.push_back( { v, o, s } );
		o += s;
		byte_count += s;
	}
}

size_t stackFrame::popCountOnReturn() const {
	size_t s = 0;
	for( auto vs : function_arguments )
		s += vs.size;
	return s;
}