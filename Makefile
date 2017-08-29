CXX = g++
CXXFLAGS = -Wall -std=c++11 -Wno-narrowing -Wfatal-errors
OBJ = debug.o symbol_table.o scope_table.o syntax_tree.o types.o error_reporting.o intermediate_code.o flow_graph.o register_allocation.o assembly.o stack_frame.o interface.o  y.tab.o lex.yy.o

y.tab.c y.tab.h: lang.y
	yacc -d lang.y

bin/y.tab.o: y.tab.c
	$(CXX) -c y.tab.c -o bin/y.tab.o $(CXXFLAGS)

lex.yy.c lex.yy.h: lang.l y.tab.h
	lex lang.l

bin/lex.yy.o: lex.yy.c 
	$(CXX) -c lex.yy.c -o bin/lex.yy.o $(CXXFLAGS)

bin/%.o: %.cc %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

asm/%.asm: code/%.code lexer
	./lexer $< -o $@ -S -vls

lexer: $(addprefix bin/, $(OBJ))
	$(CXX) $^ -o lexer $(CXXFLAGS)