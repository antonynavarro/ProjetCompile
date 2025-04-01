/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/tpc-2024-2025.y"

#include <string.h>
#include "tree.h"
#include "compilateur.h"

extern FILE *yyin;
extern int lineno;  

void yyerror(char * s);
int yylex(void);
Node *root;

#line 84 "obj/y.tab.c"

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

#include "y.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENT = 3,                      /* IDENT  */
  YYSYMBOL_STATIC = 4,                     /* STATIC  */
  YYSYMBOL_VOID = 5,                       /* VOID  */
  YYSYMBOL_IF = 6,                         /* IF  */
  YYSYMBOL_ELSE = 7,                       /* ELSE  */
  YYSYMBOL_WHILE = 8,                      /* WHILE  */
  YYSYMBOL_RETURN = 9,                     /* RETURN  */
  YYSYMBOL_CHARACTER = 10,                 /* CHARACTER  */
  YYSYMBOL_ADDSUB = 11,                    /* ADDSUB  */
  YYSYMBOL_DIVSTAR = 12,                   /* DIVSTAR  */
  YYSYMBOL_TYPE = 13,                      /* TYPE  */
  YYSYMBOL_NUM = 14,                       /* NUM  */
  YYSYMBOL_ORDER = 15,                     /* ORDER  */
  YYSYMBOL_EQ = 16,                        /* EQ  */
  YYSYMBOL_OR = 17,                        /* OR  */
  YYSYMBOL_AND = 18,                       /* AND  */
  YYSYMBOL_19_ = 19,                       /* '{'  */
  YYSYMBOL_20_ = 20,                       /* '}'  */
  YYSYMBOL_21_ = 21,                       /* '('  */
  YYSYMBOL_22_ = 22,                       /* ')'  */
  YYSYMBOL_23_ = 23,                       /* '!'  */
  YYSYMBOL_24_ = 24,                       /* ';'  */
  YYSYMBOL_25_ = 25,                       /* ','  */
  YYSYMBOL_THEN = 26,                      /* THEN  */
  YYSYMBOL_27_ = 27,                       /* '='  */
  YYSYMBOL_YYACCEPT = 28,                  /* $accept  */
  YYSYMBOL_Prog = 29,                      /* Prog  */
  YYSYMBOL_DeclVarsGlobal = 30,            /* DeclVarsGlobal  */
  YYSYMBOL_DeclVars = 31,                  /* DeclVars  */
  YYSYMBOL_Declarateurs = 32,              /* Declarateurs  */
  YYSYMBOL_DeclFoncts = 33,                /* DeclFoncts  */
  YYSYMBOL_DeclFonct = 34,                 /* DeclFonct  */
  YYSYMBOL_EnTeteFonct = 35,               /* EnTeteFonct  */
  YYSYMBOL_Parametres = 36,                /* Parametres  */
  YYSYMBOL_ListTypVar = 37,                /* ListTypVar  */
  YYSYMBOL_Corps = 38,                     /* Corps  */
  YYSYMBOL_SuiteInstr = 39,                /* SuiteInstr  */
  YYSYMBOL_Instr = 40,                     /* Instr  */
  YYSYMBOL_Exp = 41,                       /* Exp  */
  YYSYMBOL_TB = 42,                        /* TB  */
  YYSYMBOL_FB = 43,                        /* FB  */
  YYSYMBOL_M = 44,                         /* M  */
  YYSYMBOL_E = 45,                         /* E  */
  YYSYMBOL_T = 46,                         /* T  */
  YYSYMBOL_F = 47,                         /* F  */
  YYSYMBOL_Arguments = 48,                 /* Arguments  */
  YYSYMBOL_ListExp = 49                    /* ListExp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_int8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYLAST   112

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  53
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  108

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   274


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    23,     2,     2,     2,     2,     2,     2,
      21,    22,     2,     2,    25,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    24,
       2,    27,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    19,     2,    20,     2,     2,     2,     2,
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
      15,    16,    17,    18,    26
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    38,    38,    50,    57,    62,    68,    77,    83,    87,
      91,    95,    99,   107,   115,   126,   127,   131,   137,   143,
     151,   155,   159,   164,   171,   179,   184,   188,   192,   193,
     194,   197,   202,   205,   210,   213,   219,   222,   228,   231,
     237,   240,   246,   249,   254,   255,   256,   261,   266,   267,
     274,   278,   282,   286
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENT", "STATIC",
  "VOID", "IF", "ELSE", "WHILE", "RETURN", "CHARACTER", "ADDSUB",
  "DIVSTAR", "TYPE", "NUM", "ORDER", "EQ", "OR", "AND", "'{'", "'}'",
  "'('", "')'", "'!'", "';'", "','", "THEN", "'='", "$accept", "Prog",
  "DeclVarsGlobal", "DeclVars", "Declarateurs", "DeclFoncts", "DeclFonct",
  "EnTeteFonct", "Parametres", "ListTypVar", "Corps", "SuiteInstr",
  "Instr", "Exp", "TB", "FB", "M", "E", "T", "F", "Arguments", "ListExp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-72)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -72,    12,    24,   -72,     0,    66,    40,   -72,    55,    59,
      60,    36,    68,   -72,   -72,   -72,    45,    45,   -72,    72,
      60,    43,   -72,    79,    21,    58,    62,   -72,    73,    82,
      -1,   -72,   -72,    74,   -72,    82,   -72,    52,    13,    67,
      69,     3,   -72,   -72,   -72,   -72,    86,    54,   -72,    41,
      41,    41,    41,    70,   -72,    41,   -72,    41,    41,   -72,
      -2,    75,    76,    80,    83,    84,   -72,    22,   -72,   -72,
      81,    77,    78,    42,    48,    50,    41,   -72,    51,   -72,
      41,   -72,    41,    41,    41,    41,    41,   -72,    85,    41,
     -72,    30,    30,    88,   -72,    75,    76,    80,    83,    84,
     -72,   -72,    81,    90,   -72,   -72,    30,   -72
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     0,     1,     0,     0,     2,    11,     0,     0,
       9,     0,     0,    10,     7,    12,     0,     0,     3,     0,
       0,    21,    15,     0,     0,    16,     0,     8,     0,     0,
       0,    18,    14,     0,    13,     0,     9,     0,     0,     0,
       0,     0,    21,    19,    30,    20,     0,     0,     5,    51,
       0,     0,     0,    48,    47,     0,    46,     0,     0,    28,
       0,    32,    34,    36,    38,    40,    42,     0,    17,     6,
      53,     0,    50,     0,     0,     0,    51,    43,     0,    44,
       0,    27,     0,     0,     0,     0,     0,    29,     0,     0,
      22,     0,     0,     0,    45,    31,    33,    35,    37,    39,
      41,    26,    52,    23,    25,    49,     0,    24
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -72,   -72,   -72,   -72,    28,   -72,    94,   -72,    87,   -72,
     -72,    63,   -71,   -41,    26,    19,    25,    18,    27,   -54,
      31,   -72
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    21,    11,     6,     7,     8,    24,    25,
      15,    30,    45,    70,    61,    62,    63,    64,    65,    66,
      71,    72
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      60,    77,    38,     9,    79,    39,    53,    40,    41,    73,
      74,    75,     3,    54,    55,    80,    78,    56,    42,    43,
     103,   104,    81,    44,    57,    38,    58,    59,    39,     4,
      40,    41,   100,    38,    49,   107,    39,     5,    40,    41,
      50,    42,    87,    32,    53,     4,    44,    28,   102,    42,
      22,    54,    55,    12,    44,    56,    29,    37,    23,    80,
      18,    19,    57,    47,    58,    80,    90,    80,    80,    10,
      91,    20,    92,    94,    14,    27,    48,    19,    69,    19,
      16,    17,    31,    33,    34,    36,    35,    46,    51,    68,
      52,    76,    83,    82,    85,    84,    86,   106,    80,    88,
      13,    96,    98,    89,    26,    67,    95,    93,    97,   101,
     105,     0,    99
};

static const yytype_int8 yycheck[] =
{
      41,    55,     3,     3,    58,     6,     3,     8,     9,    50,
      51,    52,     0,    10,    11,    17,    57,    14,    19,    20,
      91,    92,    24,    24,    21,     3,    23,    24,     6,     5,
       8,     9,    86,     3,    21,   106,     6,    13,     8,     9,
      27,    19,    20,    22,     3,     5,    24,     4,    89,    19,
       5,    10,    11,    13,    24,    14,    13,    29,    13,    17,
      24,    25,    21,    35,    23,    17,    24,    17,    17,     3,
      22,     3,    22,    22,    19,     3,    24,    25,    24,    25,
      21,    21,     3,    25,    22,     3,    13,    13,    21,     3,
      21,    21,    16,    18,    11,    15,    12,     7,    17,    22,
       6,    82,    84,    25,    17,    42,    80,    76,    83,    24,
      22,    -1,    85
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    29,    30,     0,     5,    13,    33,    34,    35,     3,
       3,    32,    13,    34,    19,    38,    21,    21,    24,    25,
       3,    31,     5,    13,    36,    37,    36,     3,     4,    13,
      39,     3,    22,    25,    22,    13,     3,    32,     3,     6,
       8,     9,    19,    20,    24,    40,    13,    32,    24,    21,
      27,    21,    21,     3,    10,    11,    14,    21,    23,    24,
      41,    42,    43,    44,    45,    46,    47,    39,     3,    24,
      41,    48,    49,    41,    41,    41,    21,    47,    41,    47,
      17,    24,    18,    16,    15,    11,    12,    20,    22,    25,
      24,    22,    22,    48,    22,    42,    43,    44,    45,    46,
      47,    24,    41,    40,    40,    22,     7,    40
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    28,    29,    30,    30,    31,    31,    31,    32,    32,
      33,    33,    34,    35,    35,    36,    36,    37,    37,    38,
      39,    39,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    41,    41,    42,    42,    43,    43,    44,    44,    45,
      45,    46,    46,    47,    47,    47,    47,    47,    47,    47,
      48,    48,    49,    49
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     4,     0,     4,     5,     0,     3,     1,
       2,     1,     2,     5,     5,     1,     1,     4,     2,     4,
       2,     0,     4,     5,     7,     5,     5,     3,     2,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     2,     2,     3,     1,     1,     1,     4,
       1,     0,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* Prog: DeclVarsGlobal DeclFoncts  */
#line 38 "src/tpc-2024-2025.y"
                             {    
        root = makeNode("Program");
        Node * funs = makeNode("Functions");
        Node * global = (yyvsp[-1].node);
        if (global->firstChild) addChild(root, (yyvsp[-1].node));
        else { deleteTree(global);}
        addChild(root, funs);
        addChild(funs, (yyvsp[0].node));
    }
#line 1192 "obj/y.tab.c"
    break;

  case 3: /* DeclVarsGlobal: DeclVarsGlobal TYPE Declarateurs ';'  */
#line 50 "src/tpc-2024-2025.y"
                                            {
           (yyval.node) = (yyvsp[-3].node);
           Node * t = makeNode((yyvsp[-2].ident));
           addChild((yyval.node), t);
           addChild(t, (yyvsp[-1].node));
       }
#line 1203 "obj/y.tab.c"
    break;

  case 4: /* DeclVarsGlobal: %empty  */
#line 57 "src/tpc-2024-2025.y"
       { (yyval.node) = makeNode("Global"); }
#line 1209 "obj/y.tab.c"
    break;

  case 5: /* DeclVars: DeclVars TYPE Declarateurs ';'  */
#line 62 "src/tpc-2024-2025.y"
                                     {
            (yyval.node) = (yyvsp[-3].node);
            Node * t = makeNode((yyvsp[-2].ident));
            addChild((yyval.node), t);
            addChild(t, (yyvsp[-1].node));
       }
#line 1220 "obj/y.tab.c"
    break;

  case 6: /* DeclVars: DeclVars STATIC TYPE Declarateurs ';'  */
#line 68 "src/tpc-2024-2025.y"
                                            {
            (yyval.node) = (yyvsp[-4].node);
            Node * s = makeNode("Static");
            Node * t = makeNode((yyvsp[-2].ident));
            addChild((yyval.node), s);
            addChild(s, t);
            addChild(t, (yyvsp[-1].node));
       }
#line 1233 "obj/y.tab.c"
    break;

  case 7: /* DeclVars: %empty  */
#line 77 "src/tpc-2024-2025.y"
       { 
        (yyval.node) = makeNode("Vars");
       }
#line 1241 "obj/y.tab.c"
    break;

  case 8: /* Declarateurs: Declarateurs ',' IDENT  */
#line 83 "src/tpc-2024-2025.y"
                             {
            (yyval.node) = makeNode((yyvsp[0].ident));
            addSibling((yyval.node), (yyvsp[-2].node));
       }
#line 1250 "obj/y.tab.c"
    break;

  case 9: /* Declarateurs: IDENT  */
#line 87 "src/tpc-2024-2025.y"
             {(yyval.node) = makeNode((yyvsp[0].ident));}
#line 1256 "obj/y.tab.c"
    break;

  case 10: /* DeclFoncts: DeclFoncts DeclFonct  */
#line 91 "src/tpc-2024-2025.y"
                           {
            (yyval.node) = (yyvsp[-1].node);
            addSibling((yyval.node), (yyvsp[0].node));
       }
#line 1265 "obj/y.tab.c"
    break;

  case 11: /* DeclFoncts: DeclFonct  */
#line 95 "src/tpc-2024-2025.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1271 "obj/y.tab.c"
    break;

  case 12: /* DeclFonct: EnTeteFonct Corps  */
#line 99 "src/tpc-2024-2025.y"
                         {
            (yyval.node) = makeNode("Function");
            addChild((yyval.node), (yyvsp[-1].node));
            addChild((yyval.node), (yyvsp[0].node));
       }
#line 1281 "obj/y.tab.c"
    break;

  case 13: /* EnTeteFonct: TYPE IDENT '(' Parametres ')'  */
#line 107 "src/tpc-2024-2025.y"
                                    {
            (yyval.node) = makeNode("Head");
            addChild((yyval.node), makeNode((yyvsp[-4].ident)));
            addChild((yyval.node), makeNode((yyvsp[-3].ident)));
            Node * p = makeNode("Parameter");
            addChild((yyval.node), p);
            addChild(p, (yyvsp[-1].node));
       }
#line 1294 "obj/y.tab.c"
    break;

  case 14: /* EnTeteFonct: VOID IDENT '(' Parametres ')'  */
#line 115 "src/tpc-2024-2025.y"
                                    {
            (yyval.node) = makeNode("Head");
            addChild((yyval.node), makeNode((yyvsp[-4].ident)));
            addChild((yyval.node), makeNode((yyvsp[-3].ident)));
            Node * p = makeNode("Parameter");
            addChild((yyval.node), p);
            addChild(p, (yyvsp[-1].node));
        }
#line 1307 "obj/y.tab.c"
    break;

  case 15: /* Parametres: VOID  */
#line 126 "src/tpc-2024-2025.y"
            { (yyval.node) = makeNode((yyvsp[0].ident)); }
#line 1313 "obj/y.tab.c"
    break;

  case 16: /* Parametres: ListTypVar  */
#line 127 "src/tpc-2024-2025.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1319 "obj/y.tab.c"
    break;

  case 17: /* ListTypVar: ListTypVar ',' TYPE IDENT  */
#line 131 "src/tpc-2024-2025.y"
                                {
            (yyval.node) = (yyvsp[-3].node);
            Node * t = makeNode((yyvsp[-1].ident));
            addSibling((yyval.node), t);
            addChild(t, makeNode((yyvsp[0].ident)));
        }
#line 1330 "obj/y.tab.c"
    break;

  case 18: /* ListTypVar: TYPE IDENT  */
#line 137 "src/tpc-2024-2025.y"
                 {
            (yyval.node) = makeNode((yyvsp[-1].ident));
            addChild((yyval.node), makeNode((yyvsp[0].ident)));
        }
#line 1339 "obj/y.tab.c"
    break;

  case 19: /* Corps: '{' DeclVars SuiteInstr '}'  */
#line 143 "src/tpc-2024-2025.y"
                                  {
            (yyval.node) = makeNode("Body");
            addChild((yyval.node), (yyvsp[-2].node));
            addChild((yyval.node), (yyvsp[-1].node));
        }
#line 1349 "obj/y.tab.c"
    break;

  case 20: /* SuiteInstr: SuiteInstr Instr  */
#line 151 "src/tpc-2024-2025.y"
                       {
            (yyval.node) = (yyvsp[-1].node);
            addChild((yyval.node), (yyvsp[0].node));
        }
#line 1358 "obj/y.tab.c"
    break;

  case 21: /* SuiteInstr: %empty  */
#line 155 "src/tpc-2024-2025.y"
      { (yyval.node) = makeNode("Suite_instr"); }
#line 1364 "obj/y.tab.c"
    break;

  case 22: /* Instr: IDENT '=' Exp ';'  */
#line 159 "src/tpc-2024-2025.y"
                         {
            (yyval.node) = makeNode("Eq");
            addChild((yyval.node), makeNode((yyvsp[-3].ident)));
            addChild((yyval.node), (yyvsp[-1].node));
       }
#line 1374 "obj/y.tab.c"
    break;

  case 23: /* Instr: IF '(' Exp ')' Instr  */
#line 164 "src/tpc-2024-2025.y"
                                        {
            (yyval.node) =  makeNode("If");
            addChild((yyval.node), (yyvsp[-2].node));
            Node * other = makeNode("Then");
            addSibling((yyval.node), other);
            addChild(other, (yyvsp[0].node));
       }
#line 1386 "obj/y.tab.c"
    break;

  case 24: /* Instr: IF '(' Exp ')' Instr ELSE Instr  */
#line 171 "src/tpc-2024-2025.y"
                                       {
            (yyval.node) = makeNode("If");
            addChild((yyval.node), (yyvsp[-4].node));
            addChild((yyval.node), (yyvsp[-2].node));
            Node * other = makeNode("Else");
            addSibling((yyval.node), other);
            addChild(other, (yyvsp[0].node));
       }
#line 1399 "obj/y.tab.c"
    break;

  case 25: /* Instr: WHILE '(' Exp ')' Instr  */
#line 179 "src/tpc-2024-2025.y"
                               {
            (yyval.node) = makeNode("While");
            addChild((yyval.node), (yyvsp[-2].node));
            addChild((yyval.node), (yyvsp[0].node));
       }
#line 1409 "obj/y.tab.c"
    break;

  case 26: /* Instr: IDENT '(' Arguments ')' ';'  */
#line 184 "src/tpc-2024-2025.y"
                                    {
            (yyval.node) = makeNode((yyvsp[-4].ident));
            addChild((yyval.node), (yyvsp[-2].node));
       }
#line 1418 "obj/y.tab.c"
    break;

  case 27: /* Instr: RETURN Exp ';'  */
#line 188 "src/tpc-2024-2025.y"
                      {
            (yyval.node) = makeNode("Return");
            addChild((yyval.node), (yyvsp[-1].node));
       }
#line 1427 "obj/y.tab.c"
    break;

  case 28: /* Instr: RETURN ';'  */
#line 192 "src/tpc-2024-2025.y"
                  { (yyval.node) =  makeNode("Return"); }
#line 1433 "obj/y.tab.c"
    break;

  case 29: /* Instr: '{' SuiteInstr '}'  */
#line 193 "src/tpc-2024-2025.y"
                          { (yyval.node) = (yyvsp[-1].node); }
#line 1439 "obj/y.tab.c"
    break;

  case 30: /* Instr: ';'  */
#line 194 "src/tpc-2024-2025.y"
           {(yyval.node) = NULL;}
#line 1445 "obj/y.tab.c"
    break;

  case 31: /* Exp: Exp OR TB  */
#line 197 "src/tpc-2024-2025.y"
                 {
            (yyval.node) = makeNode("Or");
            addChild((yyval.node), (yyvsp[-2].node));
            addChild((yyval.node), (yyvsp[0].node));
        }
#line 1455 "obj/y.tab.c"
    break;

  case 32: /* Exp: TB  */
#line 202 "src/tpc-2024-2025.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1461 "obj/y.tab.c"
    break;

  case 33: /* TB: TB AND FB  */
#line 205 "src/tpc-2024-2025.y"
                 {
            (yyval.node) = makeNode("And");
            addChild((yyval.node), (yyvsp[-2].node));
            addChild((yyval.node), (yyvsp[0].node));
        }
#line 1471 "obj/y.tab.c"
    break;

  case 34: /* TB: FB  */
#line 210 "src/tpc-2024-2025.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 1477 "obj/y.tab.c"
    break;

  case 35: /* FB: FB EQ M  */
#line 213 "src/tpc-2024-2025.y"
               {
            (yyval.node) = makeNode("Eq");
            (yyval.node)->value = (yyvsp[-1].ident);
            addChild((yyval.node), (yyvsp[-2].node));
            addChild((yyval.node), (yyvsp[0].node));
        }
#line 1488 "obj/y.tab.c"
    break;

  case 36: /* FB: M  */
#line 219 "src/tpc-2024-2025.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1494 "obj/y.tab.c"
    break;

  case 37: /* M: M ORDER E  */
#line 222 "src/tpc-2024-2025.y"
                 {
            (yyval.node) = makeNode("Order");
            (yyval.node)->value = (yyvsp[-1].ident);
            addChild((yyval.node), (yyvsp[-2].node));
            addChild((yyval.node), (yyvsp[0].node));
        }
#line 1505 "obj/y.tab.c"
    break;

  case 38: /* M: E  */
#line 228 "src/tpc-2024-2025.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1511 "obj/y.tab.c"
    break;

  case 39: /* E: E ADDSUB T  */
#line 231 "src/tpc-2024-2025.y"
                  {
            (yyval.node) = makeNode("AddSub");
            (yyval.node)->value = (yyvsp[-1].ident);
            addChild((yyval.node), (yyvsp[-2].node));
            addChild((yyval.node), (yyvsp[0].node));
        }
#line 1522 "obj/y.tab.c"
    break;

  case 40: /* E: T  */
#line 237 "src/tpc-2024-2025.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1528 "obj/y.tab.c"
    break;

  case 41: /* T: T DIVSTAR F  */
#line 240 "src/tpc-2024-2025.y"
                   {
            (yyval.node) = makeNode("DivStar");
            (yyval.node)->value = (yyvsp[-1].ident);
            addChild((yyval.node), (yyvsp[-2].node));
            addChild((yyval.node), (yyvsp[0].node));
        }
#line 1539 "obj/y.tab.c"
    break;

  case 42: /* T: F  */
#line 246 "src/tpc-2024-2025.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 1545 "obj/y.tab.c"
    break;

  case 43: /* F: ADDSUB F  */
#line 249 "src/tpc-2024-2025.y"
                {
            (yyval.node) = makeNode("AddSub");
            (yyval.node)->value = (yyvsp[-1].ident);
            addChild((yyval.node), (yyvsp[0].node));
        }
#line 1555 "obj/y.tab.c"
    break;

  case 44: /* F: '!' F  */
#line 254 "src/tpc-2024-2025.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1561 "obj/y.tab.c"
    break;

  case 45: /* F: '(' Exp ')'  */
#line 255 "src/tpc-2024-2025.y"
                   { (yyval.node) = (yyvsp[-1].node); }
#line 1567 "obj/y.tab.c"
    break;

  case 46: /* F: NUM  */
#line 256 "src/tpc-2024-2025.y"
           { 
            char str[50];
            sprintf(str, "%d", (yyvsp[0].num));
            (yyval.node) = makeNode(str);
            }
#line 1577 "obj/y.tab.c"
    break;

  case 47: /* F: CHARACTER  */
#line 261 "src/tpc-2024-2025.y"
                 { 
                char str[50];
                sprintf(str, "'%c'", (yyvsp[0].byte));
                (yyval.node) = makeNode(str); 
                }
#line 1587 "obj/y.tab.c"
    break;

  case 48: /* F: IDENT  */
#line 266 "src/tpc-2024-2025.y"
             { (yyval.node) = makeNode((yyvsp[0].ident)); }
#line 1593 "obj/y.tab.c"
    break;

  case 49: /* F: IDENT '(' Arguments ')'  */
#line 267 "src/tpc-2024-2025.y"
                               {
            (yyval.node) = makeNode((yyvsp[-3].ident));
            addChild((yyval.node), (yyvsp[-1].node));
        }
#line 1602 "obj/y.tab.c"
    break;

  case 50: /* Arguments: ListExp  */
#line 274 "src/tpc-2024-2025.y"
               {
            (yyval.node) = makeNode("Args");
            addChild((yyval.node), (yyvsp[0].node));
        }
#line 1611 "obj/y.tab.c"
    break;

  case 51: /* Arguments: %empty  */
#line 278 "src/tpc-2024-2025.y"
      { (yyval.node) = NULL; }
#line 1617 "obj/y.tab.c"
    break;

  case 52: /* ListExp: ListExp ',' Exp  */
#line 282 "src/tpc-2024-2025.y"
                       {
        (yyval.node) = (yyvsp[0].node);
        addSibling((yyval.node), (yyvsp[-2].node));
       }
#line 1626 "obj/y.tab.c"
    break;

  case 53: /* ListExp: Exp  */
#line 286 "src/tpc-2024-2025.y"
           {(yyval.node) = (yyvsp[0].node);}
#line 1632 "obj/y.tab.c"
    break;


#line 1636 "obj/y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 289 "src/tpc-2024-2025.y"

void print_help(char * prog){
  printf("Commande :\n");
  printf("  %s [OPTIONS] [FICHIER]\n", prog);
  printf("Options : \n");
  printf("  -t, --tree    Affiche l'arbre abstrai du fichier\n");
  printf("  -h, --help    Affiche cette aide\n");
}

int option (char * arg){
  if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0){ return 1; }
  else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--tree") == 0) { return 2; }
  return 0;  
}


int main(int argc, char * argv[]) {

    int opt = 3;
      
    if (argc == 3){
        yyin = fopen(argv[2], "r");

        if(!yyin){
            fprintf(stderr, "Le fichier n'existe pas !\n");
            return 1;
        }

        opt = option(argv[1]);

        switch (opt){
        case 0:
            fprintf(stderr, "L'option %s n'est pas reconnue !\n", argv[1]);
            return 1;
        case 1:
            print_help(argv[0]);
            return 0;
        default:
        break;
        }

    } else if (argc == 2 ){
        opt = option(argv[1]);
        switch (opt){

        case 0:{
            yyin = fopen(argv[1], "r");
            if(!yyin){
                fprintf(stderr, "Le fichier n'existe pas !\n");
                return 1;
            }
            break;

        }
            
        case 1:
            print_help(argv[0]);
            return 0;
        default:
        break;
        }
    }
    else if( argc > 3){
        fprintf(stderr,"Le nombre d'Arguments est incorrect\n");
        return 1;
    }
    int parseResult  = yyparse();

    if (yyin != NULL){
        fclose(yyin);
    }

    if (parseResult == 0) {

        printf("Analyse réussie.\n");
        translate(root);

        if (opt == 2){
            printTree(root);
            deleteTree(root);
            
        }        
        
    } else 
        printf("Erreur pendant l'analyse à la ligne %d, code de retour : %d\n",lineno, parseResult);

    return parseResult ; 
}
