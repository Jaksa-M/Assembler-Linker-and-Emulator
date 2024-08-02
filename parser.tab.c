/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "definitions.h"

extern FILE *yyin;
extern int yylex();
void yyerror(const char *s);
int yyparse(void);
extern char *yytext;

extern void addToSymbolList(char* symbol);
extern void printSymbolList();
extern void clearSymbolList();
extern void addSymbolToWordList();
extern void addLiteralToWordList();
extern void printWordList();
extern void clearWordList();

//instruction functions:
extern void instruction_add_sub_mul_div_xchg(char* instr, int gprS, int gprD);
extern void instruction_and_or_xor_shl_shr(char* instr, int gprS, int gprD);
extern void instruction_not_push_pop(char* instr, int gpr);
extern void instruction_halt_int_iret_ret(char* instr);
extern void instruction_beq_bne_bgt(char* instr, int gpr1, int gpr2, char* what_op, int literal, char* symbol);
extern void instruction_call_jmp(char* instr, char* what_op, int literal, char* symbol);
extern void instruction_csrrd_csrrw(char* instr, int csr, int gpr);
extern void instruction_ld_st(char* instr, char* what_op, int gpr, int literal, char* symbol, int dollar_literal, char* dollar_symbol,
                                int reg, int mem_reg, int mem_reg_literal, int mem_reg_symbol);

//directives:
extern void directive_global();
extern void directive_extern();
extern void directive_section();
extern void directive_word();
extern void directive_skip(int literal);
extern void directive_ascii(const char* str);
extern void directive_equ();
extern void process_label(char* label);

//print functions:
extern void printSymbolTable();
extern void printFlinkTable();
extern void printRelocationTables();

//flink table functions:
extern void processFlinkTable();

#line 120 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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
    SYMBOL = 258,
    DOLLAR_SYMBOL = 259,
    STRING = 260,
    LABEL = 261,
    LITERAL = 262,
    DOLLAR_LITERAL = 263,
    GPRX = 264,
    CSRX = 265,
    SECTION = 266,
    WORD = 267,
    SKIP = 268,
    ASCII = 269,
    EQU = 270,
    END = 271,
    COMMA = 272,
    COLON = 273,
    GLOBAL = 274,
    EXTERN = 275,
    HALT = 276,
    INT = 277,
    IRET = 278,
    CALL = 279,
    RET = 280,
    JMP = 281,
    BEQ = 282,
    BNE = 283,
    BGT = 284,
    PUSH = 285,
    POP = 286,
    XCHG = 287,
    ADD = 288,
    SUB = 289,
    MUL = 290,
    DIV = 291,
    NOT = 292,
    AND = 293,
    OR = 294,
    XOR = 295,
    SHL = 296,
    SHR = 297,
    LD = 298,
    ST = 299,
    CSRRD = 300,
    CSRWR = 301,
    LEFT_BRACKET = 302,
    RIGHT_BRACKET = 303,
    PLUS = 304
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 51 "parser.y"

  int val1;
  int val2;
  char* val_str;
  int reg_num;

#line 229 "parser.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   164

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  172

#define YYUNDEFTOK  2
#define YYMAXUTOK   304


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    74,    74,    76,    77,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    91,    98,   105,   107,   114,
     116,   118,   120,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   169,   178,
     179,   181,   182,   183,   184
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SYMBOL", "DOLLAR_SYMBOL", "STRING",
  "LABEL", "LITERAL", "DOLLAR_LITERAL", "GPRX", "CSRX", "SECTION", "WORD",
  "SKIP", "ASCII", "EQU", "END", "COMMA", "COLON", "GLOBAL", "EXTERN",
  "HALT", "INT", "IRET", "CALL", "RET", "JMP", "BEQ", "BNE", "BGT", "PUSH",
  "POP", "XCHG", "ADD", "SUB", "MUL", "DIV", "NOT", "AND", "OR", "XOR",
  "SHL", "SHR", "LD", "ST", "CSRRD", "CSRWR", "LEFT_BRACKET",
  "RIGHT_BRACKET", "PLUS", "$accept", "program", "statements", "statement",
  "global_declaration", "extern_declaration", "section_declaration",
  "word_declaration", "skip_declaration", "ascii_declaration",
  "equ_declaration", "label_definition", "instruction", "end_statement",
  "SYMBOL_LIST", "SYMBOL_LITERAL_LIST", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

#define YYPACT_NINF (-47)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -47,     9,    34,   -47,   -47,    30,     7,    27,    31,    32,
     -47,    35,    35,   -47,   -47,   -47,    12,   -47,    13,    28,
      33,    43,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    -3,    85,    29,    86,   -47,
     -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,
     -47,   -47,   -47,    24,   -47,   -47,    26,   -47,    87,    87,
     -47,   -47,   -47,   -47,    88,    89,    90,   -47,   -47,    91,
      92,    93,    94,    95,   -47,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   114,   107,   108,   109,    14,
     120,   125,   121,   122,   123,   124,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     -46,     4,   140,   119,   -47,   -47,   -47,   -47,   117,   141,
     142,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,
     -47,   -47,   -47,   -47,   -47,   -47,   143,    15,   -47,   -47,
     -47,   -47,   -47,   144,   -47,   -47,    20,    21,    22,   145,
      48,    49,   -17,   -47,   -47,   -47,   -47,   -47,   -47,   -47,
     146,   147,   -47,    23,   148,   152,    50,    51,   -47,   -47,
     -47,   -47
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,    22,     0,     0,     0,     0,     0,
      68,     0,     0,    23,    24,    25,     0,    28,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      17,    73,    74,    18,    19,    20,     0,    70,    15,    16,
      26,    27,    29,    30,     0,     0,     0,    37,    38,     0,
       0,     0,     0,     0,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    71,    72,    21,    69,     0,     0,
       0,    39,    40,    41,    42,    43,    45,    46,    47,    48,
      49,    52,    50,    53,    51,    54,     0,     0,    60,    58,
      61,    59,    62,     0,    66,    67,     0,     0,     0,     0,
       0,     0,     0,    31,    32,    33,    34,    35,    36,    55,
       0,     0,    63,     0,     0,     0,     0,     0,    57,    56,
      65,    64
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,
     -47,   -47,   -47,   -47,   150,   -47
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    58,    53
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      80,    81,   136,   137,    82,    83,    84,   138,   139,     3,
      51,   140,   141,   142,    52,    60,    62,   114,   150,    61,
      63,   115,   151,   153,   155,   157,   166,   154,   156,   158,
     167,   162,   163,    50,    54,    56,    55,    64,    57,    87,
       4,    89,    65,    90,    85,     5,     6,     7,     8,     9,
      10,   143,    66,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    86,    88,   160,   161,   170,   171,
       0,     0,     0,     0,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   116,   117,   145,
     118,   119,   120,   121,   146,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   144,
       0,     0,     0,   152,   159,     0,     0,   168,   147,   148,
     149,   169,    59,   164,   165
};

static const yytype_int8 yycheck[] =
{
       3,     4,    48,    49,     7,     8,     9,     3,     4,     0,
       3,     7,     8,     9,     7,     3,     3,     3,     3,     7,
       7,     7,     7,     3,     3,     3,     3,     7,     7,     7,
       7,    48,    49,     3,     7,     3,     5,     9,     3,    10,
       6,    17,     9,    17,    47,    11,    12,    13,    14,    15,
      16,    47,     9,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,     9,     9,     9,     9,     9,     9,     9,     9,     9,
       9,     9,     9,     9,     9,     9,    48,    48,    48,    48,
      -1,    -1,    -1,    -1,    17,    17,    17,    17,    17,    17,
      17,    17,    17,    17,    17,    17,    17,    17,    17,    17,
      17,    17,    17,     9,    17,    17,    17,     7,     3,    10,
       9,     9,     9,     9,    17,     9,     9,     9,     9,     9,
       9,     9,     9,     9,     9,     9,     9,     9,     9,     9,
      -1,    -1,    -1,     9,     9,    -1,    -1,     9,    17,    17,
      17,     9,    12,    17,    17
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    51,    52,     0,     6,    11,    12,    13,    14,    15,
      16,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
       3,     3,     7,    65,     7,     5,     3,     3,    64,    64,
       3,     7,     3,     7,     9,     9,     9,     9,     9,     9,
       9,     9,     9,     9,     9,     9,     9,     9,     9,     9,
       3,     4,     7,     8,     9,    47,     9,    10,     9,    17,
      17,    17,    17,    17,    17,    17,    17,    17,    17,    17,
      17,    17,    17,    17,    17,    17,    17,    17,    17,    17,
       9,    17,    17,    17,     3,     7,     7,     3,     9,     9,
       9,     9,     9,     9,     9,     9,     9,     9,     9,     9,
       9,     9,     9,     9,     9,     9,    48,    49,     3,     4,
       7,     8,     9,    47,     9,    10,    17,    17,    17,    17,
       3,     7,     9,     3,     7,     3,     7,     3,     7,     9,
      48,    48,    48,    49,    17,    17,     3,     7,     9,     9,
      48,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    52,    52,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    63,    64,
      64,    65,    65,    65,    65
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     2,
       2,     4,     1,     1,     1,     1,     2,     2,     1,     2,
       2,     6,     6,     6,     6,     6,     6,     2,     2,     4,
       4,     4,     4,     4,     2,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     6,     8,     8,     4,     4,
       4,     4,     4,     6,     8,     8,     4,     4,     1,     3,
       1,     3,     3,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
          ++yyp;
          ++yyformat;
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

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
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 15:
#line 91 "parser.y"
                                       { 
    printf("GLOBAL ");
    printSymbolList();
    directive_global();
    clearSymbolList();
}
#line 1520 "parser.tab.c"
    break;

  case 16:
#line 98 "parser.y"
                                       {
    printf("EXTERN ");
    printSymbolList();
    directive_extern();
    clearSymbolList();
}
#line 1531 "parser.tab.c"
    break;

  case 17:
#line 105 "parser.y"
                                    { printf("SECTION %s\n", (yyvsp[0].val_str)); directive_section((yyvsp[0].val_str));}
#line 1537 "parser.tab.c"
    break;

  case 18:
#line 107 "parser.y"
                                           {
    printf("WORD ");
    printWordList();
    directive_word();
    clearWordList();
}
#line 1548 "parser.tab.c"
    break;

  case 19:
#line 114 "parser.y"
                               { printf("SKIP %d\n", (yyvsp[0].val1)); directive_skip((yyvsp[0].val1));}
#line 1554 "parser.tab.c"
    break;

  case 20:
#line 116 "parser.y"
                                { printf("ASCII %s\n", (yyvsp[0].val_str)); directive_ascii((yyvsp[0].val_str));}
#line 1560 "parser.tab.c"
    break;

  case 21:
#line 118 "parser.y"
                                          { printf(".equ %d\n", (yyvsp[0].val1)); directive_equ();}
#line 1566 "parser.tab.c"
    break;

  case 22:
#line 120 "parser.y"
                        { printf("LABEL: %s\n", (yyvsp[0].val_str)); process_label((yyvsp[0].val_str));}
#line 1572 "parser.tab.c"
    break;

  case 23:
#line 122 "parser.y"
                  { printf("HALT\n"); instruction_halt_int_iret_ret("halt");}
#line 1578 "parser.tab.c"
    break;

  case 24:
#line 123 "parser.y"
                 { printf("INT\n"); instruction_halt_int_iret_ret("int");}
#line 1584 "parser.tab.c"
    break;

  case 25:
#line 124 "parser.y"
                  { printf("IRET\n"); instruction_halt_int_iret_ret("iret");}
#line 1590 "parser.tab.c"
    break;

  case 26:
#line 125 "parser.y"
                         { printf("CALL %s\n", (yyvsp[0].val_str)); instruction_call_jmp("call", "symbol", 0, (yyvsp[0].val_str));}
#line 1596 "parser.tab.c"
    break;

  case 27:
#line 126 "parser.y"
                          { printf("CALL %d\n", (yyvsp[0].val1)); instruction_call_jmp("call", "literal", (yyvsp[0].val1), "");}
#line 1602 "parser.tab.c"
    break;

  case 28:
#line 127 "parser.y"
                 { printf("RET\n"); instruction_halt_int_iret_ret("ret");}
#line 1608 "parser.tab.c"
    break;

  case 29:
#line 128 "parser.y"
                        { printf("JMP %s\n", (yyvsp[0].val_str)); instruction_call_jmp("jmp", "symbol", 0, (yyvsp[0].val_str));}
#line 1614 "parser.tab.c"
    break;

  case 30:
#line 129 "parser.y"
                         { printf("JMP %d\n", (yyvsp[0].val1)); instruction_call_jmp("jmp", "symbol", (yyvsp[0].val1), "");}
#line 1620 "parser.tab.c"
    break;

  case 31:
#line 130 "parser.y"
                                              { printf("BEQ %d, %d, %s\n", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), (yyvsp[0].val_str)); instruction_beq_bne_bgt("beq", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), "symbol", 0, (yyvsp[0].val_str));}
#line 1626 "parser.tab.c"
    break;

  case 32:
#line 131 "parser.y"
                                               { printf("BEQ %d, %d, %d\n", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), (yyvsp[0].val1)); instruction_beq_bne_bgt("beq", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), "literal", (yyvsp[0].val1), "");}
#line 1632 "parser.tab.c"
    break;

  case 33:
#line 132 "parser.y"
                                              { printf("BNE %d, %d, %s\n", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), (yyvsp[0].val_str)); instruction_beq_bne_bgt("bne", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), "symbol", 0, (yyvsp[0].val_str));}
#line 1638 "parser.tab.c"
    break;

  case 34:
#line 133 "parser.y"
                                               { printf("BNE %d, %d, %d\n", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), (yyvsp[0].val1)); instruction_beq_bne_bgt("bne", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), "literal", (yyvsp[0].val1), "");}
#line 1644 "parser.tab.c"
    break;

  case 35:
#line 134 "parser.y"
                                              { printf("BGT %d, %d, %s\n", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), (yyvsp[0].val_str)); instruction_beq_bne_bgt("bgt", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), "symbol", 0, (yyvsp[0].val_str));}
#line 1650 "parser.tab.c"
    break;

  case 36:
#line 135 "parser.y"
                                               { printf("BGT %d, %d, %d\n", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), (yyvsp[0].val1)); instruction_beq_bne_bgt("bgt", (yyvsp[-4].reg_num), (yyvsp[-2].reg_num), "literal", (yyvsp[0].val1), "");}
#line 1656 "parser.tab.c"
    break;

  case 37:
#line 136 "parser.y"
                       { printf("PUSH %d\n", (yyvsp[0].reg_num)); instruction_not_push_pop("push",(yyvsp[0].reg_num));}
#line 1662 "parser.tab.c"
    break;

  case 38:
#line 137 "parser.y"
                      { printf("POP %d\n", (yyvsp[0].reg_num)); instruction_not_push_pop("pop",(yyvsp[0].reg_num));}
#line 1668 "parser.tab.c"
    break;

  case 39:
#line 138 "parser.y"
                                  { printf("XCHG %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_add_sub_mul_div_xchg("xchg", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1674 "parser.tab.c"
    break;

  case 40:
#line 139 "parser.y"
                                 { printf("ADD %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_add_sub_mul_div_xchg("add", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1680 "parser.tab.c"
    break;

  case 41:
#line 140 "parser.y"
                                 { printf("SUB %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_add_sub_mul_div_xchg("sub", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1686 "parser.tab.c"
    break;

  case 42:
#line 141 "parser.y"
                                 { printf("MUL %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_add_sub_mul_div_xchg("mul", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1692 "parser.tab.c"
    break;

  case 43:
#line 142 "parser.y"
                                 { printf("DIV %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_add_sub_mul_div_xchg("div", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1698 "parser.tab.c"
    break;

  case 44:
#line 143 "parser.y"
                      { printf("NOT %d\n", (yyvsp[0].reg_num)); instruction_not_push_pop("not",(yyvsp[0].reg_num));}
#line 1704 "parser.tab.c"
    break;

  case 45:
#line 144 "parser.y"
                                 { printf("AND %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_and_or_xor_shl_shr("and", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1710 "parser.tab.c"
    break;

  case 46:
#line 145 "parser.y"
                                { printf("OR %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_and_or_xor_shl_shr("or", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1716 "parser.tab.c"
    break;

  case 47:
#line 146 "parser.y"
                                 { printf("XOR %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_and_or_xor_shl_shr("xor", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1722 "parser.tab.c"
    break;

  case 48:
#line 147 "parser.y"
                                 { printf("SHL %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_and_or_xor_shl_shr("shl", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1728 "parser.tab.c"
    break;

  case 49:
#line 148 "parser.y"
                                 { printf("SHR %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_and_or_xor_shl_shr("shr", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1734 "parser.tab.c"
    break;

  case 50:
#line 149 "parser.y"
                                         { printf("LD %s, %d\n", (yyvsp[-2].val_str), (yyvsp[0].reg_num)); instruction_ld_st("ld","dollar_symbol",(yyvsp[0].reg_num),0,"",0,(yyvsp[-2].val_str),0,0,0,0);}
#line 1740 "parser.tab.c"
    break;

  case 51:
#line 150 "parser.y"
                                          { printf("LD %d, %d\n", (yyvsp[-2].val1), (yyvsp[0].reg_num)); instruction_ld_st("ld","dollar_literal",(yyvsp[0].reg_num),0,"",(yyvsp[-2].val1),"",0,0,0,0);}
#line 1746 "parser.tab.c"
    break;

  case 52:
#line 151 "parser.y"
                                  { printf("LD %s, %d\n", (yyvsp[-2].val_str), (yyvsp[0].reg_num)); instruction_ld_st("ld","symbol",(yyvsp[0].reg_num),0,(yyvsp[-2].val_str),0,"",0,0,0,0);}
#line 1752 "parser.tab.c"
    break;

  case 53:
#line 152 "parser.y"
                                   { printf("LD %d, %d\n", (yyvsp[-2].val1), (yyvsp[0].reg_num)); instruction_ld_st("ld","literal",(yyvsp[0].reg_num),(yyvsp[-2].val1),"",0,"",0,0,0,0);}
#line 1758 "parser.tab.c"
    break;

  case 54:
#line 153 "parser.y"
                                { printf("LD %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_ld_st("ld","reg",(yyvsp[0].reg_num),0,"",0,"",(yyvsp[-2].reg_num),0,0,0);}
#line 1764 "parser.tab.c"
    break;

  case 55:
#line 154 "parser.y"
                                                           { printf("LD [%d], %d\n", (yyvsp[-3].reg_num), (yyvsp[0].reg_num)); instruction_ld_st("ld","mem_reg",(yyvsp[0].reg_num),0,"",0,"",(yyvsp[-3].reg_num),0,0,0);}
#line 1770 "parser.tab.c"
    break;

  case 56:
#line 155 "parser.y"
                                                                        { printf("LD [%d + %d], %d\n", (yyvsp[-5].reg_num), (yyvsp[-3].val1), (yyvsp[0].reg_num)); instruction_ld_st("ld","mem_reg_literal",(yyvsp[0].reg_num),(yyvsp[-3].val1),"",0,"",(yyvsp[-5].reg_num),0,0,0);}
#line 1776 "parser.tab.c"
    break;

  case 57:
#line 156 "parser.y"
                                                                       { printf("LD [%d + %s], %d\n", (yyvsp[-5].reg_num), (yyvsp[-3].val_str), (yyvsp[0].reg_num)); instruction_ld_st("ld","mem_reg_symbol",(yyvsp[0].reg_num),0,(yyvsp[-3].val_str),0,"",(yyvsp[-5].reg_num),0,0,0);}
#line 1782 "parser.tab.c"
    break;

  case 58:
#line 157 "parser.y"
                                         { printf("ST %d, %s\n", (yyvsp[-2].reg_num), (yyvsp[0].val_str)); instruction_ld_st("st","dollar_symbol",(yyvsp[-2].reg_num),0,"",0,(yyvsp[0].val_str),0,0,0,0);}
#line 1788 "parser.tab.c"
    break;

  case 59:
#line 158 "parser.y"
                                          { printf("ST %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].val1)); instruction_ld_st("st","dollar_literal",(yyvsp[-2].reg_num),0,"",(yyvsp[0].val1),"",0,0,0,0);}
#line 1794 "parser.tab.c"
    break;

  case 60:
#line 159 "parser.y"
                                  { printf("ST %d, %s\n", (yyvsp[-2].reg_num), (yyvsp[0].val_str)); instruction_ld_st("st","symbol",(yyvsp[-2].reg_num),0,(yyvsp[0].val_str),0,"",0,0,0,0);}
#line 1800 "parser.tab.c"
    break;

  case 61:
#line 160 "parser.y"
                                   { printf("ST %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].val1)); instruction_ld_st("st","literal",(yyvsp[-2].reg_num),(yyvsp[0].val1),"",0,"",0,0,0,0);}
#line 1806 "parser.tab.c"
    break;

  case 62:
#line 161 "parser.y"
                                { printf("ST %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_ld_st("st","reg",(yyvsp[-2].reg_num),0,"",0,"",(yyvsp[0].reg_num),0,0,0);}
#line 1812 "parser.tab.c"
    break;

  case 63:
#line 162 "parser.y"
                                                           { printf("ST %d, [%d]\n", (yyvsp[-4].reg_num), (yyvsp[-1].reg_num)); instruction_ld_st("st","mem_reg",(yyvsp[-4].reg_num),0,"",0,"",(yyvsp[-1].reg_num),0,0,0);}
#line 1818 "parser.tab.c"
    break;

  case 64:
#line 163 "parser.y"
                                                                        { printf("ST %d, [%d + %d]\n", (yyvsp[-6].reg_num), (yyvsp[-3].reg_num), (yyvsp[-1].val1)); instruction_ld_st("st","mem_reg_literal",(yyvsp[-6].reg_num),(yyvsp[-1].val1),"",0,"",(yyvsp[-3].reg_num),0,0,0);}
#line 1824 "parser.tab.c"
    break;

  case 65:
#line 164 "parser.y"
                                                                       { printf("ST %d, [%d + %s]\n", (yyvsp[-6].reg_num), (yyvsp[-3].reg_num), (yyvsp[-1].val_str)); instruction_ld_st("st","mem_reg_symbol",(yyvsp[-6].reg_num),0,(yyvsp[-1].val_str),0,"",(yyvsp[-3].reg_num),0,0,0);}
#line 1830 "parser.tab.c"
    break;

  case 66:
#line 165 "parser.y"
                                   { printf("CSRRD %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_csrrd_csrrw("csrrd", (yyvsp[-2].reg_num), (yyvsp[0].reg_num));}
#line 1836 "parser.tab.c"
    break;

  case 67:
#line 166 "parser.y"
                                   { printf("CSRWR %d, %d\n", (yyvsp[-2].reg_num), (yyvsp[0].reg_num)); instruction_csrrd_csrrw("csrrw", (yyvsp[0].reg_num), (yyvsp[-2].reg_num));}
#line 1842 "parser.tab.c"
    break;

  case 68:
#line 169 "parser.y"
                   { 
    printf("END\n");
    printSymbolTable();
    printFlinkTable();
    processFlinkTable();
    printRelocationTables();
    YYACCEPT; //This macro will make the parser immediately accept the input and stop further parsing
}
#line 1855 "parser.tab.c"
    break;

  case 69:
#line 178 "parser.y"
                                      { addToSymbolList((yyvsp[0].val_str)); }
#line 1861 "parser.tab.c"
    break;

  case 70:
#line 179 "parser.y"
                    { /* process single symbol */ addToSymbolList((yyvsp[0].val_str)); }
#line 1867 "parser.tab.c"
    break;

  case 71:
#line 181 "parser.y"
                                                      { addSymbolToWordList((yyvsp[0].val_str)); }
#line 1873 "parser.tab.c"
    break;

  case 72:
#line 182 "parser.y"
                                               { addLiteralToWordList((yyvsp[0].val1)); }
#line 1879 "parser.tab.c"
    break;

  case 73:
#line 183 "parser.y"
                    { /* process single symbol */ addSymbolToWordList((yyvsp[0].val_str)); }
#line 1885 "parser.tab.c"
    break;

  case 74:
#line 184 "parser.y"
                     { /* process single literal */ addLiteralToWordList((yyvsp[0].val1)); }
#line 1891 "parser.tab.c"
    break;


#line 1895 "parser.tab.c"

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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp);
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
#line 186 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Parser error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Error opening file");
        return 1;
    }

    yyin = f;
    yyparse();
    fclose(f);

    return 0;
}
