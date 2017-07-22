/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 2 "lang.y" /* yacc.c:339  */


#include <iostream>
#include <cstdio>
#include <iomanip>
#include <cmath>
#include <stack>
#include <cstring>
#include "symbol_table.h"
#include "scope_table.h"
#include "types.h"
#include "syntax_tree.h"
#include "debug.h"
#include "error_reporting.h"

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


#line 119 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
    IF = 275,
    WHILE = 276,
    ELSE = 277,
    FOR = 278,
    IN = 279,
    RELOP = 280,
    ELLIPSIS = 281
  };
#endif
/* Tokens.  */
#define INT 258
#define FLT 259
#define ID 260
#define TYPENAME 261
#define VTYPE 262
#define ASSIGNMENT 263
#define ADDOP 264
#define MULOP 265
#define COMMA 266
#define SEMICOLON 267
#define LSEQ 268
#define RSEQ 269
#define LBRA 270
#define RBRA 271
#define STRING_BEGIN 272
#define STRING_END 273
#define STRING_PARTICLE 274
#define IF 275
#define WHILE 276
#define ELSE 277
#define FOR 278
#define IN 279
#define RELOP 280
#define ELLIPSIS 281

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 62 "lang.y" /* yacc.c:355  */

	char *str;
	int64_t num;
	double flt;
	const type_t* typ;
	std::vector<type_t>* typlst;
	struct syntaxTree::node *node;

#line 220 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 237 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   172

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  111

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
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
      25,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    78,    78,    81,    86,    89,    92,    95,    98,   101,
     104,   107,   110,   113,   115,   120,   123,   128,   132,   138,
     145,   145,   159,   168,   172,   175,   180,   184,   184,   187,
     187,   191,   193,   196,   206,   209,   219,   230,   234,   236,
     239,   242,   249,   252,   255,   259,   262,   269,   273,   278,
     282,   284,   289,   292,   296,   296,   311,   314,   321,   324
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLT", "ID", "TYPENAME", "VTYPE",
  "ASSIGNMENT", "ADDOP", "MULOP", "COMMA", "SEMICOLON", "LSEQ", "RSEQ",
  "LBRA", "RBRA", "STRING_BEGIN", "STRING_END", "STRING_PARTICLE", "IF",
  "WHILE", "ELSE", "FOR", "IN", "RELOP", "ELLIPSIS", "$accept",
  "entry_point", "sequential_block", "statement_list", "statement", "id",
  "typelist", "type", "declarations", "$@1", "declaration_list",
  "declaration_list_el", "assignment", "leaf_expression", "expression",
  "relational", "sum", "product", "factor", "constant", "list",
  "list_items", "string_particles", "variable", "function_call",
  "declaration", "parameter_list", "for", "$@2", "while", "if", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281
};
# endif

#define YYPACT_NINF -78

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-78)))

#define YYTABLE_NINF -20

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     149,    -8,   -78,   -78,   -78,    21,   128,    61,    18,    25,
      28,    30,    33,   -78,   -78,   107,     4,   -78,    32,    34,
     -78,    36,   -78,    24,    43,    48,   -78,   -78,    45,   -78,
     -78,   -78,   -78,   -78,   -78,   -78,    47,     5,    53,     8,
      44,    46,   -78,   -78,    20,    61,    61,    21,   -78,   -78,
      29,    21,   -78,   -78,   -78,    61,    61,    61,    61,   -78,
      61,   -78,    57,    29,   -78,   -78,   -78,    54,    55,    49,
      60,    63,    19,    58,    64,    69,   -78,    70,   -78,   -78,
     -78,   -78,   -78,    21,   149,   149,    61,    29,   -78,    29,
     -78,   -78,    29,   -78,    61,    21,    66,    71,    75,    83,
     -78,   -78,   -78,   -78,   149,   149,   -78,   -78,   -78,   149,
     -78
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    39,    40,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     9,     2,     0,    49,    20,     0,     0,
      38,     0,    31,    32,    34,    37,    27,    42,    28,    29,
      11,    12,    10,    13,    17,    44,     0,    45,     0,     0,
      49,     0,    28,    47,     0,     0,     0,     0,     1,     5,
       0,     0,     6,     7,     8,     0,     0,     0,     0,     3,
       0,    43,    45,     0,    30,    41,    48,     0,     0,     0,
      19,     0,    15,    52,     0,    25,    21,    23,    33,    35,
      36,    26,    46,     0,     0,     0,     0,     0,    18,     0,
      49,    51,     0,    50,     0,     0,    58,    56,     0,    15,
      16,    53,    24,    22,     0,     0,    54,    59,    57,     0,
      55
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -78,   -78,   -78,    -5,   -77,     0,     7,   -32,   -78,   -78,
       6,   -78,   -78,   -78,    -4,    51,    59,    56,   -78,   -78,
     -78,    40,   -78,    14,   -78,   -78,    10,   -78,   -78,   -78,
     -78
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    40,    71,    17,    18,    51,
      76,    77,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    38,    44,    42,    29,    73,    74,    30,   109,    31,
      32
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      16,    36,    37,    41,    33,    34,    16,    96,    97,   -19,
      49,     2,     3,     4,    28,    16,    60,    54,    72,    50,
      28,    39,    35,     7,     4,     8,     4,   107,   108,    28,
      89,    83,   110,    48,     4,    62,     5,    43,    65,    66,
      45,    67,    68,    46,    52,    47,    53,    69,    54,    55,
      70,    75,    56,    58,    81,    99,    62,    99,    57,    63,
      83,    59,    64,    70,     2,     3,     4,    61,    60,    92,
      84,    85,    90,    86,    39,    87,     7,    94,     8,    88,
      93,    95,    98,    90,    16,    16,    91,    70,   104,    70,
     102,   106,    70,   105,    89,    75,   100,    91,    28,    28,
      82,   103,   101,     0,    16,    16,    78,    -4,     1,    16,
       2,     3,     4,    80,     5,    79,     0,     0,    28,    28,
       6,    -4,     7,    28,     8,     0,     0,     9,    10,     1,
      11,     2,     3,     4,     0,     5,     0,     0,     0,     0,
       0,     6,    35,     7,     0,     8,     0,     0,     9,    10,
       1,    11,     2,     3,     4,     0,     5,     0,     0,     0,
       0,     0,     6,     0,     7,     0,     8,     0,     0,     9,
      10,     0,    11
};

static const yytype_int8 yycheck[] =
{
       0,     6,     6,     7,    12,     5,     6,    84,    85,     5,
      15,     3,     4,     5,     0,    15,    11,    12,    50,    15,
       6,    13,    14,    15,     5,    17,     5,   104,   105,    15,
      11,    63,   109,     0,     5,    39,     7,    19,    18,    19,
      15,    45,    46,    15,    12,    15,    12,    47,    12,    25,
      50,    51,     9,     8,    58,    87,    60,    89,    10,    15,
      92,    14,    16,    63,     3,     4,     5,    14,    11,    11,
      16,    16,    72,    24,    13,    15,    15,     8,    17,    16,
      16,    11,    86,    83,    84,    85,    72,    87,    22,    89,
      94,    16,    92,    22,    11,    95,    89,    83,    84,    85,
      60,    95,    92,    -1,   104,   105,    55,     0,     1,   109,
       3,     4,     5,    57,     7,    56,    -1,    -1,   104,   105,
      13,    14,    15,   109,    17,    -1,    -1,    20,    21,     1,
      23,     3,     4,     5,    -1,     7,    -1,    -1,    -1,    -1,
      -1,    13,    14,    15,    -1,    17,    -1,    -1,    20,    21,
       1,    23,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    13,    -1,    15,    -1,    17,    -1,    -1,    20,
      21,    -1,    23
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     5,     7,    13,    15,    17,    20,
      21,    23,    28,    29,    30,    31,    32,    34,    35,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    50,    51,
      54,    56,    57,    12,    32,    14,    30,    41,    48,    13,
      32,    41,    50,    19,    49,    15,    15,    15,     0,    30,
      15,    36,    12,    12,    12,    25,     9,    10,     8,    14,
      11,    14,    41,    15,    16,    18,    19,    41,    41,    32,
      32,    33,    34,    52,    53,    32,    37,    38,    42,    43,
      44,    41,    48,    34,    16,    16,    24,    15,    16,    11,
      32,    50,    11,    16,     8,    11,    31,    31,    41,    34,
      33,    53,    41,    37,    22,    22,    16,    31,    31,    55,
      31
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    27,    28,    29,    30,    30,    31,    31,    31,    31,
      31,    31,    31,    31,    32,    33,    33,    34,    34,    34,
      36,    35,    37,    37,    38,    38,    39,    40,    40,    40,
      40,    41,    42,    42,    43,    43,    44,    44,    45,    46,
      46,    46,    46,    47,    47,    48,    48,    49,    49,    50,
      51,    52,    53,    53,    55,    54,    56,    56,    57,    57
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     1,     2,     2,     2,     2,     1,
       1,     1,     1,     2,     1,     1,     3,     2,     4,     1,
       0,     3,     3,     1,     3,     1,     3,     1,     1,     1,
       3,     1,     1,     3,     1,     3,     3,     1,     1,     1,
       1,     3,     1,     3,     2,     1,     3,     1,     2,     1,
       4,     2,     1,     3,     0,     8,     5,     7,     5,     7
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
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
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
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

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
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
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
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
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
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
  return 0;
}
#endif /* YYERROR_VERBOSE */

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




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 78 "lang.y" /* yacc.c:1646  */
    {
							syntree->setRoot( (yyvsp[0].node) );
						}
#line 1405 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 81 "lang.y" /* yacc.c:1646  */
    {
							syntaxTree::node* n = (yyvsp[-1].node);
							n->setType( N_SEQUENTIAL_BLOCK );
							(yyval.node) = n;
						}
#line 1415 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 86 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_BLOCK_LIST, (yyvsp[0].node) );
						}
#line 1423 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 89 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_BLOCK_LIST, (yyvsp[-1].node), (yyvsp[0].node) );
						}
#line 1431 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 92 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[-1].node);
						}
#line 1439 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 95 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[-1].node);
						}
#line 1447 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 98 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[-1].node);
						}
#line 1455 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 101 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 104 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 107 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1479 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 110 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1487 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 115 "lang.y" /* yacc.c:1646  */
    {
							(yyval.num) = symtab->addSymbol( (yyvsp[0].str) );
							free( (yyvsp[0].str) );
						}
#line 1496 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 120 "lang.y" /* yacc.c:1646  */
    {
							(yyval.typlst) = new std::vector<type_t>{ *(yyvsp[0].typ) };
						}
#line 1504 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 123 "lang.y" /* yacc.c:1646  */
    {
							(yyval.typlst) = (yyvsp[0].typlst);
							(yyval.typlst)->insert( (yyval.typlst)->begin(), *(yyvsp[-2].typ) );
						}
#line 1513 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 128 "lang.y" /* yacc.c:1646  */
    {
							pmesg( 90, "ERROR: variadic types not yet implemented\n" );
							(yyval.typ) = &ERROR_TYPE;
						}
#line 1522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 132 "lang.y" /* yacc.c:1646  */
    {
							pmesg( 90, "Lexer: TYPENAME %s\n", symtab->getName( (yyvsp[-3].num) ).c_str() );
							(yyval.typ) = new type_t( scptab->getTypeDefinition( current_scope, (yyvsp[-3].num), *(yyvsp[-1].typlst) ) );
							if( *(yyval.typ) == ERROR_TYPE )
								lerr << error_line() << "Unknown type '" << symtab->getName( (yyvsp[-3].num) ).c_str() << "'" << std::endl;
						}
#line 1533 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 138 "lang.y" /* yacc.c:1646  */
    {
							pmesg( 90, "Lexer: TYPENAME %s\n", symtab->getName( (yyvsp[0].num) ).c_str() );
							(yyval.typ) = new type_t( scptab->getTypeDefinition( current_scope, (yyvsp[0].num) ) );
							if( *(yyval.typ) == ERROR_TYPE )
								lerr << error_line() << "Unknown type '" << symtab->getName( (yyvsp[0].num) ).c_str() << "'" << std::endl;
						}
#line 1544 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 145 "lang.y" /* yacc.c:1646  */
    {
							//pmesg( 90, "Declaring variables of type %d\n", $<num>1 );
							std::cout << "Declaring variables of type " << (*(yyvsp[0].typ)) << std::endl;
							decllisttypes.push( *(yyvsp[0].typ)  );
							delete (yyvsp[0].typ);
						}
#line 1555 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 150 "lang.y" /* yacc.c:1646  */
    {
							syntaxTree::node* n = (yyvsp[0].node);
							if( n == nullptr )
								(yyval.node) = new syntaxTree::node( N_EMPTY );
							else
								(yyval.node) = n;
							decllisttypes.pop();
						}
#line 1568 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 159 "lang.y" /* yacc.c:1646  */
    {
							if( (yyvsp[-2].node) == nullptr ) {
								(yyval.node) = (yyvsp[0].node);
							} else if( (yyvsp[0].node) == nullptr ){
								(yyval.node) = (yyvsp[-2].node);
							} else {
								(yyval.node) = new syntaxTree::node( N_BLOCK_LIST, (yyvsp[-2].node), (yyvsp[0].node) );
							}
						}
#line 1582 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 168 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1590 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 172 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_ASSIGN, symbolToVariable( (yyvsp[-2].num), decllisttypes.top() ), (yyvsp[0].node) );
						}
#line 1598 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 175 "lang.y" /* yacc.c:1646  */
    {
							variable_t variable = scptab->addVariable( current_scope, (yyvsp[0].num), decllisttypes.top() );
							(yyval.node) = nullptr;
						}
#line 1607 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 180 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_ASSIGN, (yyvsp[-2].node), (yyvsp[0].node) );	
						}
#line 1615 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 184 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1623 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 187 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[-1].node);
						}
#line 1631 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 193 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1639 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 196 "lang.y" /* yacc.c:1646  */
    {
							node_t type;
							int c = (yyvsp[-1].num);
							if( c == 1 )
								type = N_NOT_EQUALS;
							else if( c == 0 )
								type = N_EQUALS;
							(yyval.node) = new syntaxTree::node( type, (yyvsp[-2].node), (yyvsp[0].node) );
						}
#line 1653 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 206 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);	
						}
#line 1661 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 209 "lang.y" /* yacc.c:1646  */
    {
							node_t type;
							int c = (yyvsp[-1].num);
							if( c == 1 )
								type = N_ADD;
							else if( c == 0 )
								type = N_SUBTRACT;
							(yyval.node) = new syntaxTree::node( type, (yyvsp[-2].node), (yyvsp[0].node) );
						}
#line 1675 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 219 "lang.y" /* yacc.c:1646  */
    {
							node_t type;
							int c = (yyvsp[-1].num);
							if( c == 1 )
								type = N_MULTIPLY;
							else if( c == 0 )
								type = N_DIVIDE;
							else
								type = N_REMAINDER;
							(yyval.node) = new syntaxTree::node( type, (yyvsp[-2].node), (yyvsp[0].node) );
						}
#line 1691 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 230 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1699 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 236 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_INTEGER, nullptr, nullptr, {.integer=(yyvsp[0].num)} );
						}
#line 1707 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 239 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_FLOAT, nullptr, nullptr, {.floating=(yyvsp[0].flt)} );
						}
#line 1715 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 242 "lang.y" /* yacc.c:1646  */
    {
							pmesg(90, "Lexer: STR: \"%s\"\n", input_str.c_str());
							syntaxTree::node::extra_data_t d;
							d.string = strdup(input_str.c_str());
							(yyval.node) = new syntaxTree::node( N_STRING, nullptr, nullptr, d );
							input_str.clear();
						}
#line 1727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 249 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1735 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 252 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_LIST, (yyvsp[-1].node) );
						}
#line 1743 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 255 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_LIST );
						}
#line 1751 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 259 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_SINGLE_TYPE_EXPRESSION_LIST, (yyvsp[0].node) );
						}
#line 1759 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 262 "lang.y" /* yacc.c:1646  */
    {
							type_t a = (yyvsp[-2].node)->data_type, b = (yyvsp[0].node)->data_type;
							if( a != b and a != ERROR_TYPE and b != ERROR_TYPE )
								lerr << error_line() << "Lists and sets can only contain elements of a single type" << std::endl;
							(yyval.node) = new syntaxTree::node( N_SINGLE_TYPE_EXPRESSION_LIST, (yyvsp[-2].node), (yyvsp[0].node) );
						}
#line 1770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 269 "lang.y" /* yacc.c:1646  */
    {
							input_str.append( (yyvsp[0].str) );
							free( (yyvsp[0].str) );
						}
#line 1779 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 273 "lang.y" /* yacc.c:1646  */
    {
							input_str.append( (yyvsp[0].str) );
							free( (yyvsp[0].str) );
						}
#line 1788 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 278 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = symbolToVariable( (yyvsp[0].num) );
						}
#line 1796 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 284 "lang.y" /* yacc.c:1646  */
    {
							/*symtab->setType( $<num>2, $<num>1 );
							$<node>$ = new syntaxTree::node( VARIABLE, $<num>2 );*/
						}
#line 1805 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 289 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = (yyvsp[0].node);
						}
#line 1813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 292 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_ARGUMENT_LIST, (yyvsp[-2].node), (yyvsp[0].node) );
						}
#line 1821 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 296 "lang.y" /* yacc.c:1646  */
    {
							structure_t base = (yyvsp[-1].node)->data_type.getBase();
							if( base != LST_STRUCTURE and base != SET_STRUCTURE )
								lerr << error_line() << "Cannot iterate over variables of type " << (yyvsp[-1].node)->data_type << std::endl;
							type_t t = (yyvsp[-1].node)->data_type.getChildType();
							symbolToVariable( (yyvsp[-3].num), t );
						}
#line 1833 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 302 "lang.y" /* yacc.c:1646  */
    {
							symbol_t s = symtab->addTemporarySymbol();
							(yyval.node) = new syntaxTree::node( N_SEQUENTIAL_BLOCK, 
								new syntaxTree::node( N_ASSIGN, 
									symbolToVariable( s, (yyvsp[-3].node)->data_type ),
									(yyvsp[-3].node) ),
								new syntaxTree::node( N_WHILE, new syntaxTree::node( N_EMPTY ) /*rip*/, (yyvsp[0].node) ) );
						}
#line 1846 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 311 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_WHILE, (yyvsp[-2].node), (yyvsp[0].node) );
						}
#line 1854 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 314 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_WHILE, 
								(yyvsp[-4].node), 
								new syntaxTree::node( N_ELSE, (yyvsp[-2].node), (yyvsp[0].node) )
							);
						}
#line 1865 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 321 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_IF, (yyvsp[-2].node), (yyvsp[0].node) );
						}
#line 1873 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 324 "lang.y" /* yacc.c:1646  */
    {
							(yyval.node) = new syntaxTree::node( N_IF, (yyvsp[-4].node), new syntaxTree::node( N_ELSE, (yyvsp[-2].node), (yyvsp[0].node) ) );
						}
#line 1881 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1885 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 328 "lang.y" /* yacc.c:1906  */


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
