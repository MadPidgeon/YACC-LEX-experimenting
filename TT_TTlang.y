/* C declarations */
%{

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <cmath>
#include <stack>
#include <cstring>
#include <fstream>
#include "symbol_table.h"
#include "scope_table.h"
#include "types.h"
#include "syntax_tree.h"
#include "debug.h"
#include "error_reporting.h"
#include "intermediate_code.h"
#include "flow_graph.h"
#include "register_allocation.h"
#include "assembly.h"

#undef DEBUG

/* Prototypes */
static void yyerror(const char *);
static void yyerror_w(const char *, int);


/* Import from comp.l */
#ifdef __cplusplus
extern "C" {
#endif

extern int yylex();
extern int yyparse();
extern FILE *yyin;


int error_counter = 0;
int warning_counter = 0;

#ifdef __cplusplus
}
#endif

symbolTable *symtab;
structureTable* strtab;
scopeTable *scptab;
syntaxTree *syntree;
std::string input_str;

scope_t current_scope = GLOBAL_SCOPE;
std::stack<type_t> decllisttypes;

syntaxTree::node* symbolToVariable( symbol_t id );
syntaxTree::node* symbolToVariable( symbol_t id, type_t type );

%}

/* Start symbol */
%start entry_point

/* Tokens */

%token INT FLT ID TYPENAME VTYPE ASSIGNMENT ADDOP MULOP COMMA SEMICOLON LSEQ RSEQ LPAR RPAR LBRA RBRA STRING_BEGIN STRING_END STRING_PARTICLE FUNC IF WHILE ELSE FOR IN RELOP ELLIPSIS

%union {
	char *str;
	int64_t num;
	double flt;
	const type_t* typ;
	std::vector<type_t>* typlst;
	struct syntaxTree::node *node;
}


/* shift/reduce conflict: dangling ELSE */
%expect 0
%expect-rr 4
%% 

/* GRAMMAR RULES AND ACTIONS */

entry_point:			statement_list;

statement_list: 		statement {
							$<node>$ = new syntaxTree::node( N_BLOCK_LIST, $<node>1 );
						} 
						| statement statement_list {
							$<node>$ = new syntaxTree::node( N_BLOCK_LIST, $<node>1, $<node>2 );
						};

statement:				multi_variable_decl SEMICOLON {
							$<node>$ = $<node>1;
						} 
						| assignment SEMICOLON {
							$<node>$ = $<node>1;
						} 
						| expression SEMICOLON {
							$<node>$ = $<node>1;
						}
						| sequential_block {
							$<node>$ = $<node>1;
						}
						| parallel_block
						| function_def
						| error SEMICOLON;

function_like:			id LBRA expression_list RBRA | id;

multi_variable_decl:	function_like declaration_list;

single_variable_decl:	function_like declaration;

declaration:			id | assignment;

declaration_list:		declaration | declaration COMMA declaration_list;

assignment:				id ASSIGNMENT expression; 

expression_list:		expression | expression COMMA expression_list;

expression:				term;

term:					factor | factor ADDOP term;

factor:					leaf | leaf MULOP factor;

leaf:					LBRA expression RBRA | function_like | constant;

function_def:			FUNC id LBRA function_def_list RBRA sequential_block;

function_def_list:		single_variable_decl | single_variable_decl COMMA function_def_list;

// agregates

sequential_block:		LSEQ RSEQ | LSEQ expression_list RSEQ | LSEQ statement_list RSEQ;

parallel_block:			LPAR RPAR | LPAR expression_list RPAR | LPAR statement_list RPAR;

// elementary / token-like

id:						ID {
							$<num>$ = symtab->addSymbol( $<str>1 );
							free( $<str>1 );
						};

constant:				INT {
							$<node>$ = new syntaxTree::node( N_INTEGER, nullptr, nullptr, {.integer=$<num>1} );
						}
						| FLT {
							$<node>$ = new syntaxTree::node( N_FLOAT, nullptr, nullptr, {.floating=$<flt>1} );
						}
						| STRING_BEGIN string_particles STRING_END {
							pmesg(90, "Lexer: STR: \"%s\"\n", input_str.c_str());
							syntaxTree::node::extra_data_t d;
							d.string = strdup(input_str.c_str());
							$<node>$ = new syntaxTree::node( N_STRING, nullptr, nullptr, d );
							input_str.clear();
						}
						| sequential_block {
							$<node>$ = $<node>1;
						}
						| parallel_block;

string_particles:		STRING_PARTICLE {
							input_str.append( $<str>1 );
							free( $<str>1 );
						} 
						| string_particles STRING_PARTICLE  {
							input_str.append( $<str>2 );
							free( $<str>2 );
						};

%%

/* C-CODE */

std::ostream& operator<<( std::ostream& os, const scopeTable& t ) {
	os << std::setfill(' ') << std::left << "SCOPE ID    NAME" << std::endl;
	for( size_t i = 0; i < t.scopes.size(); ++i ) {
		if( t.scopes.at(i).declarations.size() > 0 ) {
			os << std::setw( 6 ) << i;
			auto itr = t.scopes.at(i).declarations.begin();
			os << std::setw( 6 ) << itr->second << symtab->getName( itr->first ) << std::endl;
			for( ++itr; itr != t.scopes.at(i).declarations.end(); ++itr )
				os << "      " << std::setw( 6 ) << itr->second << symtab->getName( itr->first ) << std::endl;
		}
	}
	return os;
}

syntaxTree::node* symbolToVariable( symbol_t symbol ) {
	variable_t variable = scptab->getVariable( current_scope, symbol );
	return new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=variable} );
}

syntaxTree::node* symbolToVariable( symbol_t symbol, type_t type ) {
	variable_t variable = scptab->addVariable( current_scope, symbol, type );
	return new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=variable} );
}


int main( int argc, char** argv ) {
	char *input_file_name, *output_file_name;
	symtab = new symbolTable;
	strtab = new structureTable;
	scptab = new scopeTable( symtab, strtab );
	syntree = new syntaxTree( scptab );
	if( argc < 2 ) {
		printf("Missing input file!\n");
		return -1;
	}
	input_file_name = argv[1];
	FILE* input_file = fopen(input_file_name,"r");
	if( !input_file ) {
		printf("Couldn't open input file!\n");
		return -1;
	}
	yyin = input_file;
	int result = yyparse();
	std::cout << (*syntree) << std::endl;
	intermediateCode ic( scptab );
	function_t f = ic.addFunction( NONE_SYMBOL, syntree->getRoot() );
	std::cout << ic << std::endl;
	flowGraph G( ic.getFunction(f) );
	std::cout << G << std::endl;
	auto v = G.naiveLiveIntervals();
	for( auto i : v )
		std::cout << i << std::endl;
	registerAllocation ra( 8, v );
	ra.linearScan();
	for( auto i : ra.getRegisterAssignment() )
		std::cout <<  symtab->getName( scptab->getVariableSymbol( i.variable ) ) << ": " << i.assigned_register << std::endl;
	assemblyGenerator ag( ic );
	std::ofstream ofile( "a.out" );
	ofile << ag << std::endl;
	return 0;
}

static void yyerror_w(const char *s, int warning ) {
	if( !warning ) {
		error_counter += 1;
		lerr << error_line() << s << std::endl;
	} else if( warning_counter != -1 ) { // toggle warnings
		lerr << error_line(true) << s << std::endl;
		warning_counter++;
	}
}

static void yyerror(const char* s) {
	yyerror_w(s,0);
}

int yywrap() {
	return 1;
}