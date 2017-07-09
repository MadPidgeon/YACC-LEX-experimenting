/* C declarations */
%{

#include <stdio.h>
#include <math.h>
#include <cstring>
#include <sstream>
#include "debug.h"
#include "SyntaxTree.h"
#include "SymbolTable.h"
#include "Node.h"

#undef DEBUG

/* Prototypes */
static void yyerror(const char *, bool warning = false );


/* Import from comp.l */
#ifdef __cplusplus
extern "C" {
#endif

int yylex(void);   /* Lexer function */

#ifdef __cplusplus
}
#endif

extern int lineno;        /* Current line number */


/* Other declarations */
SyntaxTree * tree;        /* The syntax tree */
SymbolTable * symtab;     /* The symbol table */
string mainScopeName;     /* The name of the main scope */
bool do_arguments = false;
bool err_var = false;
int errors = 0, warnings = 0;
Symbol mainSymbol;
bool mainSymbolDefined = false;

Symbol* newSymbol( string name, __returntype r, __symboltype s ) {
  Symbol* symbol = new Symbol();
  symbol->SetName( name );
  symbol->SetReturnType( r );
  symbol->SetSymbolType( s );
  symbol->SetLine( lineno );
  return symbol;
}

template<typename T> string ToString( T x ) {
  ostringstream ss;
  ss << x;
  return ss.str();
}

__returntype coerce( Node*& a, Node*& b ) {
  if( a->GetReturnType() == RT_ERROR || b->GetReturnType() == RT_ERROR ) 
    return RT_ERROR;
  if( a->GetReturnType() == b->GetReturnType() )
    return a->GetReturnType();
  else {
    if( a->GetReturnType() == RT_INT )
      a = tree->CreateParentNode( NODE_COERCION, RT_REAL, a );
    else
      b = tree->CreateParentNode( NODE_COERCION, RT_REAL, b );
    yyerror( "Coercion", true );
    return RT_REAL;
  }
}

__returntype coerce( Node*& a, __returntype r ) {
  if( r == a->GetReturnType() )
    return r;
  else if( r == RT_REAL && a->GetReturnType() == RT_INT ) {
    yyerror( "Coercion", true );
    a = tree->CreateParentNode( NODE_COERCION, RT_REAL, a );
    return r;
  } else {
    if( a->GetReturnType() != RT_ERROR && r != RT_ERROR && a->GetReturnType() != RT_VOID )
      yyerror( ( "Can't coerce " + string( ReturnTypeToString( a->GetReturnType() ) ) + " to " + string( ReturnTypeToString(r) ) ).data() );
    return RT_ERROR;
  }
}

Symbol* GetDecentSymbol( string name ) {
  if( symtab->GetCurrentScopeName() == name && symtab->GetCurrentScopeName() == mainScopeName ) {
    if( !mainSymbolDefined )
      yyerror( "Program name is not a legal identifier" );
    mainSymbolDefined = true;
    return &mainSymbol;
  } else
    return symtab->GetSymbol( name );
}

Symbol* GetDecentSymbol( string scope, string name, bool par ) {
  if( symtab->GetCurrentScopeName() == name && symtab->GetCurrentScopeName() == mainScopeName ) {
    if( !mainSymbolDefined )
      yyerror( "Program name is not a legal identifier" );
    mainSymbolDefined = true;
    return &mainSymbol;
  }
  else
    return symtab->GetSymbol( scope, name, par );
}

void checkParameters( string name, Node*& expr_list ) {
  Scope* sub = symtab->GetScope( name );
  size_t num_parameters = 0;
  for( ; num_parameters < sub->GetNumberOfSymbols() && sub->GetSymbol(num_parameters)->GetSymbolType() == ST_PARAMETER; num_parameters++ );
  if( expr_list == NULL ) {
    if( num_parameters > 0 ) 
      yyerror( "Expected parameters" );
    return;
  }
  size_t num_expressions = 1;
  Node* tmp = expr_list;
  for(  ; tmp->GetNodeType() == NODE_EXPRLIST ; tmp = tmp->GetRightChild(), num_expressions++ );
  if( num_expressions < num_parameters ) {
    yyerror( ("To few parameters; Expected: " + ToString( num_parameters ) + ", Received: " + ToString( num_expressions )).data() );
    return;
  } else if( num_expressions > num_parameters ) {
    yyerror( ("To many parameters; Expected: " + ToString( num_parameters ) + ", Received: " + ToString( num_expressions )).data() );
    return;
  }

  if( num_expressions == 1 ) {
    coerce( expr_list, sub->GetSymbol(0)->GetReturnType() );
  } else {
    Node* parent = expr_list;
    Node* pparent = NULL;
    Node* temp;
    size_t i = 0;
    for( ; i < num_parameters-1; i++ ) {
      temp = parent->GetLeftChild();
      coerce( temp, sub->GetSymbol(i)->GetReturnType() );
      parent->SetLeftChild( temp );
      pparent = parent;
      parent = parent->GetRightChild();
    }
    temp = pparent->GetRightChild();
    coerce( temp, sub->GetSymbol(i)->GetReturnType() );
    pparent->SetRightChild( temp );
  }

}


#ifdef DEBUG
int msglevel = 90; /* higher = more debugging messages */
#else
int msglevel = 0;   /* don't bother sensitive souls */
#endif

struct Stock {
  __returntype type;
  char* name;
  Stock* next;
  Stock( __returntype t, char* na, Stock* ne ) : type(t), name(na), next(ne) {}
  ~Stock() { free(name); }
};

%}

/* Start symbol */
%start program

/* Tokens */
%token PROGRAM ID VAR NUM INTEGER REAL FUNCTION PROCEDURE 
%token PASCALBEGIN /* BEGIN is used internally by flex :-( */
%token END ADDOP ASSIGNOP IF THEN ELSE WHILE DO RELOP
%token MULOP NOT PROG_PARAMETER IRRELEVANT
%token UMINUS /* unary minus */

/* Types to pass between lexer, rules and actions */
%union {
  char *idStr;
  char *numStr;
  Node *node;
  __returntype returntype;
  struct Stock* stock;
}

%expect 1  /* shift/reduce conflict: dangling ELSE */
%% 

/* GRAMMAR RULES AND ACTIONS */

program:        PROGRAM ID {
                  /* The program name has just been read; save it */
                  mainScopeName = $<idStr>2;

                  /* Free the chars allocated by the lexer */
                  free($<idStr>2);
                  
                  /* Put the program name into the syntax tree */
                  tree->SetProgramName (mainScopeName);
                  
                  /* Create a new scope in the symbol table */
                  symtab->AddScope (mainScopeName);

                  symtab->AddSymbol( mainScopeName, newSymbol( "readinteger", RT_INT, ST_FUNCTION ) );
                  symtab->AddScope( "readinteger" );

                  symtab->AddSymbol( mainScopeName,newSymbol( "readreal", RT_REAL, ST_FUNCTION ) );
                  symtab->AddScope( "readreal" );

                  symtab->AddSymbol( mainScopeName,newSymbol( "writeinteger", RT_VOID, ST_PROCEDURE ) );
                  symtab->AddScope( "writeinteger" );
                  symtab->AddSymbol( newSymbol( "i", RT_INT, ST_PARAMETER ) );

                  symtab->AddSymbol( mainScopeName,newSymbol( "writereal", RT_VOID, ST_PROCEDURE ) );
                  symtab->AddScope( "writereal" );
                  symtab->AddSymbol( newSymbol( "i", RT_REAL, ST_PARAMETER ) );

                  symtab->SetCurrentScope( mainScopeName );
                  Symbol* temp = newSymbol( mainScopeName, RT_ERROR, ST_FUNCTION ); 
                  mainSymbol = *temp;
                  delete temp;

                }
                ';' declarations subprogram_declarations 
                compound_statement '.' {
                  tree->SetProgramBody( $<node>7 ); 
                }
                ;


type_identifier_list:
                ',' ID type_identifier_list {
                  Stock* n = $<stock>3;
                  Stock* x = new Stock( n->type, $<idStr>2, n );
                  $<stock>$ = x;
                }

                | ':' type {
                  $<stock>$ = new Stock( $<returntype>2, NULL, NULL );
                }
                ; 


declarations:   /* In declarations the VAR-variables are listed for the 
                   program as well as for the procedures and functions. */

                declarations VAR ID { do_arguments = false; } 
                
                type_identifier_list ';' {
                  Stock* temp = new Stock( $<stock>5->type, $<idStr>3, $<stock>5 );
                  Stock* h = temp;
                  while( temp->next != NULL ) { 
                    Symbol* symbol = GetDecentSymbol( symtab->GetCurrentScopeName(), temp->name, false );
                    if( symtab->GetCurrentScopeName() == string( temp->name )  ) {
                      yyerror( "Redeclaration" );
                      temp->type = RT_ERROR;
                    }
                    else if( symbol == NULL ) {
                      symbol = newSymbol( temp->name, temp->type, ST_VARIABLE );
                      symtab->AddSymbol( symbol );
                      symbol = GetDecentSymbol( symtab->GetCurrentScopeName(), temp->name, false );
                    } else {
                      if( symbol->GetReturnType() != RT_ERROR ){
                        yyerror( "Redeclaration" );
                        symbol->SetReturnType( RT_ERROR );
                      }
                    }
                    temp = temp->next;
                  }
                  while( h ) {
                    temp = h;
                    h = h->next;
                    delete temp;
                  }
                }

                | /* lambda */
                ;


type:           standard_type {
                  $<returntype>$ = $<returntype>1;
                }

                ;


standard_type:  INTEGER {
                  $<returntype>$ = RT_INT;
                }

                | REAL {
                  $<returntype>$ = RT_REAL;
                }
                ; 


subprogram_declarations:
                subprogram_declarations subprogram_declaration ';' { symtab->SetCurrentScope( mainScopeName ); }

                | /* lambda */
                ;


subprogram_declaration: 
                subprogram_head declarations compound_statement {
                  tree->AddSubprogram( string( $<idStr>1 ), $<node>3 );
                  free($<idStr>1);
                }
                ;


subprogram_head: 
                FUNCTION ID {
                  if( symtab->GetScope( $<idStr>2 ) == NULL ) 
                    symtab->AddScope( $<idStr>2 );
                  else
                    symtab->SetCurrentScope( $<idStr>2 );
                } arguments ':' standard_type ';' {
                  Symbol* symbol = GetDecentSymbol( $<idStr>2 );
                  if( symbol == NULL ) {
                    symbol = newSymbol( $<idStr>2, $<returntype>6, ST_FUNCTION );
                    symtab->AddSymbol( mainScopeName, symbol );
                  } else {
                    if( symbol->GetReturnType() != RT_ERROR ){
                      if( symbol->GetSymbolType() == ST_PARAMETER )
                        yyerror( "Parameter has same name as function" );
                      else
                        yyerror( "Function name already declared" );
                      symbol->SetReturnType( RT_ERROR );
                    }
                  }
                  $<idStr>$ = $<idStr>2; 
                }

                | PROCEDURE ID {
                  if( symtab->GetScope( $<idStr>2 ) == NULL )
                    symtab->AddScope( $<idStr>2 );
                  else
                    symtab->SetCurrentScope( $<idStr>2 );
                  Symbol* symbol = GetDecentSymbol( $<idStr>2 );
                  if( symbol == NULL ) {
                    symbol = newSymbol( $<idStr>2, RT_VOID, ST_PROCEDURE );
                    symtab->AddSymbol( mainScopeName, symbol );
                  } else {
                    if( symbol->GetReturnType() != RT_ERROR ){
                      if( symbol->GetSymbolType() == ST_PARAMETER )
                        yyerror( "Parameter has same name as procedure" );
                      else
                        yyerror( "Procedure name already declared" );
                      symbol->SetReturnType( RT_ERROR );
                    }
                  }
                } arguments ';' { $<idStr>$ = $<idStr>2; }
                ;


arguments:      '(' parameter_lists ')'

                | /* lambda */
                ;


parameter_lists: parameter_list ';' parameter_lists
                
                | parameter_list 
                ;


parameter_list: ID { do_arguments = true; } type_identifier_list {
                  Stock* temp = new Stock( $<stock>3->type, $<idStr>1, $<stock>3 );
                  Stock* h = temp;
                  while( temp->next != NULL ) {
                    Symbol* symbol = GetDecentSymbol( symtab->GetCurrentScopeName(), temp->name, false );
                    if( symbol == NULL ) {
                      symbol = newSymbol( temp->name, temp->type, ST_PARAMETER );
                      symtab->AddSymbol( symbol );
                    } else {
                      if( symbol->GetReturnType() != RT_ERROR ){
                        yyerror( "Redeclaration" );
                        symbol->SetReturnType( RT_ERROR );
                      }
                    }
                    temp = temp->next;
                  }
                  while( h ) {
                    temp = h;
                    h = h->next;
                    delete temp;
                  }
                }
                ;


compound_statement:
                PASCALBEGIN optional_statements END {
                  $<node>$ = $<node>2;
                }
                ;


optional_statements:
                statement_list {
                  $<node>$ = $<node>1;
                }

                | /* lambda */ {
                  $<node>$ = tree->CreateLeaf();
                }
                ;


statement_list: statement {
                  $<node>$ = $<node>1;
                }

                | statement_list ';' statement {
                  $<node>$ = tree->CreateParentNode(NODE_STATEMENT_LIST, RT_VOID, $<node>1, $<node>3);
                }
                ;


statement:      variable ASSIGNOP expression {
                  coerce( $<node>3, $<node>1->GetReturnType() );
                  $<node>$ = tree->CreateParentNode(NODE_ASSIGNMENT, RT_VOID, $<node>1, $<node>3 );
                }
                
                | procedure_statement {
                  $<node>$ = $<node>1;
                }
                
                | compound_statement {
                  $<node>$ = $<node>1;
                }
                
                | IF boollikeexp THEN statement optional_else_statement {
                  Node* statement;
                  if( $<node>5 == NULL ) {
                    statement = $<node>4;
                  } else {
                    statement = tree->CreateParentNode( NODE_IF_TARGETS, RT_VOID, $<node>4, $<node>5 );
                  }
                  $<node>$ = tree->CreateParentNode( NODE_IF, RT_VOID, $<node>2, statement );
                }
                
                | WHILE boollikeexp DO statement {
                  $<node>$ = tree->CreateParentNode( NODE_WHILE, RT_VOID, $<node>2, $<node>4 );
                }
                ;


optional_else_statement:
                ELSE statement {
                  $<node>$ = $<node>2;
                }
                | /* lambda */ {
                  $<node>$ = NULL;
                }
                ;


variable:
                ID {
                  string varName = $<idStr>1;
                  free( $<idStr>1 );
                  Symbol* symbol = GetDecentSymbol( varName );

                  if( symbol == NULL ) {
                    yyerror( ("Undefined identifier " + varName).data() );
                    symbol = newSymbol( varName, RT_ERROR, ST_ERROR );
                    symtab->AddSymbol( symbol );
                  } else {
                    if( symbol->GetSymbolType() == ST_FUNCTION ) {
                      if( varName != symtab->GetCurrentScopeName() )
                        yyerror( "Assignment to function as a variable" );
                    } else if( !( symbol->GetSymbolType() == ST_VARIABLE || symbol->GetSymbolType() == ST_PARAMETER  || symbol->GetSymbolType() == ST_ERROR ) )
                      yyerror( "Not a variable" );
                  }   
                  $<node>$ = tree->CreateLeaf( symbol );
                }
                
                ;


procedure_statement:
                ID {
                  Node* temp = NULL;
                  Symbol* function = GetDecentSymbol($<idStr>1);
                  if( function == NULL ) {
                    yyerror( "Undefined procedure" );
                    function = newSymbol( $<idStr>1, RT_ERROR, ST_ERROR );
                    symtab->AddSymbol( function );
                  } else if( function->GetSymbolType() == ST_PROCEDURE )
                    checkParameters( $<idStr>1, temp );
                  ReturnType returnType = function->GetReturnType();
                  if( function->GetSymbolType() != ST_PROCEDURE && function->GetSymbolType() != ST_ERROR )
                    yyerror( "Procedure call on non-procedure" );
                  $<node>$ = tree->CreateParentNode(NODE_PROCCALL, returnType, tree->CreateLeaf( function ) );
                  free($<idStr>1);
                }
                
                | ID '(' expression_list ')' {
                  Symbol* function = GetDecentSymbol($<idStr>1);
                  if( function == NULL ) {
                    yyerror( "Undefined procedure" );
                    function = newSymbol( $<idStr>1, RT_ERROR, ST_ERROR );
                    symtab->AddSymbol( function );
                  } else if( function->GetSymbolType() == ST_PROCEDURE )
                    checkParameters( $<idStr>1 , $<node>3 );
                  ReturnType returnType = function->GetReturnType();
                  if( function->GetSymbolType() != ST_PROCEDURE  && function->GetSymbolType() != ST_ERROR )
                    yyerror( "Procedure call on non-procedure" );
                  $<node>$ = tree->CreateParentNode(NODE_PROCCALL, returnType, tree->CreateLeaf( function ), $<node>3 );
                  free($<idStr>1);
                }
                
                ;


expression_list:
                expression
                
                | expression ',' expression_list {
                  $<node>$ = tree->CreateParentNode(NODE_EXPRLIST, RT_VOID, $<node>1, $<node>3 );
                }
                ;

boollikeexp:    expression RELOP expression {
                  ReturnType returnType = RT_BOOL;
                  coerce( $<node>1, $<node>3 );
                  if( $<idStr>2[0] == '<' ) {
                    if( $<idStr>2[1] == '=' )
                      $<node>$ = tree->CreateParentNode(NODE_REL_LTE, returnType, $<node>1, $<node>3 );
                    else if( $<idStr>2[1] == '>' )
                      $<node>$ = tree->CreateParentNode(NODE_REL_NOTEQUAL, returnType, $<node>1, $<node>3 );
                    else
                      $<node>$ = tree->CreateParentNode(NODE_REL_LT, returnType, $<node>1, $<node>3 );
                  } else if( $<idStr>2[0] == '>' ) {
                    if( $<idStr>2[1] == '=' )
                      $<node>$ = tree->CreateParentNode(NODE_REL_GTE, returnType, $<node>1, $<node>3 );
                    else
                      $<node>$ = tree->CreateParentNode(NODE_REL_GT, returnType, $<node>1, $<node>3 );
                  } else { // =
                    $<node>$ = tree->CreateParentNode(NODE_REL_EQUAL, returnType, $<node>1, $<node>3 );
                  }
                  free($<idStr>2);
                }
                
                | '(' boollikeexp ')' {
                  $<node>$ = $<node>2;
                }
                ; /* to allow stuff like "if (a > 3) then" */


expression:     simple_expression
                ;


simple_expression:
                term {
                  $<node>$ = $<node>1;
                }
                
                | signplus term {
                  ReturnType returnType = $<node>2->GetReturnType();
                  $<node>$ = tree->CreateParentNode(NODE_SIGNPLUS, returnType, $<node>2);
                }
        
                | signminus term {
                  ReturnType returnType = $<node>2->GetReturnType();
                  $<node>$ = tree->CreateParentNode(NODE_SIGNMINUS, returnType, $<node>2);
                }
        
                | simple_expression '+' term {

                  ReturnType returnType = coerce( $<node>1, $<node>3 );
                  $<node>$ = tree->CreateParentNode(NODE_ADD, returnType, $<node>1, $<node>3);
                }
                
                | simple_expression '-' term {
                  ReturnType returnType = coerce( $<node>1, $<node>3 );
                  $<node>$ = tree->CreateParentNode(NODE_SUB, returnType, $<node>1, $<node>3);
                }
                
                | simple_expression ADDOP term {
                  ReturnType returnType = RT_INT;
                  if( $<node>1->GetReturnType() != RT_INT || $<node>1->GetReturnType() != RT_INT ) {
                    yyerror( "Bitwise or only defined on integers" );
                    returnType = RT_ERROR;
                  }
                  $<node>$ = tree->CreateParentNode(NODE_OR, returnType, $<node>1, $<node>3);
                  free($<idStr>2);
                }
                ;


term:           factor {
                  $<node>$ = $<node>1;
                }
                
                | term MULOP factor {
                  ReturnType returnType = RT_INT;
                  NodeType nodeType = NODE_UNKNOWN;
                  if( $<idStr>2[0] == '*' || $<idStr>2[0] == '/' ) {
                    returnType = coerce( $<node>1, $<node>3 );
                  } else if( $<node>1->GetReturnType() != RT_INT || $<node>3->GetReturnType() != RT_INT ) {
                    yyerror( ("Operation " + string( $<idStr>2 ) + " not defined on non-integers").data() );
                    returnType = RT_ERROR;
                  }
                  switch( $<idStr>2[0] ) {
                    case '/':
                      nodeType = NODE_DIV;
                      break;
                    case '*':
                      nodeType = NODE_MUL;
                      break;
                    case 'd':
                      nodeType = NODE_IDIV;
                      break;
                    case 'm':
                      nodeType = NODE_MOD;
                      break;
                    case 'a':
                      nodeType = NODE_AND;
                      break;
                  }
                  $<node>$ = tree->CreateParentNode(nodeType, returnType, $<node>1, $<node>3);
                  free($<idStr>2);
                }
                ;


factor:         ID {
                  Symbol* symbol = GetDecentSymbol( $<idStr>1 );
                  if( symbol == NULL ) {
                    yyerror( "Undefined identifier" );
                    symbol = newSymbol( $<idStr>1, RT_ERROR, ST_ERROR );
                    symtab->AddSymbol( symbol );
                  }
                  if( symbol->GetSymbolType() == ST_FUNCTION ) {
                    Node* temp = NULL;
                    checkParameters( $<idStr>1 , temp );
                  } else if( !(symbol->GetSymbolType() == ST_VARIABLE || symbol->GetSymbolType() == ST_PARAMETER || symbol->GetSymbolType() == ST_ERROR ) ) {
                    yyerror( "Expected variable or function" );
                  }
                  $<node>$ = tree->CreateLeaf( symbol );
                  free($<idStr>1);
                }
                
                | ID '(' expression_list ')' {
                  Symbol* function = GetDecentSymbol($<idStr>1);
                  if( function == NULL ) {
                    yyerror( "Undefined function" );
                    function = newSymbol( $<idStr>1, RT_ERROR, ST_ERROR );
                    symtab->AddSymbol( function );
                  } else if( function->GetSymbolType() == ST_FUNCTION ) {
                    checkParameters( $<idStr>1 , $<node>3 );
                  }
                  ReturnType returnType = function->GetReturnType();
                  if( function->GetSymbolType() != ST_FUNCTION  && function->GetSymbolType() != ST_ERROR ) {
                    yyerror( "Function call on non-function" );
                    $<node>$ = tree->CreateParentNode(NODE_FUNCTIONCALL, RT_ERROR, $<node>3);
                  }
                  else {
                    $<node>$ = tree->CreateParentNode(NODE_FUNCTIONCALL, returnType, $<node>3);
                  }
                  free($<idStr>1);
                }
                
                | NUM {
                  bool isint = true;
                  char* temp = $<idStr>1;
                  while( (*temp) != '\0' ) {
                    if( (*temp) == '.' || (*temp) == 'E' ) {
                      isint = false;
                      break;
                    }
                    temp++;
                  }
                  if( isint ) {
                    $<node>$ = tree->CreateLeaf( atoi($<idStr>1) );
                  } else {
                    $<node>$ = tree->CreateLeaf( float( atof ( $<idStr>1 ) ) );
                  }
                  free($<idStr>1);
                }
                
                | '(' expression ')' {
                  $<node>$ = $<node>2;
                }
                
                | NOT factor {
                  ReturnType returnType = $<node>2->GetReturnType();
                  $<node>$ = tree->CreateParentNode(NODE_NOT, returnType, $<node>2);
                }
                
                ;


signplus:       '+'
                ;
                
signminus:      '-' %prec UMINUS
                ;

%%

/* End of rules, more C code will follow now */

void initialize()
{
  /* Create & initialize the abstract data structures */
  tree = new SyntaxTree;
  symtab = new SymbolTable;

}


void cleanup()
{
  /* Free objects */
  delete tree;
  delete symtab; 
}


int main( int argc, char** argv )
{
  int parseResult;
  
  initialize();

  if( argc == 2 && string(argv[1]) == "--no-warnings" )
    warnings = -1;

  /* Start the parsing procedure */
  parseResult = yyparse();
  
  /* Dump the syntax tree & symbol table */
  tree->Dump (stdout);
  printf("\n");
  symtab->Dump (stdout);

  if( warnings == -1 )
    printf( "Errors: %d\n", errors );
  else
    printf( "Errors: %d\nWarnings: %d\n", errors, warnings );
  
  cleanup();
  
  /* Return the exit code. Note that the exit code does not depend on
     parseResult only, but also on the result of your semantic checks */
  return parseResult || err_var;
}


static void yyerror(const char *s, bool warning )
{
  if( !warning ) {
    err_var = true;
    fprintf(stderr, "line %d: Error: %s!\n", lineno, s);
    errors++;
  } else if( warnings != -1 ) {
    fprintf(stderr, "line %d: Warning: %s!\n", lineno, s);
    warnings++;
  }
}

int yywrap()
{
  return(1);
}
