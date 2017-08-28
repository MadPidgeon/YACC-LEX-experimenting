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

scope_t previous_nested_scope = ERROR_SCOPE;
std::stack<scope_t> scopes;
std::stack<function_t> functions;
std::stack<type_t> decllisttypes;


syntaxTree::node* symbolToVariable( symbol_t id );
syntaxTree::node* symbolToVariable( symbol_t id, type_t type );
syntaxTree::node* handleRelop( int id, syntaxTree::node* a, syntaxTree::node* b );


%}

/* Start symbol */
%start entry_point

/* Tokens */

%token INT FLT ID TYPENAME VTYPE ASSIGNMENT ADDOP MULOP COMMA SEMICOLON LSEQ RSEQ LBRA RBRA STRING_BEGIN STRING_END STRING_PARTICLE FUNC IF WHILE ELSE FOR IN RELOP ELLIPSIS JOIN_MEET BREAK CONTINUE RETURN MAPSTO

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
%expect-rr 2
%glr-parser
%% 

/* GRAMMAR RULES AND ACTIONS */

entry_point:			statement_list {
							$<node>1->type = N_SEQUENTIAL_BLOCK;
							syntree->setRoot( $<node>1 );
						}

sequential_block:		lseq statement_list rseq {
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
						| break SEMICOLON {
							$<node>$ = $<node>1;
						}
						| continue SEMICOLON {
							$<node>$ = $<node>1;
						}
						| function_definition
						| return SEMICOLON {
							$<node>$ = $<node>1;
						}
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
						| id lbra typelist rbra {
							pmesg( 90, "Lexer: TYPENAME %s\n", symtab->getName( $<num>1 ).c_str() );
							$<typ>$ = new type_t( scptab->getTypeDefinition( scopes.top(), $<num>1, *$<typlst>3 ) );
							if( *$<typ>$ == ERROR_TYPE )
								lerr << error_line() << "Unknown type '" << symtab->getName( $<num>1 ).c_str() << "'" << std::endl;
						}
						| id {
							pmesg( 90, "Lexer: TYPENAME %s\n", symtab->getName( $<num>1 ).c_str() );
							$<typ>$ = new type_t( scptab->getTypeDefinition( scopes.top(), $<num>1 ) );
							if( *$<typ>$ == ERROR_TYPE )
								lerr << error_line() << "Unknown type '" << symtab->getName( $<num>1 ).c_str() << "'" << std::endl;
						}
						| lbra typelist rbra {
							if( $<typlst>2->size() == 1 )
								$<typ>$ = new type_t( $<typlst>2->back() );
							else {
								type_t t = TUP_TYPE;
								for( auto itr = $<typlst>2->rbegin(); itr != $<typlst>2->rend(); ++itr )
									t = t.rightFlattenTypeProduct( *itr );
								$<typ>$ = new type_t( t );
							}
						}

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
							$<node>$ = new syntaxTree::node( N_GARBAGE, nullptr, nullptr, {.integer=scptab->addVariable( scopes.top(), $<num>1, decllisttypes.top() ) } );
						};

assignment:				lvalue ASSIGNMENT expression {
							$<node>$ = new syntaxTree::node( N_ASSIGN, $<node>1, $<node>3 );	
						};

leaf_expression:		constant | variable {
							$<node>$ = $<node>1;
						} 
						| function_call | lbra expression rbra {
							$<node>$ = $<node>2;
						};

expression:				relational;

expression_list:		{ $<node>$ = nullptr; } | expression_list_ne;

expression_list_ne:		expression {
							$<node>$ = new syntaxTree::node( N_ARGUMENT_LIST, $<node>1 );
						} 
						| expression COMMA expression_list_ne {
							$<node>$ = new syntaxTree::node( N_ARGUMENT_LIST, $<node>1, $<node>3 );
						};

relational:				join_meet {
							$<node>$ = $<node>1;
						}
						| join_meet RELOP chain_relational {
							$<node>$ = new syntaxTree::node( N_COMPARISON_CHAIN, handleRelop( $<num>2, $<node>1, $<node>3 ) );
						};

chain_relational:		join_meet | join_meet RELOP chain_relational {
							$<node>$ = handleRelop( $<num>2, $<node>1, $<node>3 );
						}

join_meet:				sum
						| sum JOIN_MEET join_meet { // associativity needs to be fixed in the syntax tree or intermediate code
							node_t type;
							int c = $<num>2;
							if( c == 0 )
								type = N_JOIN;
							else
								type = N_MEET;
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
						}
						| tuple
						| inline_function;

tuple_list:				expression COMMA tuple_list_ne {
							$<node>$ = new syntaxTree::node( N_TUPLE, $<node>1, $<node>3 );
						};

tuple_list_ne:			expression {
							$<node>$ = new syntaxTree::node( N_TUPLE_LIST, $<node>1 );
						}
						| expression COMMA tuple_list_ne {
							$<node>$ = new syntaxTree::node( N_TUPLE_LIST, $<node>1, $<node>3 );
						};

tuple:					lbra tuple_list rbra {
							$<node>$ = $<node>2;
						}

list:					lseq list_items rseq {
							$<node>$ = new syntaxTree::node( N_LIST, $<node>2, nullptr, {.integer=$<node>2->data.integer+1} );
						}
						| lseq rseq {
							$<node>$ = new syntaxTree::node( N_LIST );
						};

list_items:				expression {
							$<node>$ = new syntaxTree::node( N_SINGLE_TYPE_EXPRESSION_LIST, $<node>1, nullptr, {.integer=0} );
						} 
						| expression COMMA list_items {
							type_t a = $<node>1->data_type, b = $<node>3->data_type;
							if( a != b and a != ERROR_TYPE and b != ERROR_TYPE )
								lerr << error_line() << "Lists and sets can only contain elements of a single type" << std::endl;
							$<node>$ = new syntaxTree::node( N_SINGLE_TYPE_EXPRESSION_LIST, $<node>1, $<node>3, {.integer=$<node>3->data.integer+1} );
						};

string_particles:		STRING_PARTICLE {
							input_str.append( $<str>1 );
							free( $<str>1 );
						} 
						| string_particles STRING_PARTICLE  {
							input_str.append( $<str>2 );
							free( $<str>2 );
						};

lvalue:					variable | function_call;

variable:				id {
							$<node>$ = symbolToVariable( $<num>1 );
						};

function_call:			id lbra expression_list rbra {
							/*std::cout << "function_call: " << ($<num>1) << " ";
							$<node>3->print(std::cout);
							std::cout << std::endl;*/
							function_t f = scptab->getFunction( scopes.top(), $<num>1, $<node>3->data_type );
							variable_t v = scptab->getVariable( scopes.top(), $<num>1 );
							if( f == ERROR_FUNCTION and v == ERROR_VARIABLE )
								lerr << error_line() << "Undeclared function " << symtab->getName( $<num>1 ) << std::endl;
							if( v == ERROR_VARIABLE or scptab->getVariableScope( v ) <= scptab->getFunctionScope( f ) ) { // function
								$<node>$ = new syntaxTree::node( N_FUNCTION_CALL, $<node>3, nullptr, {.integer = f } );
							} else { // variable
								type_t t = scptab->getVariableType( v );
								if( t.isList() ) {
									syntaxTree::node* n = $<node>3;
									if( n->children[1] )
										lerr << error_line(true) << "Expected 1 list index, others are ignored" << std::endl;
									$<node>$ = new syntaxTree::node( N_LIST_INDEXING, new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer = v } ), n->children[0] );
								} else if( t.isFunction() ) {
									std::cout << "Lexer: Function call by function pointer!\n";
									$<node>$ = new syntaxTree::node( N_FUNCTION_CALL, $<node>3, new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer = v} ) );
									std::cout << "kaas\n";
								} else	{
									lerr << error_line() << "Variable " << symtab->getName( $<num>1 ) << " is neither list nor function (but " << t << ")" << std::endl;
									$<node>$ = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
								}
							}
						};

function_definition:	FUNC id lbra parameter_list rbra sequential_block {
							lerr << "function_definition" << std::endl;
						}

inline_function:		lbra parameter_list rbra MAPSTO type LSEQ {
							scopes.push( previous_nested_scope );
							function_t f = scptab->addFunctionDeclaration( scopes.top(), NONE_SYMBOL, *$<typ>5, scptab->getAllVariables( scopes.top() ) );
							functions.push( f );
						} statement_list rseq {
							type_t t( FNC_STRUCTURE, { $<node>2->data_type, *$<typ>5 } );
							$<node>8->type = N_SEQUENTIAL_BLOCK;
							$<node>$ = new syntaxTree::node( N_FUNCTION_DEFINITION, $<node>2, $<node>8, {.integer = functions.top() } );
							functions.pop();
							$<node>$->data_type = t;
						}

declaration:			type id {
							variable_t v = scptab->addVariable( scopes.top(), $<num>2, *$<typ>1 ); // scoping issues
							$<node>$ = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=v} );
						};

parameter_list: 		declaration {
							$<node>$ = new syntaxTree::node( N_ARGUMENT_LIST, $<node>1 );
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

while:					WHILE lbra expression rbra statement {
							$<node>$ = new syntaxTree::node( N_WHILE, $<node>3, $<node>5 );
						}
						| WHILE lbra expression rbra statement ELSE statement {
							$<node>$ = new syntaxTree::node( N_WHILE, 
								$<node>3, 
								new syntaxTree::node( N_ELSE, $<node>5, $<node>7 )
							);
						};

if:						IF lbra expression rbra statement {
							$<node>$ = new syntaxTree::node( N_IF, $<node>3, $<node>5 );
						}
						| IF lbra expression rbra statement ELSE statement {
							$<node>$ = new syntaxTree::node( N_IF, $<node>3, new syntaxTree::node( N_ELSE, $<node>5, $<node>7 ) );
						};

break:					BREAK {
							$<node>$ = new syntaxTree::node( N_BREAK, nullptr, nullptr, {.integer=1} );
						}
						| BREAK INT {
							$<node>$ = new syntaxTree::node( N_BREAK, nullptr, nullptr, {.integer=$<num>2} );
						}

continue:				CONTINUE {
							$<node>$ = new syntaxTree::node( N_CONTINUE );
						}

return:					RETURN expression {
							$<node>$ = new syntaxTree::node( N_RETURN, $<node>2 );
						}

lbra:					LBRA {
							scopes.push( scptab->addScope( scopes.top() ) );
						}

rbra:					RBRA {
							previous_nested_scope = scopes.top();
							scopes.pop();
						}

lseq:					LSEQ {
							scopes.push( scptab->addScope( scopes.top() ) );
						}

rseq:					RSEQ {
							previous_nested_scope = scopes.top();
							scopes.pop();
						}

%%

/* C-CODE */


syntaxTree::node* symbolToVariable( symbol_t symbol ) {
	variable_t variable = scptab->getVariable( scopes.top(), symbol );
	return new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=variable} );
}

syntaxTree::node* symbolToVariable( symbol_t symbol, type_t type ) {
	variable_t variable = scptab->addVariable( scopes.top(), symbol, type );
	return new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=variable} );
}

syntaxTree::node* handleRelop( int id, syntaxTree::node* a, syntaxTree::node* b ) {
	assert( id >= 0 and id < 6 );
	return new syntaxTree::node( node_t( N_EQUALS + id ), a, b );
}


int main( int argc, char** argv ) {
	char *input_file_name, *output_file_name;
	symtab = new symbolTable;
	strtab = new structureTable;
	scptab = new scopeTable( symtab, strtab );
	syntree = new syntaxTree( scptab );
	scopes.push( GLOBAL_SCOPE );
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
	std::cout << "Syntax Tree:" << (*syntree) << std::endl << std::endl;
	intermediateCode ic( scptab );
	ic.defineFunction( GLOBAL_FUNCTION, syntree->getRoot() );
	std::cout << "Scope Table:" << std::endl << (*scptab) << std::endl;
	std::cout << "Intermediate Code:" << std::endl << ic << std::endl;
	assemblyGenerator ag( ic );
	ag.print( std::cout, false );
	std::ofstream ofile( "a.out" );
	std::cout << "Printing..." << std::endl;
	// std::cout << "Assembly:" << std::endl << ag << std::endl;
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