/* C declarations */
%{

int msglevel = 90;

#include <stdio.h>
#include "syntax_tree.h"
#include "symbol_table.h"
#include "types.h"
#include "y.tab.h"
#include "error_reporting.h"


/* extern C declarations */
#if defined(__cplusplus)
extern "C" {
#endif

/* should be defined in stdio.h */
extern int fileno(FILE *);
int yylex(void);

#if defined(__cplusplus)
}
#endif

int statementno = 1;
int string_state = 0;

%}

%option nounput

LETTER	[a-zA-Z]
DIGIT	[0-9]
ID		{LETTER}({LETTER}|{DIGIT})*
DDEC	{DIGIT}+
DHEX	0x[0-9a-fA-F]+
DBIN	0b[01]+
DANY	({LETTER}|{DIGIT})+"_"{DDEC}

DDEC_TAIL	("."{DDEC})
DDEC_EXP	((("e"|"E")("+"|"-")?{DDEC}))
FLT			{DDEC}"."{DDEC_TAIL}|{DDEC}{DDEC_EXP}|{DDEC}"."{DDEC_TAIL}{DDEC_EXP}
INT			{DDEC}|{DHEX}|{DBIN}
ASSIGNMENT	"="
SEMICOLON   ";"
LSEQ		"["
RSEQ		"]"
LPAR		"{"
RPAR		"}"
COMMA		","
LBRA		"("
RBRA		")"
ELLIPSIS	"..."


TYPENAME "int"|"str"|"flt"

%x STRING_STATE
%x STRING_ESCAPE_STATE

/* Patterns */
%%

"\""		{
				pmesg(90, "Lexer: entering string\n", yytext);
				string_state = 1;
				BEGIN(STRING_STATE);
				return STRING_BEGIN;
			}

<STRING_STATE>\n {
				YY_READ_NEWLINE;
				yylval.str = strdup(yytext);
				return STRING_PARTICLE;
			}

<STRING_STATE>\\ {
				string_state = 2;
				BEGIN(STRING_ESCAPE_STATE);
			}

<STRING_STATE>"\"" {
				pmesg(90, "Lexer: exiting string\n", yytext);
				BEGIN(INITIAL);
				return STRING_END;
			}

<STRING_STATE>[^\"\\\n]+ { // "
				yylval.str = strdup(yytext);
				return STRING_PARTICLE;
			}

<STRING_ESCAPE_STATE>n {
				yylval.str = (char*) malloc(2*sizeof(char));
				yylval.str[0] = '\n';
				yylval.str[1] = 0;
				string_state = 1;
				BEGIN(STRING_STATE);
				return STRING_PARTICLE;
			}
<STRING_ESCAPE_STATE>\\ {
				yylval.str = (char*) malloc(2*sizeof(char));
				yylval.str[0] = '\\';
				yylval.str[1] = 0;
				string_state = 1;
				BEGIN(STRING_STATE);
				return STRING_PARTICLE;
			}


<STRING_ESCAPE_STATE>. {
				yylval.str = strdup(yytext);
				pmesg(90, "Lexer: Unknown escape character \\%s\n", yytext);
				string_state = 1;
				BEGIN(STRING_STATE);
				return STRING_PARTICLE;
			}

{ELLIPSIS}	{
				pmesg(90, "Lexer: ELLIPSIS\n");
				return ELLIPSIS;
			}

{FLT}		{
				pmesg(90, "Lexer: FLT: %s\n", yytext);
				yylval.flt = strtod( yytext, nullptr );
				return FLT;
			}

{DANY}		{
				int i = 0;
				while( yytext[i] != '_' ) ++i;
				int base = atoi(yytext+i+1);
				char* end_ptr;
				yylval.num = strtol( yytext, &end_ptr, base );
				pmesg(90, "Lexer: INT: %s base %d\n", yytext, base );
				if( *end_ptr != '_' )
					pmesg(0, "Lexer: Invalid integral %s\n", yytext );
				return INT;
			}

{DDEC}		{
				pmesg(90, "Lexer: INT: %s\n", yytext);
				yylval.num = strtol( yytext, nullptr, 10 );
				return INT;
			}

{DHEX}		{
				pmesg(90, "Lexer: INT: %s\n", yytext);
				yylval.num = strtol( yytext+2, nullptr, 16 );
				return INT;
			}

{DBIN}		{
				pmesg(90, "Lexer: INT: %s\n", yytext);
				yylval.num = strtol( yytext+2, nullptr, 2 );
				return INT;
			}

type		{
				pmesg(90, "Lexer: VTYPE: %s\n", yytext);
				return VTYPE;
			}

func		{
				pmesg(90, "Lexer: FUNC\n");
				return FUNC;
			}

if			{
				pmesg(90, "Lexer: IF\n");
				return IF;
			}

else		{
				pmesg(90, "Lexer: ELSE\n");
				return ELSE;
			}

while		{
				pmesg(90, "Lexer: WHILE\n");
				return WHILE;
			}

for			{
				pmesg(90, "Lexer: FOR\n");
				return FOR;
			}

in			{
				pmesg(90, "Lexer: IN\n");
				return IN;
			}	


"*"|"/"|"%"	{
				pmesg(90, "Lexer: MULOP: %s\n", yytext);
				if( yytext[0] == '/' )
					yylval.num = 0;
				else if( yytext[0] == '*' )
					yylval.num = 1;
				else
					yylval.num = 2;
				return MULOP;
			}

"-"|"+"		{
				pmesg(90, "Lexer: ADDOP: %s\n", yytext);
				if( yytext[0] == '-' )
					yylval.num = 0;
				else
					yylval.num = 1;
				return ADDOP;
			}

"=="|"!="	{
				pmesg(90, "Lexer: RELOP: %s\n", yytext);
				if( yytext[0] == '=' )
					yylval.num = 0;
				else
					yylval.num = 1;
				return RELOP;
			}

%{/*{TYPENAME}	{
				pmesg(90, "Lexer: TYPENAME: %s\n", yytext);
				yylval.num = 0;
				return TYPENAME;
			}*/%}

{ID}		{ 
				pmesg(90, "Lexer: ID: %s\n", yytext);
				yylval.str = strdup(yytext);
				return ID;
			}

{ASSIGNMENT} {
				pmesg(90, "Lexer: Assignment\n", yytext);
				return ASSIGNMENT;
			}

{LSEQ}		{
				return LSEQ;
			}

{RSEQ}		{
				return RSEQ;
			}

{LBRA}		{
				return LBRA;
			}

{RBRA}		{
				return RBRA;
			}


{COMMA}		{
				return COMMA;
			}

{SEMICOLON}	{
				++statementno;
				return SEMICOLON;
			}

\n			{
				YY_READ_NEWLINE;
			}

[ \t]+		{
				/* Remove whitespace */
			}

.			{
				pmesg(90, "Lexer: Single non-identifier character: %s\n", yytext ); 
				return(yytext[0]);
			}

%%

int yywrap() {
	return 1;
}