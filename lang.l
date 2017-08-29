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

DDEC_TAIL	"."{DDEC}
DDEC_EXP	((("e"|"E")("+"|"-")?{DDEC}))
FLT			{DDEC}{DDEC_TAIL}|{DDEC}{DDEC_EXP}|{DDEC}{DDEC_TAIL}{DDEC_EXP}
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

%x STRING_STATE
%x STRING_ESCAPE_STATE

/* Patterns */
%%

"\""		{
				lexer_out << "entering string" << std::endl;
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
				lexer_out << "exiting string" << std::endl;
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
<STRING_ESCAPE_STATE>"\"" {
				yylval.str = (char*) malloc(2*sizeof(char));
				yylval.str[0] = '\"'; //"
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
				lexer_out << "Unknown escape character \\" << yytext << std::endl;
				string_state = 1;
				BEGIN(STRING_STATE);
				return STRING_PARTICLE;
			}

{ELLIPSIS}	{
				lexer_out << "ELLIPSIS" << std::endl;
				return ELLIPSIS;
			}

{FLT}		{
				lexer_out << "FLT: " << yytext << std::endl;
				yylval.flt = strtod( yytext, nullptr );
				return FLT;
			}

{DANY}		{
				int i = 0;
				while( yytext[i] != '_' ) ++i;
				int base = atoi(yytext+i+1);
				char* end_ptr;
				yylval.num = strtol( yytext, &end_ptr, base );
				lexer_out << "INT: " << yytext << " base " << base << std::endl;
				if( *end_ptr != '_' )
					lerr << error_line() << "Invalid integral " << yytext << std::endl;
				return INT;
			}

{DDEC}		{
				lexer_out << "INT: " << yytext << std::endl;
				yylval.num = strtol( yytext, nullptr, 10 );
				return INT;
			}

{DHEX}		{
				lexer_out << "INT: " << yytext << std::endl;
				yylval.num = strtol( yytext+2, nullptr, 16 );
				return INT;
			}

{DBIN}		{
				lexer_out << "INT: " << yytext << std::endl;
				yylval.num = strtol( yytext+2, nullptr, 2 );
				return INT;
			}

"->"		{
				lexer_out << "MAPSTO" << std::endl;
				return MAPSTO;
			}

break		{
				lexer_out << "BREAK" << std::endl;
				return BREAK;
			}

continue	{
				lexer_out << "CONTINUE" << std::endl;
				return CONTINUE;
			}

type		{
				lexer_out << "VTYPE: " << yytext << std::endl;
				return VTYPE;
			}

func		{
				lexer_out << "FUNC" << std::endl;
				return FUNC;
			}

if			{
				lexer_out << "IF" << std::endl;
				return IF;
			}

else		{
				lexer_out << "ELSE" << std::endl;
				return ELSE;
			}

while		{
				lexer_out << "WHILE" << std::endl;
				return WHILE;
			}

for			{
				lexer_out << "FOR" << std::endl;
				return FOR;
			}

in			{
				lexer_out << "IN" << std::endl;
				return IN;
			}

return		{
				lexer_out << "RETURN" << std::endl;
				return RETURN;
			}


"*"|"/"|"%"	{
				lexer_out << "MULOP: " << yytext << std::endl;
				if( yytext[0] == '/' )
					yylval.num = 0;
				else if( yytext[0] == '*' )
					yylval.num = 1;
				else
					yylval.num = 2;
				return MULOP;
			}

"-"|"+"		{
				lexer_out << "ADDOP: " << yytext << std::endl;
				if( yytext[0] == '-' )
					yylval.num = 0;
				else
					yylval.num = 1;
				return ADDOP;
			}

"<="|">=" {
				lexer_out << "RELOP: " << yytext << std::endl;
				yylval.num = 4+( yytext[0] == '>' );
				return RELOP;
			}

"<"|">" {
				lexer_out << "RELOP: " << yytext << std::endl;
				yylval.num = 2+( yytext[0] == '>' );
				return RELOP;
			}

"=="|"!="	{
				lexer_out << "RELOP: " << yytext << std::endl;
				if( yytext[0] == '=' )
					yylval.num = 0;
				else
					yylval.num = 1;
				return RELOP;
			}

"|"|"&"		{
				lexer_out << "JOIN_MEET: " << yytext << std::endl;
				if( yytext[0] == '|' )
					yylval.num = 0;
				else
					yylval.num = 1;
				return JOIN_MEET;
			}

%{/*{TYPENAME}	{
				lexer_out << "TYPENAME: " << yytext << std::endl;
				yylval.num = 0;
				return TYPENAME;
			}*/%}

{ID}		{ 
				lexer_out << "ID: " << yytext << std::endl;
				yylval.str = strdup(yytext);
				return ID;
			}

{ASSIGNMENT} {
				lexer_out << "ASSIGNMENT" << std::endl;
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
				lexer_out << "Single non-identifier character: " << yytext << std::endl; 
				return(yytext[0]);
			}

%%

int yywrap() {
	return 1;
}