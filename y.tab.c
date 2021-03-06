/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Skeleton implementation for Bison GLR parsers in C

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

/* C GLR parser skeleton written by Paul Hilfinger.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.c"

/* Pure parsers.  */
#define YYPURE 0






/* First part of user declarations.  */
#line 2 "lang.y" /* glr.c:240  */


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



#line 119 "y.tab.c" /* glr.c:240  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "y.tab.h"

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template, here we set
   the default value of $$ to a zeroed-out value.  Since the default
   value is undefined, this behavior is technically correct.  */
static YYSTYPE yyval_default;

/* Copy the second part of user declarations.  */

#line 147 "y.tab.c" /* glr.c:263  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t) -1)

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(Env) setjmp (Env)
/* Pacify clang.  */
# define YYLONGJMP(Env, Val) (longjmp (Env, Val), YYASSERT (0))
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#ifndef YYASSERT
# define YYASSERT(Condition) ((void) ((Condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  70
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   496

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNRULES -- Number of states.  */
#define YYNSTATES  194
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 9
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    92,    92,    97,   102,   105,   108,   111,   114,   117,
     120,   123,   126,   129,   132,   135,   136,   139,   145,   151,
     156,   159,   164,   168,   181,   187,   207,   207,   220,   229,
     233,   238,   242,   248,   248,   248,   248,   249,   252,   257,
     263,   265,   265,   267,   270,   273,   278,   281,   285,   285,
     289,   290,   302,   303,   313,   314,   326,   328,   331,   334,
     341,   344,   345,   347,   350,   354,   358,   361,   365,   368,
     372,   375,   378,   383,   389,   392,   399,   403,   408,   408,
     410,   414,   480,   506,   516,   521,   532,   536,   536,   548,
     553,   556,   560,   560,   577,   580,   587,   590,   594,   597,
     601,   605,   609,   613,   618,   622
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLT", "ID", "TYPENAME", "VTYPE",
  "ASSIGNMENT", "ADDOP", "MULOP", "COMMA", "SEMICOLON", "LSEQ", "RSEQ",
  "LBRA", "RBRA", "STRING_BEGIN", "STRING_END", "STRING_PARTICLE", "FUNC",
  "IF", "WHILE", "ELSE", "FOR", "IN", "RELOP", "ELLIPSIS", "JOIN_MEET",
  "BREAK", "CONTINUE", "RETURN", "MAPSTO", "SIZE_OF", "$accept",
  "entry_point", "sequential_block", "statement_list", "statement", "id",
  "typelist", "type", "declarations", "$@1", "declaration_list",
  "declaration_list_el", "assignment", "leaf_expression", "expression",
  "expression_list", "expression_list_ne", "relational",
  "chain_relational", "join_meet", "sum", "product", "factor", "constant",
  "size_of", "tuple_list", "tuple_list_ne", "tuple", "list", "list_items",
  "string_particles", "lvalue", "variable", "function_call",
  "function_definition", "inline_function", "$@2", "declaration",
  "parameter_list", "for", "$@3", "while", "if", "break", "continue",
  "return", "lbra", "rbra", "lseq", "rseq", YY_NULLPTR
};
#endif

#define YYPACT_NINF -117
#define YYTABLE_NINF -80

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const short int yypact[] =
{
     401,    99,  -117,  -117,  -117,     1,  -117,  -117,   -11,     1,
       9,     9,     9,    28,  -117,   365,    14,    40,  -117,  -117,
     327,    72,  -117,    51,    27,  -117,    64,  -117,    31,    50,
      70,    74,  -117,  -117,     9,     9,    77,    78,    29,  -117,
    -117,  -117,  -117,  -117,    82,    83,    86,   283,   370,   365,
    -117,     9,    59,    85,  -117,     9,   283,   155,  -117,  -117,
       4,     9,   365,   365,     1,  -117,  -117,   365,  -117,   365,
    -117,  -117,   432,     1,  -117,  -117,  -117,  -117,   365,   365,
     365,   365,   437,   437,   365,   437,  -117,  -117,  -117,  -117,
      73,    48,     3,    73,    94,    73,  -117,  -117,    85,    30,
      16,  -117,   437,   365,  -117,  -117,     1,    96,  -117,  -117,
      75,    73,    73,    88,    73,    73,    97,   104,    73,  -117,
     114,  -117,   113,  -117,   100,  -117,  -117,  -117,    73,    73,
    -117,    73,  -117,    75,  -117,  -117,   365,     9,  -117,    75,
      93,  -117,  -117,  -117,  -117,     9,    73,    75,   401,   401,
     365,  -117,  -117,   463,  -117,   365,     1,   365,  -117,  -117,
    -117,  -117,   116,  -117,   127,  -117,    75,    75,   117,   108,
     111,    73,   456,  -117,  -117,  -117,  -117,   365,  -117,   125,
    -117,   401,   401,   401,  -117,  -117,  -117,  -117,  -117,   401,
     401,  -117,    85,  -117
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,    57,    58,    19,     0,   104,   102,     0,     0,
       0,     0,     0,    98,   100,     0,     0,     0,     9,     2,
       0,    80,    26,     0,     0,    56,     0,    40,    46,    50,
      52,    54,    33,    36,    61,    60,     0,    34,    35,    15,
      62,    11,    12,    10,     0,     0,     0,     0,     0,     0,
      17,    80,    74,     0,    34,    35,     0,     0,    22,    76,
       0,     0,     0,     0,     0,    99,   101,     0,    64,     0,
       1,     5,     0,     0,     6,    18,     7,     8,     0,     0,
       0,     0,     0,     0,     0,     0,    13,    14,    16,   103,
       0,    20,     0,     0,    90,     0,    69,   105,     0,    74,
       0,    71,     0,     0,    39,    73,     0,    74,    59,    77,
       0,     0,     0,     0,     0,     0,    20,    43,     0,    42,
      31,    27,    29,    47,    48,    51,    53,    55,     0,     0,
      32,     0,    25,     0,    89,    38,     0,    37,    68,     0,
       0,     3,    72,    70,    75,    24,     0,     0,     0,     0,
       0,    63,    23,     0,    81,     0,     0,     0,    83,    84,
      82,    21,    66,    65,     0,    91,     0,     0,     0,    96,
      94,     0,     0,    44,    30,    28,    49,     0,    85,     0,
      86,     0,     0,     0,    92,    67,    87,    97,    95,     0,
       0,    93,     0,    88
};

  /* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -117,  -117,   -28,     5,  -116,    45,   -71,    -4,  -117,  -117,
     -15,  -117,  -117,  -117,   209,    38,    -9,  -117,    -7,   -76,
      62,    65,  -117,  -117,  -117,  -117,   -24,  -117,   138,   -39,
    -117,  -117,    26,    71,  -117,  -117,  -117,  -117,  -103,  -117,
    -117,  -117,  -117,  -117,  -117,  -117,     0,   190,    49,   -40
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] =
{
      -1,    17,    18,    98,    20,    51,    90,    22,    23,    73,
     121,   122,    24,    25,    26,   118,   119,    27,   123,    28,
      29,    30,    31,    32,    33,    93,   163,    34,    35,    53,
      60,    36,    54,    55,    39,    40,   190,    94,    95,    41,
     189,    42,    43,    44,    45,    46,    56,    96,    57,   101
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const short int yytable[] =
{
      47,   115,   124,   125,   135,    19,     4,   146,    59,   100,
      62,    63,    64,   105,   136,    67,    69,   142,   100,    89,
      47,    72,   108,   109,     7,    71,    37,     6,    75,     7,
      97,    65,   169,   170,    82,    83,   165,   -79,    85,    76,
      70,   103,    77,    91,     7,    21,    37,    47,    47,    48,
      58,   102,   106,     4,    61,    85,    47,    78,   141,   133,
     143,   110,   161,    74,   144,    21,   187,   188,   116,    48,
     103,    38,    47,   191,    37,    89,    77,   -24,    79,    80,
       4,   124,     5,   -24,    81,    84,   -78,     7,   -24,    89,
       7,    38,    21,    21,    86,    87,   115,    48,    88,    97,
      49,    21,     2,     3,     4,   139,   106,   103,   133,   113,
     147,    50,     6,   150,     7,   153,     8,    21,   120,    38,
     128,   129,   155,   131,   156,   166,   157,   177,   178,   116,
       6,   182,    16,   147,   183,   106,   134,   164,   186,   147,
     180,   175,   126,   116,   173,   167,   127,   147,    47,    47,
     176,   134,   193,   185,    68,   145,    49,     0,     2,     3,
       4,     0,   179,   116,     0,     0,   147,   147,     6,    97,
       7,     0,     8,     0,    37,    37,     0,     0,   145,     0,
       0,    47,    47,    47,   145,     0,     0,     0,    16,    47,
      47,     0,   145,    21,    21,   192,     0,    48,    48,     0,
       0,   120,     0,     0,     0,     0,     0,    37,    37,    37,
      52,   145,   145,     0,     0,    37,    37,   181,     0,    38,
      38,     0,     0,     0,    66,     0,    21,    21,    21,     0,
      48,    48,    48,     0,    21,    21,     0,     0,    48,    48,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,    38,    38,     0,    92,    99,    52,     0,
      38,    38,     0,     0,     0,    92,   107,     0,     0,     0,
       0,   111,   112,     0,     0,     0,   107,     0,   114,     0,
     132,   117,   137,   138,    49,   140,     2,     3,     4,     0,
       5,   117,   117,   130,   117,     0,     6,     0,     7,    89,
       8,   148,   149,     0,   151,   152,     0,     0,   154,     0,
       0,   117,   107,     0,     0,     0,    16,     0,   158,   159,
       0,   160,     0,     0,     0,     0,     0,    -4,     1,     0,
       2,     3,     4,     0,     5,     0,   168,     0,     0,     0,
       6,    -4,     7,     0,     8,   162,     0,     9,    10,    11,
       0,    12,     0,     0,     0,     0,    13,    14,    15,   171,
      16,   184,   117,     0,   174,     0,    49,     0,     2,     3,
       4,     1,     0,     2,     3,     4,     0,     5,     6,     0,
       7,    52,     8,     6,    97,     7,   162,     8,     0,     0,
       9,    10,    11,     0,    12,     0,     0,     0,    16,    13,
      14,    15,     1,    16,     2,     3,     4,     0,     5,     0,
       0,     0,     0,     0,     6,     0,     7,     0,     8,     0,
       0,     9,    10,    11,     0,    12,     0,     0,     0,     0,
      13,    14,    15,    49,    16,     2,     3,     4,    49,     5,
       2,     3,     4,     0,     0,     6,     0,     7,   -41,     8,
       6,     0,     7,   -41,     8,     0,     0,    49,     0,     2,
       3,     4,     0,     0,   172,    16,     2,     3,     4,     6,
      16,     7,   -45,     8,     0,     0,     6,     0,     7,     0,
       8,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,    16
};

static const short int yycheck[] =
{
       0,    72,    78,    79,     1,     0,     5,   110,    19,    48,
      10,    11,    12,    53,    11,     1,    16,     1,    57,    16,
      20,    21,    18,    19,    15,    20,     0,    13,     1,    15,
      14,     3,   148,   149,    34,    35,   139,     8,    38,    12,
       0,    11,    12,    47,    15,     0,    20,    47,    48,     0,
       5,    51,    56,     5,     9,    55,    56,    26,    98,    11,
     100,    61,   133,    12,   103,    20,   182,   183,    72,    20,
      11,     0,    72,   189,    48,    16,    12,     5,    28,     9,
       5,   157,     7,    11,    10,     8,     8,    15,    16,    16,
      15,    20,    47,    48,    12,    12,   167,    48,    12,    14,
       1,    56,     3,     4,     5,    11,   110,    11,    11,    64,
     110,    12,    13,    25,    15,    11,    17,    72,    73,    48,
      82,    83,     8,    85,    11,    32,    26,    11,     1,   133,
      13,    23,    33,   133,    23,   139,    91,   137,    13,   139,
     168,   156,    80,   147,   153,   145,    81,   147,   148,   149,
     157,   106,   192,   177,    16,   110,     1,    -1,     3,     4,
       5,    -1,   166,   167,    -1,    -1,   166,   167,    13,    14,
      15,    -1,    17,    -1,   148,   149,    -1,    -1,   133,    -1,
      -1,   181,   182,   183,   139,    -1,    -1,    -1,    33,   189,
     190,    -1,   147,   148,   149,   190,    -1,   148,   149,    -1,
      -1,   156,    -1,    -1,    -1,    -1,    -1,   181,   182,   183,
       1,   166,   167,    -1,    -1,   189,   190,   168,    -1,   148,
     149,    -1,    -1,    -1,    15,    -1,   181,   182,   183,    -1,
     181,   182,   183,    -1,   189,   190,    -1,    -1,   189,   190,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   181,   182,   183,    -1,    47,    48,    49,    -1,
     189,   190,    -1,    -1,    -1,    56,    57,    -1,    -1,    -1,
      -1,    62,    63,    -1,    -1,    -1,    67,    -1,    69,    -1,
      90,    72,    92,    93,     1,    95,     3,     4,     5,    -1,
       7,    82,    83,    84,    85,    -1,    13,    -1,    15,    16,
      17,   111,   112,    -1,   114,   115,    -1,    -1,   118,    -1,
      -1,   102,   103,    -1,    -1,    -1,    33,    -1,   128,   129,
      -1,   131,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,     4,     5,    -1,     7,    -1,   146,    -1,    -1,    -1,
      13,    14,    15,    -1,    17,   136,    -1,    20,    21,    22,
      -1,    24,    -1,    -1,    -1,    -1,    29,    30,    31,   150,
      33,   171,   153,    -1,   155,    -1,     1,    -1,     3,     4,
       5,     1,    -1,     3,     4,     5,    -1,     7,    13,    -1,
      15,   172,    17,    13,    14,    15,   177,    17,    -1,    -1,
      20,    21,    22,    -1,    24,    -1,    -1,    -1,    33,    29,
      30,    31,     1,    33,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    15,    -1,    17,    -1,
      -1,    20,    21,    22,    -1,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,     1,    33,     3,     4,     5,     1,     7,
       3,     4,     5,    -1,    -1,    13,    -1,    15,    16,    17,
      13,    -1,    15,    16,    17,    -1,    -1,     1,    -1,     3,
       4,     5,    -1,    -1,     1,    33,     3,     4,     5,    13,
      33,    15,    16,    17,    -1,    -1,    13,    -1,    15,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    33
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     1,     3,     4,     5,     7,    13,    15,    17,    20,
      21,    22,    24,    29,    30,    31,    33,    35,    36,    37,
      38,    39,    41,    42,    46,    47,    48,    51,    53,    54,
      55,    56,    57,    58,    61,    62,    65,    66,    67,    68,
      69,    73,    75,    76,    77,    78,    79,    80,    82,     1,
      12,    39,    48,    63,    66,    67,    80,    82,    39,    19,
      64,    39,    80,    80,    80,     3,    48,     1,    62,    80,
       0,    37,    80,    43,    12,     1,    12,    12,    26,    28,
       9,    10,    80,    80,     8,    80,    12,    12,    12,    16,
      40,    41,    48,    59,    71,    72,    81,    14,    37,    48,
      63,    83,    80,    11,    81,    83,    41,    48,    18,    19,
      80,    48,    48,    39,    48,    40,    41,    48,    49,    50,
      39,    44,    45,    52,    53,    53,    54,    55,    49,    49,
      48,    49,    81,    11,    39,     1,    11,    81,    81,    11,
      81,    83,     1,    83,    63,    39,    72,    80,    81,    81,
      25,    81,    81,    11,    81,     8,    11,    26,    81,    81,
      81,    40,    48,    60,    80,    72,    32,    80,    81,    38,
      38,    48,     1,    50,    48,    44,    52,    11,     1,    41,
      36,    82,    23,    23,    81,    60,    13,    38,    38,    74,
      70,    38,    37,    83
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    34,    35,    36,    37,    37,    38,    38,    38,    38,
      38,    38,    38,    38,    38,    38,    38,    38,    38,    39,
      40,    40,    41,    41,    41,    41,    43,    42,    44,    44,
      45,    45,    46,    47,    47,    47,    47,    47,    47,    47,
      48,    49,    49,    50,    50,    50,    51,    51,    52,    52,
      53,    53,    54,    54,    55,    55,    56,    57,    57,    57,
      57,    57,    57,    58,    58,    59,    60,    60,    61,    61,
      62,    62,    62,    62,    63,    63,    64,    64,    65,    65,
      66,    67,    67,    67,    67,    67,    68,    70,    69,    71,
      72,    72,    74,    73,    75,    75,    76,    76,    77,    77,
      78,    79,    80,    81,    82,    83
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     3,     1,     2,     2,     2,     2,     1,
       1,     1,     1,     2,     2,     1,     2,     2,     2,     1,
       1,     3,     2,     4,     1,     3,     0,     3,     3,     1,
       3,     1,     3,     1,     1,     1,     1,     3,     3,     3,
       1,     0,     1,     1,     3,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     4,     2,     3,     1,     3,     3,     2,
       3,     2,     3,     3,     1,     3,     1,     2,     1,     1,
       1,     4,     4,     4,     4,     5,     6,     0,     9,     2,
       1,     3,     0,     8,     5,     7,     5,     7,     1,     2,
       1,     2,     1,     1,     1,     1
};


/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none).  */
static const unsigned char yydprec[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM.  */
static const unsigned char yymerger[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0
};

/* YYIMMEDIATE[RULE-NUM] -- True iff rule #RULE-NUM is not to be deferred, as
   in the case of predicates.  */
static const yybool yyimmediate[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     0,     0,     0,     0,     3,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     0,     0,     7,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,    80,     0,    80,     0,    96,     0,    94,     0
};

/* Error token number */
#define YYTERROR 1



YYSTYPE yylval;

int yynerrs;
int yychar;

static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)                              \
  do {                                          \
    YYRESULTTAG yychk_flag = YYE;               \
    if (yychk_flag != yyok)                     \
      return yychk_flag;                        \
  } while (0)

#if YYDEBUG

# ifndef YYFPRINTF
#  define YYFPRINTF fprintf
# endif

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YYDPRINTF(Args)                        \
  do {                                          \
    if (yydebug)                                \
      YYFPRINTF Args;                           \
  } while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                  \
  do {                                                                  \
    if (yydebug)                                                        \
      {                                                                 \
        YYFPRINTF (stderr, "%s ", Title);                               \
        yy_symbol_print (stderr, Type, Value);        \
        YYFPRINTF (stderr, "\n");                                       \
      }                                                                 \
  } while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

struct yyGLRStack;
static void yypstack (struct yyGLRStack* yystackp, size_t yyk)
  YY_ATTRIBUTE_UNUSED;
static void yypdumpstack (struct yyGLRStack* yystackp)
  YY_ATTRIBUTE_UNUSED;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data.  */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
#  define YYSTACKEXPANDABLE 1
#endif

#if YYSTACKEXPANDABLE
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyexpandGLRStack (Yystack);                       \
  } while (0)
#else
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyMemoryExhausted (Yystack);                      \
  } while (0)
#endif


#if YYERROR_VERBOSE

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static size_t
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return strlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* !YYERROR_VERBOSE */

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef int yySymbol;

/** Item references, as in LALR(1) machine */
typedef short int yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yyGLRStateSet yyGLRStateSet;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;

struct yyGLRState {
  /** Type tag: always true.  */
  yybool yyisState;
  /** Type tag for yysemantics.  If true, yysval applies, otherwise
   *  yyfirstVal applies.  */
  yybool yyresolved;
  /** Number of corresponding LALR(1) machine state.  */
  yyStateNum yylrState;
  /** Preceding state in this stack */
  yyGLRState* yypred;
  /** Source position of the last token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext.  */
    yySemanticOption* yyfirstVal;
    /** Semantic value for this state.  */
    YYSTYPE yysval;
  } yysemantics;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  /** During nondeterministic operation, yylookaheadNeeds tracks which
   *  stacks have actually needed the current lookahead.  During deterministic
   *  operation, yylookaheadNeeds[0] is not maintained since it would merely
   *  duplicate yychar != YYEMPTY.  */
  yybool* yylookaheadNeeds;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false.  */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced.  */
  yyGLRState* yystate;
  /** The lookahead for this reduction.  */
  int yyrawchar;
  YYSTYPE yyval;
  /** Next sibling in chain of options.  To facilitate merging,
   *  options are chained in decreasing order by address.  */
  yySemanticOption* yynext;
};

/** Type of the items in the GLR stack.  The yyisState field
 *  indicates which item of the union is valid.  */
union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrState;


  YYJMP_BUF yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  size_t yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

#if YYSTACKEXPANDABLE
static void yyexpandGLRStack (yyGLRStack* yystackp);
#endif

static _Noreturn void
yyFail (yyGLRStack* yystackp, const char* yymsg)
{
  if (yymsg != YY_NULLPTR)
    yyerror (yymsg);
  YYLONGJMP (yystackp->yyexception_buffer, 1);
}

static _Noreturn void
yyMemoryExhausted (yyGLRStack* yystackp)
{
  YYLONGJMP (yystackp->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
static inline const char*
yytokenName (yySymbol yytoken)
{
  if (yytoken == YYEMPTY)
    return "";

  return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain.  */
static void yyfillin (yyGLRStackItem *, int, int) YY_ATTRIBUTE_UNUSED;
static void
yyfillin (yyGLRStackItem *yyvsp, int yylow0, int yylow1)
{
  int i;
  yyGLRState *s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
#if YYDEBUG
      yyvsp[i].yystate.yylrState = s->yylrState;
#endif
      yyvsp[i].yystate.yyresolved = s->yyresolved;
      if (s->yyresolved)
        yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
      else
        /* The effect of using yysval or yyloc (in an immediate rule) is
         * undefined.  */
        yyvsp[i].yystate.yysemantics.yyfirstVal = YY_NULLPTR;
      s = yyvsp[i].yystate.yypred = s->yypred;
    }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
 * YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
 * For convenience, always return YYLOW1.  */
static inline int yyfill (yyGLRStackItem *, int *, int, yybool)
     YY_ATTRIBUTE_UNUSED;
static inline int
yyfill (yyGLRStackItem *yyvsp, int *yylow, int yylow1, yybool yynormal)
{
  if (!yynormal && yylow1 < *yylow)
    {
      yyfillin (yyvsp, *yylow, yylow1);
      *yylow = yylow1;
    }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$).  Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT.  */
static YYRESULTTAG
yyuserAction (yyRuleNum yyn, size_t yyrhslen, yyGLRStackItem* yyvsp,
              yyGLRStack* yystackp,
              YYSTYPE* yyvalp)
{
  yybool yynormal YY_ATTRIBUTE_UNUSED = (yystackp->yysplitPoint == YY_NULLPTR);
  int yylow;
  YYUSE (yyvalp);
  YYUSE (yyrhslen);
# undef yyerrok
# define yyerrok (yystackp->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerrok, yyerr
# undef YYRECOVERING
# define YYRECOVERING() (yystackp->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = YYEMPTY)
# undef YYFILL
# define YYFILL(N) yyfill (yyvsp, &yylow, N, yynormal)
# undef YYBACKUP
# define YYBACKUP(Token, Value)                                              \
  return yyerror (YY_("syntax error: cannot back up")),     \
         yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  switch (yyn)
    {
        case 2:
#line 92 "lang.y" /* glr.c:816  */
    {
							(((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node)->type = N_SEQUENTIAL_BLOCK;
							syntree->setRoot( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1125 "y.tab.c" /* glr.c:816  */
    break;

  case 3:
#line 97 "lang.y" /* glr.c:816  */
    {
							syntaxTree::node* n = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
							n->setType( N_SEQUENTIAL_BLOCK );
							((*yyvalp).node) = n;
						}
#line 1135 "y.tab.c" /* glr.c:816  */
    break;

  case 4:
#line 102 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_BLOCK_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1143 "y.tab.c" /* glr.c:816  */
    break;

  case 5:
#line 105 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_BLOCK_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1151 "y.tab.c" /* glr.c:816  */
    break;

  case 6:
#line 108 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1159 "y.tab.c" /* glr.c:816  */
    break;

  case 7:
#line 111 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1167 "y.tab.c" /* glr.c:816  */
    break;

  case 8:
#line 114 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1175 "y.tab.c" /* glr.c:816  */
    break;

  case 9:
#line 117 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node);
						}
#line 1183 "y.tab.c" /* glr.c:816  */
    break;

  case 10:
#line 120 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node);
						}
#line 1191 "y.tab.c" /* glr.c:816  */
    break;

  case 11:
#line 123 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node);
						}
#line 1199 "y.tab.c" /* glr.c:816  */
    break;

  case 12:
#line 126 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node);
						}
#line 1207 "y.tab.c" /* glr.c:816  */
    break;

  case 13:
#line 129 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1215 "y.tab.c" /* glr.c:816  */
    break;

  case 14:
#line 132 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1223 "y.tab.c" /* glr.c:816  */
    break;

  case 16:
#line 136 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1231 "y.tab.c" /* glr.c:816  */
    break;

  case 17:
#line 139 "lang.y" /* glr.c:816  */
    {
							yyerrok;
							((*yyvalp).node) = new syntaxTree::node( N_EMPTY );
							syntax_errors--;
							lerr << error_line() << "Unexpected semicolon" << std::endl;
						}
#line 1242 "y.tab.c" /* glr.c:816  */
    break;

  case 18:
#line 145 "lang.y" /* glr.c:816  */
    {
							lerr << error_line() << "Missing semicolon in assignment" << std::endl;
							syntax_errors--;
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1252 "y.tab.c" /* glr.c:816  */
    break;

  case 19:
#line 151 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).num) = symtab->addSymbol( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.str) );
							free( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.str) );
						}
#line 1261 "y.tab.c" /* glr.c:816  */
    break;

  case 20:
#line 156 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).typlst) = new std::vector<type_t>{ *(((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.typ) };
						}
#line 1269 "y.tab.c" /* glr.c:816  */
    break;

  case 21:
#line 159 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).typlst) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.typlst);
							((*yyvalp).typlst)->insert( ((*yyvalp).typlst)->begin(), *(((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.typ) );
						}
#line 1278 "y.tab.c" /* glr.c:816  */
    break;

  case 22:
#line 164 "lang.y" /* glr.c:816  */
    {
							lerr << error_line() << "Variadic types not yet implemented" << std::endl;
							((*yyvalp).typ) = new type_t( ERROR_TYPE );
						}
#line 1287 "y.tab.c" /* glr.c:816  */
    break;

  case 23:
#line 168 "lang.y" /* glr.c:816  */
    {
							lexer_out << "TYPENAME " << symtab->getName( (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num) ) << std::endl;
							if( (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num) == TUP_SYMBOL ) {
								type_t t = TUP_TYPE;
								for( auto itr = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.typlst)->rbegin(); itr != (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.typlst)->rend(); ++itr )
									t = t.rightFlattenTypeProduct( *itr );
								((*yyvalp).typ) = new type_t( t );
							} else {
								((*yyvalp).typ) = new type_t( scptab->getTypeDefinition( scopes.top(), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num), *(((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.typlst) ) );
								if( *((*yyvalp).typ) == ERROR_TYPE )
									lerr << error_line() << "Unknown type '" << symtab->getName( (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num) ).c_str() << "'" << std::endl;
							}
						}
#line 1305 "y.tab.c" /* glr.c:816  */
    break;

  case 24:
#line 181 "lang.y" /* glr.c:816  */
    {
							lexer_out << "TYPENAME " << symtab->getName( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.num) ) << std::endl;
							((*yyvalp).typ) = new type_t( scptab->getTypeDefinition( scopes.top(), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.num) ) );
							if( *((*yyvalp).typ) == ERROR_TYPE )
								lerr << error_line() << "Unknown type '" << symtab->getName( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.num) ).c_str() << "'" << std::endl;
						}
#line 1316 "y.tab.c" /* glr.c:816  */
    break;

  case 25:
#line 187 "lang.y" /* glr.c:816  */
    { 
							lexer_out << "TUPLE" << std::endl;
							if( (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.typlst)->size() == 1 ) {
								lerr << error_line(true) << "Cannot construct a tuple of a single type using '(x)', use 'tup(x)' instead" << std::endl;
								((*yyvalp).typ) = new type_t( (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.typlst)->back() );
							} else {
								type_t t = TUP_TYPE;
								for( auto itr = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.typlst)->rbegin(); itr != (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.typlst)->rend(); ++itr )
									t = t.rightFlattenTypeProduct( *itr );
								((*yyvalp).typ) = new type_t( t );
							}
						}
#line 1333 "y.tab.c" /* glr.c:816  */
    break;

  case 26:
#line 207 "lang.y" /* glr.c:816  */
    {
							lexer_out << "Declaring variables of type " << (*(((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.typ)) << std::endl;
							decllisttypes.push( *(((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.typ)  );
							delete (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.typ);
						}
#line 1343 "y.tab.c" /* glr.c:816  */
    break;

  case 27:
#line 211 "lang.y" /* glr.c:816  */
    {
							syntaxTree::node* n = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node); // <- 3
							if( n == nullptr )
								((*yyvalp).node) = new syntaxTree::node( N_EMPTY );
							else
								((*yyvalp).node) = n;
							decllisttypes.pop();
						}
#line 1356 "y.tab.c" /* glr.c:816  */
    break;

  case 28:
#line 220 "lang.y" /* glr.c:816  */
    {
							if( (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node) == nullptr ) {
								((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node);
							} else if( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) == nullptr ){
								((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node);
							} else {
								((*yyvalp).node) = new syntaxTree::node( N_BLOCK_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
							}
						}
#line 1370 "y.tab.c" /* glr.c:816  */
    break;

  case 29:
#line 229 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node);
						}
#line 1378 "y.tab.c" /* glr.c:816  */
    break;

  case 30:
#line 233 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_ASSIGN, symbolToVariable( (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.num), decllisttypes.top() ), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
							if( not decllisttypes.top().weaklyEqual( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node)->data_type ) )
								lerr << error_line() << "Mismatched types in assignment (" << decllisttypes.top() << " and " << (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node)->data_type << ")" << std::endl;	
						}
#line 1388 "y.tab.c" /* glr.c:816  */
    break;

  case 31:
#line 238 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_GARBAGE, nullptr, nullptr, {.integer=scptab->addVariable( scopes.top(), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.num), decllisttypes.top() ) } );
						}
#line 1396 "y.tab.c" /* glr.c:816  */
    break;

  case 32:
#line 242 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_ASSIGN, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
							if( not (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node)->data_type.weaklyEqual( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node)->data_type ) )
								lerr << error_line() << "Mismatched types in assignment (" << (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node)->data_type << " and " << (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node)->data_type << ")" << std::endl;	
						}
#line 1406 "y.tab.c" /* glr.c:816  */
    break;

  case 37:
#line 249 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1414 "y.tab.c" /* glr.c:816  */
    break;

  case 38:
#line 252 "lang.y" /* glr.c:816  */
    {
							lerr << error_line() << "Missing right parenthesis" << std::endl;
							syntax_errors--;
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1424 "y.tab.c" /* glr.c:816  */
    break;

  case 39:
#line 257 "lang.y" /* glr.c:816  */
    {
							lerr << error_line() << "Missing left parenthesis" << std::endl;
							syntax_errors--;
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1434 "y.tab.c" /* glr.c:816  */
    break;

  case 41:
#line 265 "lang.y" /* glr.c:816  */
    { ((*yyvalp).node) = nullptr; }
#line 1440 "y.tab.c" /* glr.c:816  */
    break;

  case 43:
#line 267 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_ARGUMENT_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1448 "y.tab.c" /* glr.c:816  */
    break;

  case 44:
#line 270 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_ARGUMENT_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1456 "y.tab.c" /* glr.c:816  */
    break;

  case 45:
#line 273 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node);
							lerr << error_line() << "Expected expression after comma" << std::endl;
						}
#line 1465 "y.tab.c" /* glr.c:816  */
    break;

  case 46:
#line 278 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node);
						}
#line 1473 "y.tab.c" /* glr.c:816  */
    break;

  case 47:
#line 281 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_COMPARISON_CHAIN, handleRelop( (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.num), (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) ) );
						}
#line 1481 "y.tab.c" /* glr.c:816  */
    break;

  case 49:
#line 285 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = handleRelop( (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.num), (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1489 "y.tab.c" /* glr.c:816  */
    break;

  case 51:
#line 290 "lang.y" /* glr.c:816  */
    { // associativity needs to be fixed in the syntax tree or intermediate code
							node_t type;
							int c = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.num);
							if( c == 0 )
								type = N_JOIN;
							else
								type = N_MEET;
							((*yyvalp).node) = new syntaxTree::node( type, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
							if( not (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node)->data_type.weaklyEqual( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node)->data_type ) )
								lerr << error_line() << "Cannot join/meet types " << (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node)->data_type << " and " << (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node)->data_type << std::endl;
						}
#line 1505 "y.tab.c" /* glr.c:816  */
    break;

  case 53:
#line 303 "lang.y" /* glr.c:816  */
    {
							node_t type;
							int c = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.num);
							if( c == 1 )
								type = N_ADD;
							else if( c == 0 )
								type = N_SUBTRACT;
							((*yyvalp).node) = new syntaxTree::node( type, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1519 "y.tab.c" /* glr.c:816  */
    break;

  case 55:
#line 314 "lang.y" /* glr.c:816  */
    {
							node_t type;
							int c = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.num);
							if( c == 1 )
								type = N_MULTIPLY;
							else if( c == 0 )
								type = N_DIVIDE;
							else
								type = N_REMAINDER;
							((*yyvalp).node) = new syntaxTree::node( type, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1535 "y.tab.c" /* glr.c:816  */
    break;

  case 57:
#line 328 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_INTEGER, nullptr, nullptr, {.integer=(((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.num)} );
						}
#line 1543 "y.tab.c" /* glr.c:816  */
    break;

  case 58:
#line 331 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_FLOAT, nullptr, nullptr, {.floating=(((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.flt)} );
						}
#line 1551 "y.tab.c" /* glr.c:816  */
    break;

  case 59:
#line 334 "lang.y" /* glr.c:816  */
    {
							lexer_out << "STR: \"" << input_str << "\"" << std::endl;
							syntaxTree::node::extra_data_t d;
							d.string = strdup(input_str.c_str());
							((*yyvalp).node) = new syntaxTree::node( N_STRING, nullptr, nullptr, d );
							input_str.clear();
						}
#line 1563 "y.tab.c" /* glr.c:816  */
    break;

  case 60:
#line 341 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node);
						}
#line 1571 "y.tab.c" /* glr.c:816  */
    break;

  case 63:
#line 347 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_SIZE_OF, (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node) );
						}
#line 1579 "y.tab.c" /* glr.c:816  */
    break;

  case 64:
#line 350 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_SIZE_OF, (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1587 "y.tab.c" /* glr.c:816  */
    break;

  case 65:
#line 354 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_TUPLE, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1595 "y.tab.c" /* glr.c:816  */
    break;

  case 66:
#line 358 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_TUPLE_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1603 "y.tab.c" /* glr.c:816  */
    break;

  case 67:
#line 361 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_TUPLE_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1611 "y.tab.c" /* glr.c:816  */
    break;

  case 68:
#line 365 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
						}
#line 1619 "y.tab.c" /* glr.c:816  */
    break;

  case 69:
#line 368 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_TUPLE ); // this might die
						}
#line 1627 "y.tab.c" /* glr.c:816  */
    break;

  case 70:
#line 372 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node), nullptr, {.integer=(((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->data.integer+1} );
						}
#line 1635 "y.tab.c" /* glr.c:816  */
    break;

  case 71:
#line 375 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_LIST );
						}
#line 1643 "y.tab.c" /* glr.c:816  */
    break;

  case 72:
#line 378 "lang.y" /* glr.c:816  */
    {
							lerr << error_line() << "Expected right square bracket at end of list" << std::endl;
							syntax_errors--;
							((*yyvalp).node) = new syntaxTree::node( N_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node), nullptr, {.integer=(((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->data.integer+1} );
						}
#line 1653 "y.tab.c" /* glr.c:816  */
    break;

  case 73:
#line 383 "lang.y" /* glr.c:816  */
    {
							lerr << error_line() << "Expected left square bracket at beginning of list" << std::endl;
							syntax_errors--;
							((*yyvalp).node) = new syntaxTree::node( N_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node), nullptr, {.integer=(((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->data.integer+1} );
						}
#line 1663 "y.tab.c" /* glr.c:816  */
    break;

  case 74:
#line 389 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_SINGLE_TYPE_EXPRESSION_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node), nullptr, {.integer=0} );
						}
#line 1671 "y.tab.c" /* glr.c:816  */
    break;

  case 75:
#line 392 "lang.y" /* glr.c:816  */
    {
							type_t a = (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node)->data_type, b = (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node)->data_type;
							if( a != b and a != ERROR_TYPE and b != ERROR_TYPE )
								lerr << error_line() << "Lists and sets can only contain elements of a single type" << std::endl;
							((*yyvalp).node) = new syntaxTree::node( N_SINGLE_TYPE_EXPRESSION_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node), {.integer=(((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node)->data.integer+1} );
						}
#line 1682 "y.tab.c" /* glr.c:816  */
    break;

  case 76:
#line 399 "lang.y" /* glr.c:816  */
    {
							input_str.append( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.str) );
							free( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.str) );
						}
#line 1691 "y.tab.c" /* glr.c:816  */
    break;

  case 77:
#line 403 "lang.y" /* glr.c:816  */
    {
							input_str.append( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.str) );
							free( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.str) );
						}
#line 1700 "y.tab.c" /* glr.c:816  */
    break;

  case 80:
#line 410 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = symbolToVariable( (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.num) );
						}
#line 1708 "y.tab.c" /* glr.c:816  */
    break;

  case 81:
#line 414 "lang.y" /* glr.c:816  */
    {
							lexer_out << "CALL " << symtab->getName((((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num)) << std::endl;
							/*std::cout << "function_call: " << ($<num>1) << " ";
							$<node>3->print(std::cout);
							std::cout << std::endl;*/
							syntaxTree::node* n = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
							type_t t = n ? n->data_type : VOID_TYPE;
							function_t f = scptab->getFunction( scopes.top(), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num), t );
							variable_t v = scptab->getVariable( scopes.top(), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num) );
							if( f == ERROR_FUNCTION and v == ERROR_VARIABLE ) {
								if( (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num) == TUP_SYMBOL ) {
									syntaxTree::node* traverser = n;
									while( traverser != nullptr ) {
										traverser->type = N_TUPLE_LIST;
										traverser = traverser->children[1];
									}
									if( n == nullptr ) {	// empty tuple
										((*yyvalp).node) = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=ERROR_VARIABLE} );
										lerr << error_line() << "Empty tuple not yet supported" << std::endl;
										delete n;
									} else {
										(((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->type = N_TUPLE;
										((*yyvalp).node) = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
									}
								} else {
									((*yyvalp).node) = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=ERROR_VARIABLE} );
									lerr << error_line() << "Undeclared function " << symtab->getName( (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num) ) << "(";
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
								((*yyvalp).node) = new syntaxTree::node( N_FUNCTION_CALL, (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node), nullptr, {.integer = f } );
							} else { // variable
								type_t t = scptab->getVariableType( v );
								if( t.isList() ) {
									if( n->children[1] )
										lerr << error_line(true) << "Expected 1 list index, others are ignored" << std::endl;
									((*yyvalp).node) = new syntaxTree::node( N_LIST_INDEXING, new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer = v } ), n->children[0] );
									n->children[0] = nullptr;
									delete n;
								} else if( t.isFunction() ) {
									((*yyvalp).node) = new syntaxTree::node( N_FUNCTION_CALL, (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node), new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer = v} ) );
								} else if( t.isTuple() ) {
									if( n->children[1] == nullptr /*single argument*/
											and n->children[0]->type == N_INTEGER /*constant argument*/ ) {
										((*yyvalp).node) = new syntaxTree::node( N_TUPLE_INDEXING, new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer = v } ), nullptr, {.integer = n->children[0]->data.integer } );
										delete n;
									} else {
										lerr << error_line() << "Tuples can only be indexed by constants" << std::endl;
									}									
								} else {
									lerr << error_line() << "Variable " << symtab->getName( (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num) ) << " is neither list nor function (but " << t << ")" << std::endl;
									((*yyvalp).node) = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
									delete n;
								}
							}
						}
#line 1779 "y.tab.c" /* glr.c:816  */
    break;

  case 82:
#line 480 "lang.y" /* glr.c:816  */
    {
							type_t t = (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.node)->data_type;
							if( t.isFunction() ) {
								// check args plz
								((*yyvalp).node) = new syntaxTree::node( N_FUNCTION_CALL, (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.node) );
							} else if( t.isList() ) {
								if( (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[1] )
									lerr << error_line(true) << "Expected 1 list index, others are ignored" << std::endl;
								((*yyvalp).node) = new syntaxTree::node( N_LIST_INDEXING, (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[0] );
							} else if( t.isTuple() ) {
								if( (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[1] == nullptr /*single argument*/
										and (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[0]->type == N_INTEGER /*constant argument*/ ) {
									((*yyvalp).node) = new syntaxTree::node( N_TUPLE_INDEXING, (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.node), nullptr, {.integer = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[0]->data.integer } );
									delete (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
								} else {
									lerr << error_line() << "Tuples can only be indexed by constants" << std::endl;
									((*yyvalp).node) = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
								}
							} else if( t == ERROR_TYPE ) {
								delete (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
								((*yyvalp).node)=(((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.node);
							} else {
								lerr << error_line() << "Variable is neither list nor function (but " << t << ")" << std::endl;
								((*yyvalp).node) = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
							}
						}
#line 1810 "y.tab.c" /* glr.c:816  */
    break;

  case 83:
#line 506 "lang.y" /* glr.c:816  */
    {
							if( (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[1] == nullptr /*single argument*/
									and (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[0]->type == N_INTEGER /*constant argument*/ ) {
								((*yyvalp).node) = new syntaxTree::node( N_TUPLE_INDEXING, (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.node), nullptr, {.integer = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[0]->data.integer } );
								delete (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node);
							} else {
								lerr << error_line() << "Tuples can only be indexed by constants" << std::endl;
								((*yyvalp).node) = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
							}
						}
#line 1825 "y.tab.c" /* glr.c:816  */
    break;

  case 84:
#line 516 "lang.y" /* glr.c:816  */
    {
							if( (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[1] )
								lerr << error_line(true) << "Expected 1 list index, others are ignored" << std::endl;
							((*yyvalp).node) = new syntaxTree::node( N_LIST_INDEXING, (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->children[0] );
						}
#line 1835 "y.tab.c" /* glr.c:816  */
    break;

  case 85:
#line 521 "lang.y" /* glr.c:816  */
    {
							lerr << error_line() << "Cannot construct 1-element tuples with '(x)', use 'tup(x)' instead" << std::endl;
							syntax_errors--;
							((*yyvalp).node) = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, { .integer = ERROR_VARIABLE } );
						}
#line 1845 "y.tab.c" /* glr.c:816  */
    break;

  case 86:
#line 532 "lang.y" /* glr.c:816  */
    {
							lerr << "function_definition" << std::endl;
						}
#line 1853 "y.tab.c" /* glr.c:816  */
    break;

  case 87:
#line 536 "lang.y" /* glr.c:816  */
    {
							scopes.push( previous_nested_scope );
							function_t f = scptab->addFunctionDeclaration( scopes.top(), NONE_SYMBOL, *(((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.typ), scptab->getAllVariables( scopes.top() ) );
							functions.push( f );
						}
#line 1863 "y.tab.c" /* glr.c:816  */
    break;

  case 88:
#line 540 "lang.y" /* glr.c:816  */
    {
							type_t t( FNC_STRUCTURE, { (((yyGLRStackItem const *)yyvsp)[YYFILL (-7)].yystate.yysemantics.yysval.node)->data_type, *(((yyGLRStackItem const *)yyvsp)[YYFILL (-4)].yystate.yysemantics.yysval.typ) } );
							(((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->type = N_SEQUENTIAL_BLOCK;
							((*yyvalp).node) = new syntaxTree::node( N_FUNCTION_DEFINITION, (((yyGLRStackItem const *)yyvsp)[YYFILL (-7)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node), {.integer = functions.top() } );
							functions.pop();
							((*yyvalp).node)->data_type = t;
						}
#line 1875 "y.tab.c" /* glr.c:816  */
    break;

  case 89:
#line 548 "lang.y" /* glr.c:816  */
    {
							variable_t v = scptab->addVariable( scopes.top(), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.num), *(((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.typ) ); // scoping issues
							((*yyvalp).node) = new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=v} );
						}
#line 1884 "y.tab.c" /* glr.c:816  */
    break;

  case 90:
#line 553 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_ARGUMENT_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1892 "y.tab.c" /* glr.c:816  */
    break;

  case 91:
#line 556 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_ARGUMENT_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1900 "y.tab.c" /* glr.c:816  */
    break;

  case 92:
#line 560 "lang.y" /* glr.c:816  */
    {
							type_t t = (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->data_type;
							if( not( t.isList() or t.isSet() or t == STR_TYPE ) )
								lerr << error_line() << "Cannot iterate over instance of type " << (((yyGLRStackItem const *)yyvsp)[YYFILL (-1)].yystate.yysemantics.yysval.node)->data_type << std::endl;
							if( t == STR_TYPE )
								for_iterating_variable.push( scptab->addVariable( scopes.top(), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num), UTF8CHAR_TYPE ) );
							else
								for_iterating_variable.push( scptab->addVariable( scopes.top(), (((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.num), t.getChildType() ) );
						}
#line 1914 "y.tab.c" /* glr.c:816  */
    break;

  case 93:
#line 568 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_FOR, 
								new syntaxTree::node( N_IN, 
									new syntaxTree::node( N_VARIABLE, nullptr, nullptr, {.integer=for_iterating_variable.top() } ),
									(((yyGLRStackItem const *)yyvsp)[YYFILL (-3)].yystate.yysemantics.yysval.node) ),
								(((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
							for_iterating_variable.pop();
						}
#line 1927 "y.tab.c" /* glr.c:816  */
    break;

  case 94:
#line 577 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_WHILE, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1935 "y.tab.c" /* glr.c:816  */
    break;

  case 95:
#line 580 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_WHILE, 
								(((yyGLRStackItem const *)yyvsp)[YYFILL (-4)].yystate.yysemantics.yysval.node), 
								new syntaxTree::node( N_ELSE, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) )
							);
						}
#line 1946 "y.tab.c" /* glr.c:816  */
    break;

  case 96:
#line 587 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_IF, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1954 "y.tab.c" /* glr.c:816  */
    break;

  case 97:
#line 590 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_IF, (((yyGLRStackItem const *)yyvsp)[YYFILL (-4)].yystate.yysemantics.yysval.node), new syntaxTree::node( N_ELSE, (((yyGLRStackItem const *)yyvsp)[YYFILL (-2)].yystate.yysemantics.yysval.node), (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) ) );
						}
#line 1962 "y.tab.c" /* glr.c:816  */
    break;

  case 98:
#line 594 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_BREAK, nullptr, nullptr, {.integer=1} );
						}
#line 1970 "y.tab.c" /* glr.c:816  */
    break;

  case 99:
#line 597 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_BREAK, nullptr, nullptr, {.integer=(((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.num)} );
						}
#line 1978 "y.tab.c" /* glr.c:816  */
    break;

  case 100:
#line 601 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_CONTINUE );
						}
#line 1986 "y.tab.c" /* glr.c:816  */
    break;

  case 101:
#line 605 "lang.y" /* glr.c:816  */
    {
							((*yyvalp).node) = new syntaxTree::node( N_RETURN, (((yyGLRStackItem const *)yyvsp)[YYFILL (0)].yystate.yysemantics.yysval.node) );
						}
#line 1994 "y.tab.c" /* glr.c:816  */
    break;

  case 102:
#line 609 "lang.y" /* glr.c:816  */
    {
							scopes.push( scptab->addScope( scopes.top() ) ); // do such brackets realy need to be a scope?
						}
#line 2002 "y.tab.c" /* glr.c:816  */
    break;

  case 103:
#line 613 "lang.y" /* glr.c:816  */
    {
							previous_nested_scope = scopes.top();
							scopes.pop();
						}
#line 2011 "y.tab.c" /* glr.c:816  */
    break;

  case 104:
#line 618 "lang.y" /* glr.c:816  */
    {
							scopes.push( scptab->addScope( scopes.top() ) );
						}
#line 2019 "y.tab.c" /* glr.c:816  */
    break;

  case 105:
#line 622 "lang.y" /* glr.c:816  */
    {
							previous_nested_scope = scopes.top();
							scopes.pop();
						}
#line 2028 "y.tab.c" /* glr.c:816  */
    break;


#line 2032 "y.tab.c" /* glr.c:816  */
      default: break;
    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
}


static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  YYUSE (yy0);
  YYUSE (yy1);

  switch (yyn)
    {

      default: break;
    }
}

                              /* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

static void
yydestroyGLRState (char const *yymsg, yyGLRState *yys)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
                &yys->yysemantics.yysval);
  else
    {
#if YYDEBUG
      if (yydebug)
        {
          if (yys->yysemantics.yyfirstVal)
            YYFPRINTF (stderr, "%s unresolved", yymsg);
          else
            YYFPRINTF (stderr, "%s incomplete", yymsg);
          YY_SYMBOL_PRINT ("", yystos[yys->yylrState], YY_NULLPTR, &yys->yyloc);
        }
#endif

      if (yys->yysemantics.yyfirstVal)
        {
          yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
          yyGLRState *yyrh;
          int yyn;
          for (yyrh = yyoption->yystate, yyn = yyrhsLength (yyoption->yyrule);
               yyn > 0;
               yyrh = yyrh->yypred, yyn -= 1)
            yydestroyGLRState (yymsg, yyrh);
        }
    }
}

/** Left-hand-side symbol for rule #YYRULE.  */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-117)))

/** True iff LR state YYSTATE has only a default reduction (regardless
 *  of token).  */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yypact_value_is_default (yypact[yystate]);
}

/** The default reduction for YYSTATE, assuming it has one.  */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yytable_value_is_error(Yytable_value) \
  0

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *YYCONFLICTS to a pointer into yyconfl to a 0-terminated list
 *  of conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
                int* yyaction, const short int** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yypact_value_is_default (yypact[yystate])
      || yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yytable_value_is_error (yytable[yyindex]))
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
  else
    {
      *yyaction = 0;
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

/** Compute post-reduction state.
 * \param yystate   the current state
 * \param yysym     the nonterminal to push on the stack
 */
static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yysym)
{
  int yyr = yypgoto[yysym - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yysym - YYNTOKENS];
}

static inline yybool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline yybool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

                                /* GLRStates */

/** Return a fresh GLRStackItem in YYSTACKP.  The item is an LR state
 *  if YYISSTATE, and otherwise a semantic option.  Callers should call
 *  YY_RESERVE_GLRSTACK afterwards to make sure there is sufficient
 *  headroom.  */

static inline yyGLRStackItem*
yynewGLRStackItem (yyGLRStack* yystackp, yybool yyisState)
{
  yyGLRStackItem* yynewItem = yystackp->yynextFree;
  yystackp->yyspaceLeft -= 1;
  yystackp->yynextFree += 1;
  yynewItem->yystate.yyisState = yyisState;
  return yynewItem;
}

/** Add a new semantic action that will execute the action for rule
 *  YYRULE on the semantic values in YYRHS to the list of
 *  alternative actions for YYSTATE.  Assumes that YYRHS comes from
 *  stack #YYK of *YYSTACKP. */
static void
yyaddDeferredAction (yyGLRStack* yystackp, size_t yyk, yyGLRState* yystate,
                     yyGLRState* yyrhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewOption =
    &yynewGLRStackItem (yystackp, yyfalse)->yyoption;
  YYASSERT (!yynewOption->yyisState);
  yynewOption->yystate = yyrhs;
  yynewOption->yyrule = yyrule;
  if (yystackp->yytops.yylookaheadNeeds[yyk])
    {
      yynewOption->yyrawchar = yychar;
      yynewOption->yyval = yylval;
    }
  else
    yynewOption->yyrawchar = YYEMPTY;
  yynewOption->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewOption;

  YY_RESERVE_GLRSTACK (yystackp);
}

                                /* GLRStacks */

/** Initialize YYSET to a singleton set containing an empty stack.  */
static yybool
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  if (! yyset->yystates)
    return yyfalse;
  yyset->yystates[0] = YY_NULLPTR;
  yyset->yylookaheadNeeds =
    (yybool*) YYMALLOC (16 * sizeof yyset->yylookaheadNeeds[0]);
  if (! yyset->yylookaheadNeeds)
    {
      YYFREE (yyset->yystates);
      return yyfalse;
    }
  return yytrue;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
  YYFREE (yyset->yylookaheadNeeds);
}

/** Initialize *YYSTACKP to a single empty stack, with total maximum
 *  capacity for all stacks of YYSIZE.  */
static yybool
yyinitGLRStack (yyGLRStack* yystackp, size_t yysize)
{
  yystackp->yyerrState = 0;
  yynerrs = 0;
  yystackp->yyspaceLeft = yysize;
  yystackp->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystackp->yynextFree[0]);
  if (!yystackp->yyitems)
    return yyfalse;
  yystackp->yynextFree = yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULLPTR;
  yystackp->yylastDeleted = YY_NULLPTR;
  return yyinitStateSet (&yystackp->yytops);
}


#if YYSTACKEXPANDABLE
# define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If *YYSTACKP is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation.  */
static void
yyexpandGLRStack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yynewItems;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yynewSize;
  size_t yyn;
  size_t yysize = yystackp->yynextFree - yystackp->yyitems;
  if (YYMAXDEPTH - YYHEADROOM < yysize)
    yyMemoryExhausted (yystackp);
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem*) YYMALLOC (yynewSize * sizeof yynewItems[0]);
  if (! yynewItems)
    yyMemoryExhausted (yystackp);
  for (yyp0 = yystackp->yyitems, yyp1 = yynewItems, yyn = yysize;
       0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(yybool *) yyp0)
        {
          yyGLRState* yys0 = &yyp0->yystate;
          yyGLRState* yys1 = &yyp1->yystate;
          if (yys0->yypred != YY_NULLPTR)
            yys1->yypred =
              YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
          if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != YY_NULLPTR)
            yys1->yysemantics.yyfirstVal =
              YYRELOC (yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
        }
      else
        {
          yySemanticOption* yyv0 = &yyp0->yyoption;
          yySemanticOption* yyv1 = &yyp1->yyoption;
          if (yyv0->yystate != YY_NULLPTR)
            yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
          if (yyv0->yynext != YY_NULLPTR)
            yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
        }
    }
  if (yystackp->yysplitPoint != YY_NULLPTR)
    yystackp->yysplitPoint = YYRELOC (yystackp->yyitems, yynewItems,
                                      yystackp->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystackp->yytops.yysize; yyn += 1)
    if (yystackp->yytops.yystates[yyn] != YY_NULLPTR)
      yystackp->yytops.yystates[yyn] =
        YYRELOC (yystackp->yyitems, yynewItems,
                 yystackp->yytops.yystates[yyn], yystate);
  YYFREE (yystackp->yyitems);
  yystackp->yyitems = yynewItems;
  yystackp->yynextFree = yynewItems + yysize;
  yystackp->yyspaceLeft = yynewSize - yysize;
}
#endif

static void
yyfreeGLRStack (yyGLRStack* yystackp)
{
  YYFREE (yystackp->yyitems);
  yyfreeStateSet (&yystackp->yytops);
}

/** Assuming that YYS is a GLRState somewhere on *YYSTACKP, update the
 *  splitpoint of *YYSTACKP, if needed, so that it is at least as deep as
 *  YYS.  */
static inline void
yyupdateSplit (yyGLRStack* yystackp, yyGLRState* yys)
{
  if (yystackp->yysplitPoint != YY_NULLPTR && yystackp->yysplitPoint > yys)
    yystackp->yysplitPoint = yys;
}

/** Invalidate stack #YYK in *YYSTACKP.  */
static inline void
yymarkStackDeleted (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yytops.yystates[yyk] != YY_NULLPTR)
    yystackp->yylastDeleted = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yystates[yyk] = YY_NULLPTR;
}

/** Undelete the last stack in *YYSTACKP that was marked as deleted.  Can
    only be done once after a deletion, and only when all other stacks have
    been deleted.  */
static void
yyundeleteLastStack (yyGLRStack* yystackp)
{
  if (yystackp->yylastDeleted == YY_NULLPTR || yystackp->yytops.yysize != 0)
    return;
  yystackp->yytops.yystates[0] = yystackp->yylastDeleted;
  yystackp->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystackp->yylastDeleted = YY_NULLPTR;
}

static inline void
yyremoveDeletes (yyGLRStack* yystackp)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystackp->yytops.yysize)
    {
      if (yystackp->yytops.yystates[yyi] == YY_NULLPTR)
        {
          if (yyi == yyj)
            {
              YYDPRINTF ((stderr, "Removing dead stacks.\n"));
            }
          yystackp->yytops.yysize -= 1;
        }
      else
        {
          yystackp->yytops.yystates[yyj] = yystackp->yytops.yystates[yyi];
          /* In the current implementation, it's unnecessary to copy
             yystackp->yytops.yylookaheadNeeds[yyi] since, after
             yyremoveDeletes returns, the parser immediately either enters
             deterministic operation or shifts a token.  However, it doesn't
             hurt, and the code might evolve to need it.  */
          yystackp->yytops.yylookaheadNeeds[yyj] =
            yystackp->yytops.yylookaheadNeeds[yyi];
          if (yyj != yyi)
            {
              YYDPRINTF ((stderr, "Rename stack %lu -> %lu.\n",
                          (unsigned long int) yyi, (unsigned long int) yyj));
            }
          yyj += 1;
        }
      yyi += 1;
    }
}

/** Shift to a new state on stack #YYK of *YYSTACKP, corresponding to LR
 * state YYLRSTATE, at input position YYPOSN, with (resolved) semantic
 * value *YYVALP and source location *YYLOCP.  */
static inline void
yyglrShift (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
            size_t yyposn,
            YYSTYPE* yyvalp)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yytrue;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yysval = *yyvalp;
  yystackp->yytops.yystates[yyk] = yynewState;

  YY_RESERVE_GLRSTACK (yystackp);
}

/** Shift stack #YYK of *YYSTACKP, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE.  */
static inline void
yyglrShiftDefer (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
                 size_t yyposn, yyGLRState* yyrhs, yyRuleNum yyrule)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;
  YYASSERT (yynewState->yyisState);

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yyfalse;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yyfirstVal = YY_NULLPTR;
  yystackp->yytops.yystates[yyk] = yynewState;

  /* Invokes YY_RESERVE_GLRSTACK.  */
  yyaddDeferredAction (yystackp, yyk, yynewState, yyrhs, yyrule);
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(Args)
#else
# define YY_REDUCE_PRINT(Args)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print Args;               \
} while (0)

/*----------------------------------------------------------------------.
| Report that stack #YYK of *YYSTACKP is going to be reduced by YYRULE. |
`----------------------------------------------------------------------*/

static inline void
yy_reduce_print (int yynormal, yyGLRStackItem* yyvsp, size_t yyk,
                 yyRuleNum yyrule)
{
  int yynrhs = yyrhsLength (yyrule);
  int yyi;
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu):\n",
             (unsigned long int) yyk, yyrule - 1,
             (unsigned long int) yyrline[yyrule]);
  if (! yynormal)
    yyfillin (yyvsp, 1, -yynrhs);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyvsp[yyi - yynrhs + 1].yystate.yylrState],
                       &yyvsp[yyi - yynrhs + 1].yystate.yysemantics.yysval
                                              );
      if (!yyvsp[yyi - yynrhs + 1].yystate.yyresolved)
        YYFPRINTF (stderr, " (unresolved)");
      YYFPRINTF (stderr, "\n");
    }
}
#endif

/** Pop the symbols consumed by reduction #YYRULE from the top of stack
 *  #YYK of *YYSTACKP, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved.  Set *YYVALP to the resulting value,
 *  and *YYLOCP to the computed location (if any).  Return value is as
 *  for userAction.  */
static inline YYRESULTTAG
yydoAction (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
            YYSTYPE* yyvalp)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystackp->yysplitPoint == YY_NULLPTR)
    {
      /* Standard special case: single stack.  */
      yyGLRStackItem* yyrhs = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystackp->yynextFree -= yynrhs;
      yystackp->yyspaceLeft += yynrhs;
      yystackp->yytops.yystates[0] = & yystackp->yynextFree[-1].yystate;
      YY_REDUCE_PRINT ((1, yyrhs, yyk, yyrule));
      return yyuserAction (yyrule, yynrhs, yyrhs, yystackp,
                           yyvalp);
    }
  else
    {
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
      yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred
        = yystackp->yytops.yystates[yyk];
      for (yyi = 0; yyi < yynrhs; yyi += 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yystackp->yytops.yystates[yyk] = yys;
      YY_REDUCE_PRINT ((0, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1, yyk, yyrule));
      return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp);
    }
}

/** Pop items off stack #YYK of *YYSTACKP according to grammar rule YYRULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with YYRULE and store its value with the
 *  newly pushed state, if YYFORCEEVAL or if *YYSTACKP is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #YYK from
 *  *YYSTACKP.  In this case, the semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
             yybool yyforceEval)
{
  size_t yyposn = yystackp->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystackp->yysplitPoint == YY_NULLPTR)
    {
      YYSTYPE yysval;

      YYRESULTTAG yyflag = yydoAction (yystackp, yyk, yyrule, &yysval);
      if (yyflag == yyerr && yystackp->yysplitPoint != YY_NULLPTR)
        {
          YYDPRINTF ((stderr, "Parse on stack %lu rejected by rule #%d.\n",
                     (unsigned long int) yyk, yyrule - 1));
        }
      if (yyflag != yyok)
        return yyflag;
      YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyrule], &yysval, &yyloc);
      yyglrShift (yystackp, yyk,
                  yyLRgotoState (yystackp->yytops.yystates[yyk]->yylrState,
                                 yylhsNonterm (yyrule)),
                  yyposn, &yysval);
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystackp->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystackp->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
           0 < yyn; yyn -= 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
                  "Reduced stack %lu by rule #%d; action deferred.  "
                  "Now in state %d.\n",
                  (unsigned long int) yyk, yyrule - 1, yynewLRState));
      for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
        if (yyi != yyk && yystackp->yytops.yystates[yyi] != YY_NULLPTR)
          {
            yyGLRState *yysplit = yystackp->yysplitPoint;
            yyGLRState *yyp = yystackp->yytops.yystates[yyi];
            while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
              {
                if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
                  {
                    yyaddDeferredAction (yystackp, yyk, yyp, yys0, yyrule);
                    yymarkStackDeleted (yystackp, yyk);
                    YYDPRINTF ((stderr, "Merging stack %lu into stack %lu.\n",
                                (unsigned long int) yyk,
                                (unsigned long int) yyi));
                    return yyok;
                  }
                yyp = yyp->yypred;
              }
          }
      yystackp->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystackp, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return yyok;
}

static size_t
yysplitStack (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yysplitPoint == YY_NULLPTR)
    {
      YYASSERT (yyk == 0);
      yystackp->yysplitPoint = yystackp->yytops.yystates[yyk];
    }
  if (yystackp->yytops.yysize >= yystackp->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      yybool* yynewLookaheadNeeds;

      yynewStates = YY_NULLPTR;

      if (yystackp->yytops.yycapacity
          > (YYSIZEMAX / (2 * sizeof yynewStates[0])))
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yycapacity *= 2;

      yynewStates =
        (yyGLRState**) YYREALLOC (yystackp->yytops.yystates,
                                  (yystackp->yytops.yycapacity
                                   * sizeof yynewStates[0]));
      if (yynewStates == YY_NULLPTR)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yystates = yynewStates;

      yynewLookaheadNeeds =
        (yybool*) YYREALLOC (yystackp->yytops.yylookaheadNeeds,
                             (yystackp->yytops.yycapacity
                              * sizeof yynewLookaheadNeeds[0]));
      if (yynewLookaheadNeeds == YY_NULLPTR)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yylookaheadNeeds = yynewLookaheadNeeds;
    }
  yystackp->yytops.yystates[yystackp->yytops.yysize]
    = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yylookaheadNeeds[yystackp->yytops.yysize]
    = yystackp->yytops.yylookaheadNeeds[yyk];
  yystackp->yytops.yysize += 1;
  return yystackp->yytops.yysize-1;
}

/** True iff YYY0 and YYY1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols.  */
static yybool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
           yyn = yyrhsLength (yyy0->yyrule);
           yyn > 0;
           yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
        if (yys0->yyposn != yys1->yyposn)
          return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (YYY0,YYY1), destructively merge the
 *  alternative semantic values for the RHS-symbols of YYY1 and YYY0.  */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    {
      if (yys0 == yys1)
        break;
      else if (yys0->yyresolved)
        {
          yys1->yyresolved = yytrue;
          yys1->yysemantics.yysval = yys0->yysemantics.yysval;
        }
      else if (yys1->yyresolved)
        {
          yys0->yyresolved = yytrue;
          yys0->yysemantics.yysval = yys1->yysemantics.yysval;
        }
      else
        {
          yySemanticOption** yyz0p = &yys0->yysemantics.yyfirstVal;
          yySemanticOption* yyz1 = yys1->yysemantics.yyfirstVal;
          while (yytrue)
            {
              if (yyz1 == *yyz0p || yyz1 == YY_NULLPTR)
                break;
              else if (*yyz0p == YY_NULLPTR)
                {
                  *yyz0p = yyz1;
                  break;
                }
              else if (*yyz0p < yyz1)
                {
                  yySemanticOption* yyz = *yyz0p;
                  *yyz0p = yyz1;
                  yyz1 = yyz1->yynext;
                  (*yyz0p)->yynext = yyz;
                }
              yyz0p = &(*yyz0p)->yynext;
            }
          yys1->yysemantics.yyfirstVal = yys0->yysemantics.yyfirstVal;
        }
    }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred.  */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
        return 0;
      else
        return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yyGLRState* yys,
                                   yyGLRStack* yystackp);


/** Resolve the previous YYN states starting at and including state YYS
 *  on *YYSTACKP. If result != yyok, some states may have been left
 *  unresolved possibly with empty semantic option chains.  Regardless
 *  of whether result = yyok, each state has been left with consistent
 *  data so that yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn,
                 yyGLRStack* yystackp)
{
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      YYCHK (yyresolveStates (yys->yypred, yyn-1, yystackp));
      if (! yys->yyresolved)
        YYCHK (yyresolveValue (yys, yystackp));
    }
  return yyok;
}

/** Resolve the states for the RHS of YYOPT on *YYSTACKP, perform its
 *  user action, and return the semantic value and location in *YYVALP
 *  and *YYLOCP.  Regardless of whether result = yyok, all RHS states
 *  have been destroyed (assuming the user action destroys all RHS
 *  semantic values if invoked).  */
static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystackp,
                 YYSTYPE* yyvalp)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs = yyrhsLength (yyopt->yyrule);
  YYRESULTTAG yyflag =
    yyresolveStates (yyopt->yystate, yynrhs, yystackp);
  if (yyflag != yyok)
    {
      yyGLRState *yys;
      for (yys = yyopt->yystate; yynrhs > 0; yys = yys->yypred, yynrhs -= 1)
        yydestroyGLRState ("Cleanup: popping", yys);
      return yyflag;
    }

  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  {
    int yychar_current = yychar;
    YYSTYPE yylval_current = yylval;
    yychar = yyopt->yyrawchar;
    yylval = yyopt->yyval;
    yyflag = yyuserAction (yyopt->yyrule, yynrhs,
                           yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp);
    yychar = yychar_current;
    yylval = yylval_current;
  }
  return yyflag;
}

#if YYDEBUG
static void
yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[1 + YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == YY_NULLPTR)
    {
      yyleftmost_state.yyposn = 0;
      yystates[0] = &yyleftmost_state;
    }
  else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1, (unsigned long int) (yys->yyposn + 1),
               (unsigned long int) yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
        {
          if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
            YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
                       yytokenName (yystos[yystates[yyi]->yylrState]));
          else
            YYFPRINTF (stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent+2, "",
                       yytokenName (yystos[yystates[yyi]->yylrState]),
                       (unsigned long int) (yystates[yyi-1]->yyposn + 1),
                       (unsigned long int) yystates[yyi]->yyposn);
        }
      else
        yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

static YYRESULTTAG
yyreportAmbiguity (yySemanticOption* yyx0,
                   yySemanticOption* yyx1)
{
  YYUSE (yyx0);
  YYUSE (yyx1);

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif

  yyerror (YY_("syntax is ambiguous"));
  return yyabort;
}

/** Resolve the ambiguity represented in state YYS in *YYSTACKP,
 *  perform the indicated actions, and set the semantic value of YYS.
 *  If result != yyok, the chain of semantic options in YYS has been
 *  cleared instead or it has been left unmodified except that
 *  redundant options may have been removed.  Regardless of whether
 *  result = yyok, YYS has been left with consistent data so that
 *  yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveValue (yyGLRState* yys, yyGLRStack* yystackp)
{
  yySemanticOption* yyoptionList = yys->yysemantics.yyfirstVal;
  yySemanticOption* yybest = yyoptionList;
  yySemanticOption** yypp;
  yybool yymerge = yyfalse;
  YYSTYPE yysval;
  YYRESULTTAG yyflag;

  for (yypp = &yyoptionList->yynext; *yypp != YY_NULLPTR; )
    {
      yySemanticOption* yyp = *yypp;

      if (yyidenticalOptions (yybest, yyp))
        {
          yymergeOptionSets (yybest, yyp);
          *yypp = yyp->yynext;
        }
      else
        {
          switch (yypreference (yybest, yyp))
            {
            case 0:
              return yyreportAmbiguity (yybest, yyp);
              break;
            case 1:
              yymerge = yytrue;
              break;
            case 2:
              break;
            case 3:
              yybest = yyp;
              yymerge = yyfalse;
              break;
            default:
              /* This cannot happen so it is not worth a YYASSERT (yyfalse),
                 but some compilers complain if the default case is
                 omitted.  */
              break;
            }
          yypp = &yyp->yynext;
        }
    }

  if (yymerge)
    {
      yySemanticOption* yyp;
      int yyprec = yydprec[yybest->yyrule];
      yyflag = yyresolveAction (yybest, yystackp, &yysval);
      if (yyflag == yyok)
        for (yyp = yybest->yynext; yyp != YY_NULLPTR; yyp = yyp->yynext)
          {
            if (yyprec == yydprec[yyp->yyrule])
              {
                YYSTYPE yysval_other;
                yyflag = yyresolveAction (yyp, yystackp, &yysval_other);
                if (yyflag != yyok)
                  {
                    yydestruct ("Cleanup: discarding incompletely merged value for",
                                yystos[yys->yylrState],
                                &yysval);
                    break;
                  }
                yyuserMerge (yymerger[yyp->yyrule], &yysval, &yysval_other);
              }
          }
    }
  else
    yyflag = yyresolveAction (yybest, yystackp, &yysval);

  if (yyflag == yyok)
    {
      yys->yyresolved = yytrue;
      yys->yysemantics.yysval = yysval;
    }
  else
    yys->yysemantics.yyfirstVal = YY_NULLPTR;
  return yyflag;
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystackp)
{
  if (yystackp->yysplitPoint != YY_NULLPTR)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystackp->yytops.yystates[0];
           yys != yystackp->yysplitPoint;
           yys = yys->yypred, yyn += 1)
        continue;
      YYCHK (yyresolveStates (yystackp->yytops.yystates[0], yyn, yystackp
                             ));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystackp)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystackp->yytops.yysize != 1 || yystackp->yysplitPoint == YY_NULLPTR)
    return;

  for (yyp = yystackp->yytops.yystates[0], yyq = yyp->yypred, yyr = YY_NULLPTR;
       yyp != yystackp->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystackp->yyspaceLeft += yystackp->yynextFree - yystackp->yyitems;
  yystackp->yynextFree = ((yyGLRStackItem*) yystackp->yysplitPoint) + 1;
  yystackp->yyspaceLeft -= yystackp->yynextFree - yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULLPTR;
  yystackp->yylastDeleted = YY_NULLPTR;

  while (yyr != YY_NULLPTR)
    {
      yystackp->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystackp->yynextFree->yystate.yypred = &yystackp->yynextFree[-1].yystate;
      yystackp->yytops.yystates[0] = &yystackp->yynextFree->yystate;
      yystackp->yynextFree += 1;
      yystackp->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystackp, size_t yyk,
                   size_t yyposn)
{
  while (yystackp->yytops.yystates[yyk] != YY_NULLPTR)
    {
      yyStateNum yystate = yystackp->yytops.yystates[yyk]->yylrState;
      YYDPRINTF ((stderr, "Stack %lu Entering state %d\n",
                  (unsigned long int) yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
        {
          YYRESULTTAG yyflag;
          yyRuleNum yyrule = yydefaultAction (yystate);
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              return yyok;
            }
          yyflag = yyglrReduce (yystackp, yyk, yyrule, yyimmediate[yyrule]);
          if (yyflag == yyerr)
            {
              YYDPRINTF ((stderr,
                          "Stack %lu dies "
                          "(predicate failure or explicit user error).\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              return yyok;
            }
          if (yyflag != yyok)
            return yyflag;
        }
      else
        {
          yySymbol yytoken;
          int yyaction;
          const short int* yyconflicts;

          yystackp->yytops.yylookaheadNeeds[yyk] = yytrue;
          if (yychar == YYEMPTY)
            {
              YYDPRINTF ((stderr, "Reading a token: "));
              yychar = yylex ();
            }

          if (yychar <= YYEOF)
            {
              yychar = yytoken = YYEOF;
              YYDPRINTF ((stderr, "Now at end of input.\n"));
            }
          else
            {
              yytoken = YYTRANSLATE (yychar);
              YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
            }

          yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);

          while (*yyconflicts != 0)
            {
              YYRESULTTAG yyflag;
              size_t yynewStack = yysplitStack (yystackp, yyk);
              YYDPRINTF ((stderr, "Splitting off stack %lu from %lu.\n",
                          (unsigned long int) yynewStack,
                          (unsigned long int) yyk));
              yyflag = yyglrReduce (yystackp, yynewStack,
                                    *yyconflicts,
                                    yyimmediate[*yyconflicts]);
              if (yyflag == yyok)
                YYCHK (yyprocessOneStack (yystackp, yynewStack,
                                          yyposn));
              else if (yyflag == yyerr)
                {
                  YYDPRINTF ((stderr, "Stack %lu dies.\n",
                              (unsigned long int) yynewStack));
                  yymarkStackDeleted (yystackp, yynewStack);
                }
              else
                return yyflag;
              yyconflicts += 1;
            }

          if (yyisShiftAction (yyaction))
            break;
          else if (yyisErrorAction (yyaction))
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              break;
            }
          else
            {
              YYRESULTTAG yyflag = yyglrReduce (yystackp, yyk, -yyaction,
                                                yyimmediate[-yyaction]);
              if (yyflag == yyerr)
                {
                  YYDPRINTF ((stderr,
                              "Stack %lu dies "
                              "(predicate failure or explicit user error).\n",
                              (unsigned long int) yyk));
                  yymarkStackDeleted (yystackp, yyk);
                  break;
                }
              else if (yyflag != yyok)
                return yyflag;
            }
        }
    }
  return yyok;
}

static void
yyreportSyntaxError (yyGLRStack* yystackp)
{
  if (yystackp->yyerrState != 0)
    return;
#if ! YYERROR_VERBOSE
  yyerror (YY_("syntax error"));
#else
  {
  yySymbol yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);
  size_t yysize0 = yytnamerr (YY_NULLPTR, yytokenName (yytoken));
  size_t yysize = yysize0;
  yybool yysize_overflow = yyfalse;
  char* yymsg = YY_NULLPTR;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected").  */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[yystackp->yytops.yystates[0]->yylrState];
      yyarg[yycount++] = yytokenName (yytoken);
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for this
             state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;
          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytokenName (yyx);
                {
                  size_t yysz = yysize + yytnamerr (YY_NULLPTR, yytokenName (yyx));
                  yysize_overflow |= yysz < yysize;
                  yysize = yysz;
                }
              }
        }
    }

  switch (yycount)
    {
#define YYCASE_(N, S)                   \
      case N:                           \
        yyformat = S;                   \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  {
    size_t yysz = yysize + strlen (yyformat);
    yysize_overflow |= yysz < yysize;
    yysize = yysz;
  }

  if (!yysize_overflow)
    yymsg = (char *) YYMALLOC (yysize);

  if (yymsg)
    {
      char *yyp = yymsg;
      int yyi = 0;
      while ((*yyp = *yyformat))
        {
          if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
            {
              yyp += yytnamerr (yyp, yyarg[yyi++]);
              yyformat += 2;
            }
          else
            {
              yyp++;
              yyformat++;
            }
        }
      yyerror (yymsg);
      YYFREE (yymsg);
    }
  else
    {
      yyerror (YY_("syntax error"));
      yyMemoryExhausted (yystackp);
    }
  }
#endif /* YYERROR_VERBOSE */
  yynerrs += 1;
}

/* Recover from a syntax error on *YYSTACKP, assuming that *YYSTACKP->YYTOKENP,
   yylval, and yylloc are the syntactic category, semantic value, and location
   of the lookahead.  */
static void
yyrecoverSyntaxError (yyGLRStack* yystackp)
{
  size_t yyk;
  int yyj;

  if (yystackp->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (yytrue)
      {
        yySymbol yytoken;
        if (yychar == YYEOF)
          yyFail (yystackp, YY_NULLPTR);
        if (yychar != YYEMPTY)
          {
            yytoken = YYTRANSLATE (yychar);
            yydestruct ("Error: discarding",
                        yytoken, &yylval);
          }
        YYDPRINTF ((stderr, "Reading a token: "));
        yychar = yylex ();
        if (yychar <= YYEOF)
          {
            yychar = yytoken = YYEOF;
            YYDPRINTF ((stderr, "Now at end of input.\n"));
          }
        else
          {
            yytoken = YYTRANSLATE (yychar);
            YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
          }
        yyj = yypact[yystackp->yytops.yystates[0]->yylrState];
        if (yypact_value_is_default (yyj))
          return;
        yyj += yytoken;
        if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken)
          {
            if (yydefact[yystackp->yytops.yystates[0]->yylrState] != 0)
              return;
          }
        else if (! yytable_value_is_error (yytable[yyj]))
          return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystackp->yytops.yysize; yyk += 1)
    if (yystackp->yytops.yystates[yyk] != YY_NULLPTR)
      break;
  if (yyk >= yystackp->yytops.yysize)
    yyFail (yystackp, YY_NULLPTR);
  for (yyk += 1; yyk < yystackp->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystackp, yyk);
  yyremoveDeletes (yystackp);
  yycompressStack (yystackp);

  /* Now pop stack until we find a state that shifts the error token.  */
  yystackp->yyerrState = 3;
  while (yystackp->yytops.yystates[0] != YY_NULLPTR)
    {
      yyGLRState *yys = yystackp->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yypact_value_is_default (yyj))
        {
          yyj += YYTERROR;
          if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
              && yyisShiftAction (yytable[yyj]))
            {
              /* Shift the error token.  */
              YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
                               &yylval, &yyerrloc);
              yyglrShift (yystackp, 0, yytable[yyj],
                          yys->yyposn, &yylval);
              yys = yystackp->yytops.yystates[0];
              break;
            }
        }
      if (yys->yypred != YY_NULLPTR)
        yydestroyGLRState ("Error: popping", yys);
      yystackp->yytops.yystates[0] = yys->yypred;
      yystackp->yynextFree -= 1;
      yystackp->yyspaceLeft += 1;
    }
  if (yystackp->yytops.yystates[0] == YY_NULLPTR)
    yyFail (yystackp, YY_NULLPTR);
}

#define YYCHK1(YYE)                                                          \
  do {                                                                       \
    switch (YYE) {                                                           \
    case yyok:                                                               \
      break;                                                                 \
    case yyabort:                                                            \
      goto yyabortlab;                                                       \
    case yyaccept:                                                           \
      goto yyacceptlab;                                                      \
    case yyerr:                                                              \
      goto yyuser_error;                                                     \
    default:                                                                 \
      goto yybuglab;                                                         \
    }                                                                        \
  } while (0)

/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
  int yyresult;
  yyGLRStack yystack;
  yyGLRStack* const yystackp = &yystack;
  size_t yyposn;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY;
  yylval = yyval_default;

  if (! yyinitGLRStack (yystackp, YYINITDEPTH))
    goto yyexhaustedlab;
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yyglrShift (&yystack, 0, 0, 0, &yylval);
  yyposn = 0;

  while (yytrue)
    {
      /* For efficiency, we have two loops, the first of which is
         specialized to deterministic operation (single stack, no
         potential ambiguity).  */
      /* Standard mode */
      while (yytrue)
        {
          yyRuleNum yyrule;
          int yyaction;
          const short int* yyconflicts;

          yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
          YYDPRINTF ((stderr, "Entering state %d\n", yystate));
          if (yystate == YYFINAL)
            goto yyacceptlab;
          if (yyisDefaultedState (yystate))
            {
              yyrule = yydefaultAction (yystate);
              if (yyrule == 0)
                {

                  yyreportSyntaxError (&yystack);
                  goto yyuser_error;
                }
              YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue));
            }
          else
            {
              yySymbol yytoken;
              if (yychar == YYEMPTY)
                {
                  YYDPRINTF ((stderr, "Reading a token: "));
                  yychar = yylex ();
                }

              if (yychar <= YYEOF)
                {
                  yychar = yytoken = YYEOF;
                  YYDPRINTF ((stderr, "Now at end of input.\n"));
                }
              else
                {
                  yytoken = YYTRANSLATE (yychar);
                  YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
                }

              yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
              if (*yyconflicts != 0)
                break;
              if (yyisShiftAction (yyaction))
                {
                  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
                  yychar = YYEMPTY;
                  yyposn += 1;
                  yyglrShift (&yystack, 0, yyaction, yyposn, &yylval);
                  if (0 < yystack.yyerrState)
                    yystack.yyerrState -= 1;
                }
              else if (yyisErrorAction (yyaction))
                {

                  yyreportSyntaxError (&yystack);
                  goto yyuser_error;
                }
              else
                YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue));
            }
        }

      while (yytrue)
        {
          yySymbol yytoken_to_shift;
          size_t yys;

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            yystackp->yytops.yylookaheadNeeds[yys] = yychar != YYEMPTY;

          /* yyprocessOneStack returns one of three things:

              - An error flag.  If the caller is yyprocessOneStack, it
                immediately returns as well.  When the caller is finally
                yyparse, it jumps to an error label via YYCHK1.

              - yyok, but yyprocessOneStack has invoked yymarkStackDeleted
                (&yystack, yys), which sets the top state of yys to NULL.  Thus,
                yyparse's following invocation of yyremoveDeletes will remove
                the stack.

              - yyok, when ready to shift a token.

             Except in the first case, yyparse will invoke yyremoveDeletes and
             then shift the next token onto all remaining stacks.  This
             synchronization of the shift (that is, after all preceding
             reductions on all stacks) helps prevent double destructor calls
             on yylval in the event of memory exhaustion.  */

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn));
          yyremoveDeletes (&yystack);
          if (yystack.yytops.yysize == 0)
            {
              yyundeleteLastStack (&yystack);
              if (yystack.yytops.yysize == 0)
                yyFail (&yystack, YY_("syntax error"));
              YYCHK1 (yyresolveStack (&yystack));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));

              yyreportSyntaxError (&yystack);
              goto yyuser_error;
            }

          /* If any yyglrShift call fails, it will fail after shifting.  Thus,
             a copy of yylval will already be on stack 0 in the event of a
             failure in the following loop.  Thus, yychar is set to YYEMPTY
             before the loop to make sure the user destructor for yylval isn't
             called twice.  */
          yytoken_to_shift = YYTRANSLATE (yychar);
          yychar = YYEMPTY;
          yyposn += 1;
          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            {
              int yyaction;
              const short int* yyconflicts;
              yyStateNum yystate = yystack.yytops.yystates[yys]->yylrState;
              yygetLRActions (yystate, yytoken_to_shift, &yyaction,
                              &yyconflicts);
              /* Note that yyconflicts were handled by yyprocessOneStack.  */
              YYDPRINTF ((stderr, "On stack %lu, ", (unsigned long int) yys));
              YY_SYMBOL_PRINT ("shifting", yytoken_to_shift, &yylval, &yylloc);
              yyglrShift (&yystack, yys, yyaction, yyposn,
                          &yylval);
              YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
                          (unsigned long int) yys,
                          yystack.yytops.yystates[yys]->yylrState));
            }

          if (yystack.yytops.yysize == 1)
            {
              YYCHK1 (yyresolveStack (&yystack));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
              yycompressStack (&yystack);
              break;
            }
        }
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (yyfalse);
  goto yyabortlab;

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

 yyreturn:
  if (yychar != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
                YYTRANSLATE (yychar), &yylval);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems)
    {
      yyGLRState** yystates = yystack.yytops.yystates;
      if (yystates)
        {
          size_t yysize = yystack.yytops.yysize;
          size_t yyk;
          for (yyk = 0; yyk < yysize; yyk += 1)
            if (yystates[yyk])
              {
                while (yystates[yyk])
                  {
                    yyGLRState *yys = yystates[yyk];
                  if (yys->yypred != YY_NULLPTR)
                      yydestroyGLRState ("Cleanup: popping", yys);
                    yystates[yyk] = yys->yypred;
                    yystack.yynextFree -= 1;
                    yystack.yyspaceLeft += 1;
                  }
                break;
              }
        }
      yyfreeGLRStack (&yystack);
    }

  return yyresult;
}

/* DEBUGGING ONLY */
#if YYDEBUG
static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      YYFPRINTF (stderr, " -> ");
    }
  YYFPRINTF (stderr, "%d@%lu", yys->yylrState,
             (unsigned long int) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == YY_NULLPTR)
    YYFPRINTF (stderr, "<null>");
  else
    yy_yypstack (yyst);
  YYFPRINTF (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystackp, size_t yyk)
{
  yypstates (yystackp->yytops.yystates[yyk]);
}

#define YYINDEX(YYX)                                                         \
    ((YYX) == YY_NULLPTR ? -1 : (yyGLRStackItem*) (YYX) - yystackp->yyitems)


static void
yypdumpstack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystackp->yyitems; yyp < yystackp->yynextFree; yyp += 1)
    {
      YYFPRINTF (stderr, "%3lu. ",
                 (unsigned long int) (yyp - yystackp->yyitems));
      if (*(yybool *) yyp)
        {
          YYASSERT (yyp->yystate.yyisState);
          YYASSERT (yyp->yyoption.yyisState);
          YYFPRINTF (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
                     yyp->yystate.yyresolved, yyp->yystate.yylrState,
                     (unsigned long int) yyp->yystate.yyposn,
                     (long int) YYINDEX (yyp->yystate.yypred));
          if (! yyp->yystate.yyresolved)
            YYFPRINTF (stderr, ", firstVal: %ld",
                       (long int) YYINDEX (yyp->yystate
                                             .yysemantics.yyfirstVal));
        }
      else
        {
          YYASSERT (!yyp->yystate.yyisState);
          YYASSERT (!yyp->yyoption.yyisState);
          YYFPRINTF (stderr, "Option. rule: %d, state: %ld, next: %ld",
                     yyp->yyoption.yyrule - 1,
                     (long int) YYINDEX (yyp->yyoption.yystate),
                     (long int) YYINDEX (yyp->yyoption.yynext));
        }
      YYFPRINTF (stderr, "\n");
    }
  YYFPRINTF (stderr, "Tops:");
  for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
    YYFPRINTF (stderr, "%lu: %ld; ", (unsigned long int) yyi,
               (long int) YYINDEX (yystackp->yytops.yystates[yyi]));
  YYFPRINTF (stderr, "\n");
}
#endif

#undef yylval
#undef yychar
#undef yynerrs



#line 627 "lang.y" /* glr.c:2584  */


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
	syntree_out << "\nSyntax Tree:" << std::flush;
	syntree_out << (*syntree) << std::endl << std::endl;
	if( err_count > 0 ) {
		return -1;
	}
	if( settings.output_format == command_line_data::output_format_t::PARSE )
		return 0;
	intermediateCode ic( scptab );
	ic.defineFunction( GLOBAL_FUNCTION, syntree->getRoot() );
	if( err_count > 0 )
		return -1;
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
