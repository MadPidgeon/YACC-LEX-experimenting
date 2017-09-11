CXX = g++
CXXFLAGS = -Wall -std=c++11 -Wno-narrowing -Wfatal-errors
OBJ = debug.o symbol_table.o scope_table.o syntax_tree.o types.o error_reporting.o intermediate_code.o flow_graph.o register_allocation.o assembly.o stack_frame.o interface.o  y.tab.o lex.yy.o

.PHONY: install
install:
	if ! test -d bin; then mkdir bin; fi
	if ! test -d obj; then mkdir obj; fi
	if ! test -d asm; then mkdir asm; fi
	if ! test -d code; then mkdir code; fi
	if ! test -d tmp; then mkdir tmp; fi

y.tab.c y.tab.h: lang.y
	yacc -d lang.y

obj/y.tab.o: y.tab.c
	$(CXX) -c y.tab.c -o obj/y.tab.o $(CXXFLAGS)

lex.yy.c lex.yy.h: lang.l y.tab.h
	lex lang.l

obj/lex.yy.o: lex.yy.c 
	$(CXX) -c lex.yy.c -o obj/lex.yy.o $(CXXFLAGS)

obj/%.o: %.cc %.h
	$(CXX) -c $< -o $@ $(CXXFLAGS)

asm/%.asm: code/%.code lexer
	./lexer $< -o $@ -S -vsioa

tokenizer_test: tokenizer_test.cc obj/symbol_table.o obj/error_reporting.o obj/tokenizer.o
	$(CXX) $^ -o tokenizer_test $(CXXFLAGS)

parse_tree_test: parse_tree_test.cc obj/symbol_table.o obj/error_reporting.o obj/tokenizer.o obj/parse_tree.o
	$(CXX) $^ -o parse_tree_test $(CXXFLAGS)

lexer: $(addprefix obj/, $(OBJ))
	$(CXX) $^ -o lexer $(CXXFLAGS)

