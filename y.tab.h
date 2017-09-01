/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    FLT = 259,
    ID = 260,
    TYPENAME = 261,
    VTYPE = 262,
    ASSIGNMENT = 263,
    ADDOP = 264,
    MULOP = 265,
    COMMA = 266,
    SEMICOLON = 267,
    LSEQ = 268,
    RSEQ = 269,
    LBRA = 270,
    RBRA = 271,
    STRING_BEGIN = 272,
    STRING_END = 273,
    STRING_PARTICLE = 274,
    FUNC = 275,
    IF = 276,
    WHILE = 277,
    ELSE = 278,
    FOR = 279,
    IN = 280,
    RELOP = 281,
    ELLIPSIS = 282,
    JOIN_MEET = 283,
    BREAK = 284,
    CONTINUE = 285,
    RETURN = 286,
    MAPSTO = 287,
    SIZE_OF = 288
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 74 "lang.y" /* glr.c:197  */

	char *str;
	int64_t num;
	double flt;
	const type_t* typ;
	std::vector<type_t>* typlst;
	struct syntaxTree::node *node;

#line 97 "y.tab.h" /* glr.c:197  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
