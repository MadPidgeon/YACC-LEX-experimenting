#pragma once
#include <string>
#include <vector>

struct command_line_data {
	std::vector<std::string> infilenames;
	std::string outfilename;
	enum output_format_t {
		ASSEMBLY, OBJECT, EXECUTABLE
	} output_format;
	bool parse( int argc, char** argv );
	command_line_data();
};