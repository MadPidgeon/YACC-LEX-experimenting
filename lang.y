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
#include "interface.h"

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


int warning_counter = 0;
int syntax_errors = 0;

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
std::stack<type_t> decllisttypes; // TODO: this might die due to error handling
std::stack<variable_t> for_iterating_variable;
bool is_func;

syntaxTree::node* symbolToVariable( symbol_t id );
syntaxTree::node* symbolToVariable( symbol_t id, type_t type );
syntaxTree::node* handleRelop( int id, syntaxTree::node* a, syntaxTree::node* b );


%}

/* Start symbol */
%start entry_point

/* Tokens */

%token INT FLT ID TYPENAME VTYPE ASSIGNMENT ADDOP MULOP COMMA SEMICOLON LSEQ RSEQ LBRA RBRA STRING_BEGIN STRING_END STRING_PARTICLE FUNC IF WHILE ELSE FOR IN RELOP ELLIPSIS JOIN_MEET BREAK CONTINUE RETURN MAPSTO SIZE_OF

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
						| error SEMICOLON {
							yyerrok;
							$<node>$ = new syntaxTree::node( N_EMPTY );
							syntax_errors--;
							lerr << error_line() << "Unexpected semicolon" << std::endl;
						}
						| assignment error {
							lerr << error_line() << "Missing semicolon in assignment" << std::endl;
							syntax_errors--;
							$<node>$ = $<node>1;
						};

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
							lerr << error_line() << "Variadic types not yet implemented" << std::endl;
							$<typ>$ = new type_t( ERROR_TYPE );
						}
						| id lbra typelist rbra {
							lexer_out << "TYPENAME " << symtab->getName( $<num>1 ) << std::endl;
							if( $<num>1 == TUP_SYMBOL ) {
								type_t t = TUP_TYPE;
								for( auto itr = $<typlst>3->rbegin(); itr != $<typlst>3->rend(); ++itr )
									t = t.rightFlattenTypeProduct( *itr );
								$<typ>$ = new type_t( t );
							} else {
								$<typ>$ = new type_t( scptab->getTypeDefinition( scopes.top(), $<num>1, *$<typlst>3 ) );
								if( *$<typ>$ == ERROR_TYPE )
									lerr << error_line() << "Unknown type '" << symtab->getName( $<num>1 ).c_str() << "'" << std::endl;
							}
						}
						| id {
							lexer_out << "TYPENAME " << symtab->getName( $<num>1 ) << std::endl;
							$<typ>$ = new type_t( scptab->getTypeDefinition( scopes.top(), $<num>1 ) );
							if( *$<typ>$ == ERROR_TYPE )
								lerr << error_line() << "Unknown type '" << symtab->getName( $<num>1 ).c_str() << "'" << std::endl;
						}
						| lbra typelist rbra { 
							lexer_out << "TUPLE" << std::endl;
							if( $<typlst>2->size() == 1 ) {
								lerr << error_line(true) << "Cannot construct a tuple of a single type using '(x)', use 'tup(x)' instead" << std::endl;
								$<typ>$ = new type_t( $<typlst>2->back() );
							} else {
								type_t t = TUP_TYPE;
								for( auto itr = $<typlst>2->rbegin(); itr != $<typlst>2->rend(); ++itr )
									t = t.rightFlattenTypeProduct( *itr );
								$<typ>$ = new type_t( t );
							}
						}
						/*| id lbra typelist error {
							lerr << "Expected right bracket at end of type definition" << std::endl;
							$<type>$ = new type_t( ERROR_TYPE );
						}*/
						/*| lbra rbra {
							$<typ>$ = new type_t( TUP_TYPE );
						}*/

declarations:			type {
							lexer_out << "Declaring variables of type " << (*$<typ>1) << std::endl;
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
							if( not decllisttypes.top().weaklyEqual( $<node>3->data_type ) )
								lerr << error_line() << "Mismatched types in assignment (" << decllisttypes.top() << " and " << $<node>3->data_type << ")" << std::endl;	
						}
						| id {
							$<node>$ = new syntaxTree::node( N_GARBAGE, nullptr, nullptr, {.integer=scptab->addVariable( scopes.top(), $<num>1, decllisttypes.top() ) } );
						};

assignment:				lvalue ASSIGNMENT expression {
							$<node>$ = new syntaxTree::node( N_ASSIGN, $<node>1, $<node>3 );
							if( not $<node>1->data_type.weaklyEqual( $<node>3->data_type ) )
								lerr << error_line() << "Mismatched types in assignment (" << $<node>1->data_type << " and " << $<node>3->data_type << ")" << std::endl;	
						};

leaf_expression:		constant | variable | function_call | size_of 
						| lbra expression rbra {
							$<node>$ = $<node>2;
						}
						| lbra expression error {
							lerr << error_line() << "Missing right parenthesis" << std::endl;
							syntax_errors--;
							$<node>$ = $<node>2;
						}
						| error expression rbra {
							lerr << error_line() << "Missing left parenthesis" << std::endl;
							syntax_errors--;
							$<node>$ = $<node>2;
						};

expression:				relational;

expression_list:		{ $<node>$ = nullptr; } | expression_list_ne;

expression_list_ne:		expression {
							$<node>$ = new syntaxTree::node( N_ARGUMENT_LIST, $<node>1 );
						} 
						| expression COMMA expression_list_ne {
							$<node>$ = new syntaxTree::node( N_ARGUMENT_LIST, $<node>1, $<node>3 );
						}
						| expression COMMA error {
							$<node>$ = $<node>1;
							lerr << error_line() << "Expected expression after comma" << std::endl;
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
							if( not $<node>1->data_type.weaklyEqual( $<node>3->data_type ) )
								lerr << error_line() << "Cannot join/meet types " << $<node>1->data_type << " and " << $<node>3->data_type << std::endl;
						};

sum:					product 
						| product ADDOP sum {
							node_t type;
							int c = $<num>2;
							if( c == 1 )
								type = N_ADD;
							else if( c == 0 )
								type = N_SUBTRACT;
							$<node>$ = new syntaxTree::node( type, $<node>1, $<node>3 );
						};

product:				factor
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
							lexer_out << "STR: \"" << input_str << "\"" << std::endl;
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

size_of:				SIZE_OF lbra expression rbra {
							$<node>$ = new syntaxTree::node( N_SIZE_OF, $<node>3 );
						}
						| SIZE_OF list {
							$<node>$ = new syntaxTree::node( N_SIZE_OF, $<node>2 );
						}

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
						| lbra rbra {
							$<node>$ = new syntaxTree::node( N_TUPLE ); // this might die
						}

list:					lseq list_items rseq {
							$<node>$ = new syntaxTree::node( N_LIST, $<node>2, nullptr, {.integer=$<node>2->data.integer+1} );
						}
						| lseq rseq {
							$<node>$ = new syntaxTree::node( N_LIST );
						}
						| lseq list_items error {
							lerr << error_line() << "Expected right square bracket at end of list" << std::endl;
							syntax_errors--;
							$<node>$ = new syntaxTree::node( N_LIST, $<node>2, nullptr, {.integer=$<node>2->data.integer+1} );
						}
						| error list_items rseq {
							lerr << error_line() << "Expected left square bracket at beginning of list" << std::endl;
							syntax_errors--;
							$<node>$ = new syntaxTree::node( N_LIST, $<node>2, nullptr, {.integer=$<node>2->data.integer+1} );
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
							lexer_out << "CALL " << symtab->getName($<num>1) << std::endl;
							/*std::cout << "function_call: " << ($<num>1) << " ";
							$<node>3->print(std::cout);
							std::cout << std::endl;*/
							syntaxTree::node* n = $<node>3;
							type_t t = n ? n->data_type : VOID_TYPE;
							function_t f = scptab->getFunction( scopes.top(), $<num>1, t );
							variable_t v = scptab->getVariable( scopes.top(), $<num>1 );
							if( f == ERROR_FUNCTION and v == ERROR_VARIABLE ) {
								if( $<num>1 == TUP_SYMBOL ) {
									syntaxTree::node* traverser = n;
									while( traverser != nullptr ) {
										traverser->type = N_TUPLE_LIST;
										traverser = traverser->children[1];
									}
									if( n == nullptr ) {	// empty tuple
										$<node>$ = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=ERROR_VARIABLE} );
										lerr << error_line() << "Empty tuple not yet supported" << std::endl;
										delete n;
									} else {
										$<node>3->type = N_TUPLE;
										$<node>$ = $<node>3;
									}
								} else {
									$<node>$ = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=ERROR_VARIABLE} );
									lerr << error_line() << "Undeclared function " << symtab->getName( $<num>1 ) << "(";
									bool comma = false;
									syntaxTree::node* traverser = n;
									while( traverser != nullptr ) {
										if( comma )
											lerr << ",";
										comma = true;
										lerr << traverser->children[0]->data_type;
										traverser = traverser->children[1];
									}
									lerr << ")" << std::endl;
									delete n;
								}
							} else if( v == ERROR_VARIABLE or scptab->getVariableScope( v ) <= scptab->getFunctionScope( f ) ) { // function
								$<node>$ = new syntaxTree::node( N_FUNCTION_CALL, $<node>3, nullptr, {.integer = f } );
							} else { // variable
								type_t t = scptab->getVariableType( v );
								if( t.isList() ) {
									if( n->children[1] )
										lerr << error_line(true) << "Expected 1 list index, others are ignored" << std::endl;
									$<node>$ = new syntaxTree::node( N_LIST_INDEXING, new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer = v } ), n->children[0] );
									n->children[0] = nullptr;
									delete n;
								} else if( t.isFunction() ) {
									$<node>$ = new syntaxTree::node( N_FUNCTION_CALL, $<node>3, new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer = v} ) );
								} else if( t.isTuple() ) {
									if( n->children[1] == nullptr /*single argument*/
											and n->children[0]->type == N_INTEGER /*constant argument*/ ) {
										$<node>$ = new syntaxTree::node( N_TUPLE_INDEXING, new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer = v } ), nullptr, {.integer = n->children[0]->data.integer } );
										delete n;
									} else {
										lerr << error_line() << "Tuples can only be indexed by constants" << std::endl;
									}									
								} else {
									lerr << error_line() << "Variable " << symtab->getName( $<num>1 ) << " is neither list nor function (but " << t << ")" << std::endl;
									$<node>$ = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
									delete n;
								}
							}
						}
						| function_call lbra expression_list rbra {
							type_t t = $<node>1->data_type;
							if( t.isFunction() ) {
								// check args plz
								$<node>$ = new syntaxTree::node( N_FUNCTION_CALL, $<node>3, $<node>1 );
							} else if( t.isList() ) {
								if( $<node>3->children[1] )
									lerr << error_line(true) << "Expected 1 list index, others are ignored" << std::endl;
								$<node>$ = new syntaxTree::node( N_LIST_INDEXING, $<node>1, $<node>3->children[0] );
							} else if( t.isTuple() ) {
								if( $<node>3->children[1] == nullptr /*single argument*/
										and $<node>3->children[0]->type == N_INTEGER /*constant argument*/ ) {
									$<node>$ = new syntaxTree::node( N_TUPLE_INDEXING, $<node>1, nullptr, {.integer = $<node>3->children[0]->data.integer } );
									delete $<node>3;
								} else {
									lerr << error_line() << "Tuples can only be indexed by constants" << std::endl;
									$<node>$ = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
								}
							} else if( t == ERROR_TYPE ) {
								delete $<node>3;
								$<node>$=$<node>1;
							} else {
								lerr << error_line() << "Variable is neither list nor function (but " << t << ")" << std::endl;
								$<node>$ = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
							}
						}
						| tuple lbra expression_list rbra {
							if( $<node>3->children[1] == nullptr /*single argument*/
									and $<node>3->children[0]->type == N_INTEGER /*constant argument*/ ) {
								$<node>$ = new syntaxTree::node( N_TUPLE_INDEXING, $<node>1, nullptr, {.integer = $<node>3->children[0]->data.integer } );
								delete $<node>3;
							} else {
								lerr << error_line() << "Tuples can only be indexed by constants" << std::endl;
								$<node>$ = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
							}
						}
						| list lbra expression_list rbra {
							if( $<node>3->children[1] )
								lerr << error_line(true) << "Expected 1 list index, others are ignored" << std::endl;
							$<node>$ = new syntaxTree::node( N_LIST_INDEXING, $<node>1, $<node>3->children[0] );
						}
						| lbra expression rbra lbra error {
							lerr << error_line() << "Cannot construct 1-element tuples with '(x)', use 'tup(x)' instead" << std::endl;
							syntax_errors--;
							$<node>$ = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
						} 
						/*| id lbra expression_list error {
							lerr << error_line() << "Missing right parenthesis in function call" << std::endl;
							syntax_errors--;
							$<node>$ = new syntaxTree::node( N_FUNCTION_CALL, $<node>3, new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer = ERROR_VARIABLE } ) );
						};*/

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

for:					FOR lbra id IN expression rbra {
							type_t t = $<node>5->data_type;
							if( not( t.isList() or t.isSet() ) )
								lerr << error_line() << "Cannot iterate over instance of type " << $<node>5->data_type << std::endl;
							for_iterating_variable.push( scptab->addVariable( scopes.top(), $<num>3, t.getChildType() ) );
						} statement {
							$<node>$ = new syntaxTree::node( N_FOR, 
								new syntaxTree::node( N_IN, 
									new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=for_iterating_variable.top() } ),
									$<node>5 ),
								$<node>8 );
							for_iterating_variable.pop();
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
							scopes.push( scptab->addScope( scopes.top() ) ); // do such brackets realy need to be a scope?
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
	command_line_data settings;
	if( not settings.parse( argc, argv ) )
		return -1;
	FILE* input_file = fopen( settings.infilenames[0].c_str(), "r" );
	if( !input_file ) {
		printf("Couldn't open input file!\n");
		return -1;
	}
	yyin = input_file;
	lexer_out << "Tokens:" << std::endl;
	int result = yyparse();
	if( syntax_errors != 0 ) {
		std::cout << error_line() << "Unresolved syntax errors" << std::endl;
		return -1;
	}
	if( err_count > 0 ) {
		return -1;
	}
	syntree_out << "\nSyntax Tree:" << std::flush;
	syntree_out << (*syntree) << std::endl << std::endl;
	if( settings.output_format == command_line_data::output_format_t::PARSE )
		return 0;
	intermediateCode ic( scptab );
	ic.defineFunction( GLOBAL_FUNCTION, syntree->getRoot() );
	syntree_out << "Scope Table:" << std::endl << (*scptab) << std::endl;
	ic_out << "Intermediate Code:" << std::endl << ic << std::endl;
	assemblyGenerator ag( ic, settings.optimization_level );
	if( asm_out.enabled )
		ag.print( asm_out.stream, false );
	std::string asmofn = current_directory + "tmp/out.asm";
	if( settings.output_format == command_line_data::output_format_t::ASSEMBLY )
		asmofn = settings.outfilename;
	{
		std::ofstream ofile( asmofn );
		ofile << ag << std::endl;
	}
	if( settings.output_format == command_line_data::output_format_t::ASSEMBLY )
		return 0;
	std::string objofn = current_directory + "tmp/out.o";
	if( settings.output_format == command_line_data::output_format_t::OBJECT )
		objofn = settings.outfilename;
	int asm_ret = system( ( "nasm -f elf64 " + asmofn + " -o " + objofn ).c_str() );
	if( asm_ret != 0 ) {
		std::cout << ERROR_FORMATTED_STRING << "Generated assembly won't compile (this is a bug in the compiler)" << std::endl;
		return asm_ret;
	}
	if( settings.output_format == command_line_data::output_format_t::OBJECT )
		return 0;
	int lnk_ret = system( ( "ld -o " + settings.outfilename + " " + objofn ).c_str() );
	if( lnk_ret != 0 ) {
		std::cout << ERROR_FORMATTED_STRING << "Generated assembly won't link (this is a bug in the compiler)" << std::endl;
		return lnk_ret;
	}
	return 0;
}

static void yyerror_w(const char *s, int warning ) {
	/*if( !warning ) {
		error_counter += 1;
		lerr << error_line() << s << std::endl;
	} else if( warning_counter != -1 ) {
		lerr << error_line(true) << s << std::endl;
		warning_counter++;
	}*/
}

static void yyerror(const char* s) {
	// yyerror_w(s,0);
	syntax_errors++;
}

int yywrap() {
	return 1;
}