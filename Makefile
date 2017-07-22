CXX=g++
CXXFLAGS=-Wall -std=c++11 -Wno-narrowing

#bin/$.o: $.cc
#	$(CXX) -c $< -o $@ $(CXXFLAGS)

bin/debug.o: debug.c
	$(CXX) -c debug.c -o bin/debug.o $(CXXFLAGS)

bin/symbol_table.o: symbol_table.cc
	$(CXX) -c symbol_table.cc -o bin/symbol_table.o $(CXXFLAGS)

bin/scope_table.o: scope_table.cc
	$(CXX) -c scope_table.cc -o bin/scope_table.o $(CXXFLAGS)

bin/syntax_tree.o: syntax_tree.cc
	$(CXX) -c syntax_tree.cc -o bin/syntax_tree.o $(CXXFLAGS)

bin/types.o: types.cc
	$(CXX) -c types.cc -o bin/types.o $(CXXFLAGS)

bin/error_reporting.o: error_reporting.cc
	$(CXX) -c error_reporting.cc -o bin/error_reporting.o $(CXXFLAGS)

bin/y.tab.o: y.tab.c
	$(CXX) -c y.tab.c -o bin/y.tab.o $(CXXFLAGS)

bin/lex.yy.o: lex.yy.c
	$(CXX) -c lex.yy.c -o bin/lex.yy.o $(CXXFLAGS)

y.tab.c y.tab.h: lang.y
	yacc -d lang.y

lex.yy.c lex.yy.h: lang.l y.tab.h
	lex lang.l

lexer: bin/lex.yy.o bin/y.tab.o bin/debug.o bin/symbol_table.o bin/syntax_tree.o bin/scope_table.o bin/types.o bin/error_reporting.o
	$(CXX) bin/debug.o bin/y.tab.o bin/lex.yy.o bin/symbol_table.o bin/syntax_tree.o bin/scope_table.o bin/types.o bin/error_reporting.o -o lexer $(CXXFLAGS)