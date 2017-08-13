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

%token INT FLT ID TYPENAME VTYPE ASSIGNMENT ADDOP MULOP COMMA SEMICOLON LSEQ RSEQ LBRA RBRA STRING_BEGIN STRING_END STRING_PARTICLE FUNC IF WHILE ELSE FOR IN RELOP ELLIPSIS

%union {
	char *str;
	int64_t num;
	double flt;
	const type_t* typ;
	std::vector<type_t>* typlst;
	struct syntaxTree::node *node;
}


/* shift/reduce conflict: dangling ELSE */
%expect 2
%% 

/* GRAMMAR RULES AND ACTIONS */

entry_point:			statement_list {
							$<node>1->type = N_SEQUENTIAL_BLOCK;
							syntree->setRoot( $<node>1 );
						}
sequential_block:		LSEQ statement_list RSEQ {
							syntaxTree::node* n = $<node>2;
							n->setType( N_SEQUENTIAL_BLOCK );
							$<node>$ = n;
						}
statement_list: 		statement {
							$<node>$ = new syntaxTree::node( N_BLOCK_LIST, $<node>1 );
						} 
						| statement statement_list {
							$<node>$ = new syntaxTree::node( N_BLOCK_LIST, $<node>1, $<node>2 );
						}
statement:				declarations SEMICOLON {
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
						| if {
							$<node>$ = $<node>1;
						}
						| for {
							$<node>$ = $<node>1;
						}
						| while {
							$<node>$ = $<node>1;
						}
						| function_definition
						| error SEMICOLON;

id:						ID {
							$<num>$ = symtab->addSymbol( $<str>1 );
							free( $<str>1 );
						}

typelist:				type {
							$<typlst>$ = new std::vector<type_t>{ *$<typ>1 };
						}
						| type COMMA typelist {
							$<typlst>$ = $<typlst>3;
							$<typlst>$->insert( $<typlst>$->begin(), *$<typ>1 );
						};

type:					VTYPE id {
							pmesg( 90, "ERROR: variadic types not yet implemented\n" );
							$<typ>$ = &ERROR_TYPE;
						}
						| id LBRA typelist RBRA {
							pmesg( 90, "Lexer: TYPENAME %s\n", symtab->getName( $<num>1 ).c_str() );
							$<typ>$ = new type_t( scptab->getTypeDefinition( current_scope, $<num>1, *$<typlst>3 ) );
							if( *$<typ>$ == ERROR_TYPE )
								lerr << error_line() << "Unknown type '" << symtab->getName( $<num>1 ).c_str() << "'" << std::endl;
						}
						| id /*{
							pmesg( 90, "Lexer: TYPENAME %s\n", symtab->getName( $<num>1 ).c_str() );
							$<typ>$ = new type_t( scptab->getTypeDefinition( current_scope, $<num>1 ) );
							if( *$<typ>$ == ERROR_TYPE )
								lerr << error_line() << "Unknown type '" << symtab->getName( $<num>1 ).c_str() << "'" << std::endl;
						}*/

_dummy_set_type:		{

						};

declarations:			type {
							//pmesg( 90, "Declaring variables of type %d\n", $<num>1 );
							std::cout << "Declaring variables of type " << (*$<typ>1) << std::endl;
							decllisttypes.push( *$<typ>1  );
							delete $<typ>1;
						} declaration_list {
							syntaxTree::node* n = $<node>3; // <- 3
							if( n == nullptr )
								$<node>$ = new syntaxTree::node( N_EMPTY );
							else
								$<node>$ = n;
							decllisttypes.pop();
						};

declaration_list:		declaration_list_el COMMA declaration_list {
							if( $<node>1 == nullptr ) {
								$<node>$ = $<node>3;
							} else if( $<node>3 == nullptr ){
								$<node>$ = $<node>1;
							} else {
								$<node>$ = new syntaxTree::node( N_BLOCK_LIST, $<node>1, $<node>3 );
							}
						}
						| declaration_list_el {
							$<node>$ = $<node>1;
						};

declaration_list_el:	id ASSIGNMENT expression {
							$<node>$ = new syntaxTree::node( N_ASSIGN, symbolToVariable( $<num>1, decllisttypes.top() ), $<node>3 );
						}
						| id {
							variable_t variable = scptab->addVariable( current_scope, $<num>1, decllisttypes.top() );
							$<node>$ = nullptr;
						};

assignment:				variable ASSIGNMENT expression {
							$<node>$ = new syntaxTree::node( N_ASSIGN, $<node>1, $<node>3 );	
						};

leaf_expression:		constant | variable {
							$<node>$ = $<node>1;
						} 
						| function_call | LBRA expression RBRA {
							$<node>$ = $<node>2;
						};

expression:				relational;

relational:				sum {
							$<node>$ = $<node>1;
						}
						| sum RELOP relational {
							node_t type;
							int c = $<num>2;
							if( c == 1 )
								type = N_NOT_EQUALS;
							else if( c == 0 )
								type = N_EQUALS;
							$<node>$ = new syntaxTree::node( type, $<node>1, $<node>3 );
						};

sum:					product {
							$<node>$ = $<node>1;	
						}
						| product ADDOP sum {
							node_t type;
							int c = $<num>2;
							if( c == 1 )
								type = N_ADD;
							else if( c == 0 )
								type = N_SUBTRACT;
							$<node>$ = new syntaxTree::node( type, $<node>1, $<node>3 );
						};

product:				factor {
							$<node>$ = $<node>1;
						}
						| factor MULOP product {
							node_t type;
							int c = $<num>2;
							if( c == 1 )
								type = N_MULTIPLY;
							else if( c == 0 )
								type = N_DIVIDE;
							else
								type = N_REMAINDER;
							$<node>$ = new syntaxTree::node( type, $<node>1, $<node>3 );
						};

factor:					leaf_expression;

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
						| list {
							$<node>$ = $<node>1;
						};
list:					LSEQ list_items RSEQ {
							$<node>$ = new syntaxTree::node( N_LIST, $<node>2 );
						}
						| LSEQ RSEQ {
							$<node>$ = new syntaxTree::node( N_LIST );
						};

list_items:				expression {
							$<node>$ = new syntaxTree::node( N_SINGLE_TYPE_EXPRESSION_LIST, $<node>1 );
						} 
						| expression COMMA list_items {
							type_t a = $<node>1->data_type, b = $<node>3->data_type;
							if( a != b and a != ERROR_TYPE and b != ERROR_TYPE )
								lerr << error_line() << "Lists and sets can only contain elements of a single type" << std::endl;
							$<node>$ = new syntaxTree::node( N_SINGLE_TYPE_EXPRESSION_LIST, $<node>1, $<node>3 );
						};

string_particles:		STRING_PARTICLE {
							input_str.append( $<str>1 );
							free( $<str>1 );
						} 
						| string_particles STRING_PARTICLE  {
							input_str.append( $<str>2 );
							free( $<str>2 );
						};

variable:				id {
							$<node>$ = symbolToVariable( $<num>1 );
						};

function_call:			id LBRA statement_list RBRA {
							lerr << "function_call" << std::endl;
						};

function_definition:	FUNC id LBRA parameter_list RBRA sequential_block {
							lerr << "function_definition" << std::endl;
						}

declaration:			type variable {
							/*symtab->setType( $<num>2, $<num>1 );
							$<node>$ = new syntaxTree::node( VARIABLE, $<num>2 );*/
						};

parameter_list: 		declaration {
							$<node>$ = $<node>1;
						}
						| declaration COMMA parameter_list {
							$<node>$ = new syntaxTree::node( N_ARGUMENT_LIST, $<node>1, $<node>3 );
						};

for:					FOR LBRA id IN expression RBRA {
							structure_t base = $<node>5->data_type.getBase();
							if( base != LST_STRUCTURE and base != SET_STRUCTURE )
								lerr << error_line() << "Cannot iterate over variables of type " << $<node>5->data_type << std::endl;
							type_t t = $<node>5->data_type.getChildType();
							symbolToVariable( $<num>3, t );
						} statement {
							symbol_t s = symtab->addTemporarySymbol();
							$<node>$ = new syntaxTree::node( N_SEQUENTIAL_BLOCK, 
								new syntaxTree::node( N_ASSIGN, 
									symbolToVariable( s, $<node>5->data_type ),
									$<node>5 ),
								new syntaxTree::node( N_WHILE, new syntaxTree::node( N_EMPTY ) /*rip*/, $<node>8 ) );
						};

while:					WHILE LBRA expression RBRA statement {
							$<node>$ = new syntaxTree::node( N_WHILE, $<node>3, $<node>5 );
						}
						| WHILE LBRA expression RBRA statement ELSE statement {
							$<node>$ = new syntaxTree::node( N_WHILE, 
								$<node>3, 
								new syntaxTree::node( N_ELSE, $<node>5, $<node>7 )
							);
						};

if:						IF LBRA expression RBRA statement {
							$<node>$ = new syntaxTree::node( N_IF, $<node>3, $<node>5 );
						}
						| IF LBRA expression RBRA statement ELSE statement {
							$<node>$ = new syntaxTree::node( N_IF, $<node>3, new syntaxTree::node( N_ELSE, $<node>5, $<node>7 ) );
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