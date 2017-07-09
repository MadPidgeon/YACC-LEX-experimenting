bin/debug.o: debug.c
	g++ -c debug.c -o bin/debug.o -std=c++11

bin/symbol_table.o: symbol_table.cc
	g++ -c symbol_table.cc -o bin/symbol_table.o -std=c++11

bin/scope_table.o: scope_table.cc
	g++ -c scope_table.cc -o bin/scope_table.o -std=c++11

bin/syntax_tree.o: syntax_tree.cc
	g++ -c syntax_tree.cc -o bin/syntax_tree.o -std=c++11

bin/y.tab.o: y.tab.c
	g++ -c y.tab.c -o bin/y.tab.o -std=c++11

bin/lex.yy.o: lex.yy.c
	g++ -c lex.yy.c -o bin/lex.yy.o -std=c++11

y.tab.c y.tab.h: lang.y
	yacc -d lang.y

lex.yy.c lex.yy.h: lang.l y.tab.h
	lex lang.l

lexer: bin/lex.yy.o bin/y.tab.o bin/debug.o bin/symbol_table.o bin/syntax_tree.o bin/scope_table.o
	g++ bin/debug.o bin/y.tab.o bin/lex.yy.o bin/symbol_table.o bin/syntax_tree.o bin/scope_table.o -o lexer -std=c++11