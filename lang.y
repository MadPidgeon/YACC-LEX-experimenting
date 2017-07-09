/* C declarations */
%{

#include <stdio.h>
#include <math.h>
#include <stack>
#include "symbol_table.h"
#include "scope_table.h"
#include "types.h"
#include "syntax_tree.h"

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

symbolTable *symtab;
scopeTable *scptab;
scope_t current_scope = GLOBAL_SCOPE;
std::stack<type_t> decllisttypes;


int error_counter = 0;
int warning_counter = 0;

#ifdef __cplusplus
}
#endif

extern int lineno;        /* Current line number */

%}

/* Start symbol */
%start sequential_statements

/* Tokens */

%token INT FLT ID TYPENAME ASSIGNMENT ADDOP MULOP SEMICOLON

%union {
	char *str;
	int64_t num;
	double flt;
	struct syntaxTree::node *node;
}

/* shift/reduce conflict: dangling ELSE */
/* %expect 1 */
%% 

/* GRAMMAR RULES AND ACTIONS */

sequential_statements: 	statement | statement sequential_statements;
statement:				declarations SEMICOLON | assignment SEMICOLON | expression SEMICOLON | error SEMICOLON;
declarations:			TYPENAME {
							decllisttypes.push( $<num>1 );
						} declaration_list {
							decllisttypes.pop();
						};
declaration_list:		declaration_list ',' declaration_list_1 | declaration_list_1;
declaration_list_1:		declaration_list_2 {
							// symtab->setType( $<num>1, decllisttypes.top() );
						};
declaration_list_2:		assignment {
							$<num>$ = $<num>1;
						}
						| variable {
							$<num>$ = $<num>1;
						};

assignment:				variable ASSIGNMENT expression {
							$<node>$ = new syntaxTree::node( ASSIGN, $<node>1, $<node>3 );	
						};
leaf_expression:		constant | variable | function_call | '(' expression ')';
expression:				term {
							$<node>$ = $<node>1;	
						}
						| term ADDOP expression {
							node_t type;
							char c = $<str>2[0];
							if( c == '+' )
								type = ADD;
							else if( c == '-' )
								type = SUBTRACT;
							$<node>$ = new syntaxTree::node( type, $<node>1, $<node>3 );
						};
term:					leaf_expression;
constant:				INT {
							$<node>$ = new syntaxTree::node( INTEGER, nullptr, nullptr, {.integer=$<num>1} );
						}
						| FLT {
							$<node>$ = new syntaxTree::node( FLOAT, nullptr, nullptr, {.floating=$<flt>1} );
						};
variable:				ID {
							symbol_t symbol = symtab->addSymbol( $<str>1 );
							variable_t variable = scptab->getVariable( symbol, current_scope );
							$<node>$ = new syntaxTree::node( VARIABLE, nullptr, nullptr, {.integer=variable} );
							free( $<str>1 );
						};
function_call:			ID '(' parameter_list ')';
declaration:			TYPENAME variable {
							/*symtab->setType( $<num>2, $<num>1 );
							$<node>$ = new syntaxTree::node( VARIABLE, $<num>2 );*/
						}
parameter_list: 		declaration {
							$<node>$ = $<node>1;
						}
						| declaration ',' parameter_list {
							$<node>$ = new syntaxTree::node( ARGUMENT_LIST, $<node>1, $<node>3 );
						}


%%

/* C-CODE */

int main( int argc, char** argv ) {
	char *input_file_name, *output_file_name;
	symtab = new symbolTable;
	scptab = new scopeTable;
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
	auto P = yyparse();
}

static void yyerror_w(const char *s, int warning ) {
	if( !warning ) {
		error_counter += 1;
		fprintf(stderr, "line %d: Error: %s!\n", lineno, s);
	} else if( warning_counter != -1 ) { // toggle warnings
		fprintf(stderr, "line %d: Warning: %s!\n", lineno, s);
		warning_counter++;
	}
}

static void yyerror(const char* s) {
	yyerror_w(s,0);
}

int yywrap() {
	return 1;
}