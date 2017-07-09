/* C declarations */
%{

int msglevel = 90;

#include <stdio.h>
#include "debug.h"
#include "syntax_tree.h"
#include "symbol_table.h"
#include "types.h"
#include "y.tab.h"

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


int lineno = 1;

%}

%option nounput

LETTER	[a-zA-Z]
DIGIT	[0-9]
ID		{LETTER}({LETTER}|{DIGIT})*
DDEC	{DIGIT}+
DHEX	0x[0-9a-fA-F]+
DBIN	0b[01]+

DDEC_TAIL	("."{DDEC})
DDEC_EXP	((("e"|"E")("+"|"-")?{DDEC}))
FLT			{DDEC}"."{DDEC_TAIL}|{DDEC}{DDEC_EXP}|{DDEC}"."{DDEC_TAIL}{DDEC_EXP}
INT			{DDEC}|{DHEX}|{DBIN}
ASSIGNMENT	"="
SEMICOLON   ";"

TYPENAME "int"|"str"|"flt"

/* Patterns */
%%

{FLT}		{
				pmesg(90, "Lexer: FLT: %s\n", yytext);
				yylval.flt = strtod( yytext, nullptr );
				return FLT;
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

"*"|"/"		{
				pmesg(90, "Lexer: MULOP: %s\n", yytext);
				// yylval.idStr = strdup(yytext);
				// return MULOP;
			}

{TYPENAME}	{
				pmesg(90, "Lexer: TYPENAME: %s\n", yytext);
				yylval.num = 0;
				return TYPENAME;
			}

{ID}		{ 
				pmesg(90, "Lexer: ID: %s\n", yytext);
				yylval.str = strdup(yytext);
				return ID;
			}

{ASSIGNMENT} {
				pmesg(90, "Lexer: Assignment\n", yytext);
				return ASSIGNMENT;
			}

{SEMICOLON}	{
				return SEMICOLON;
			}

\n			{
				++lineno;
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