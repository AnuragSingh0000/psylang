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
#line 1 "psy.y"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "defs.h"
#include "ast_printer.h"
#include "semantic.h"

int yylex(void);
void yyerror(const char *);
extern int yylineno;
extern int yycolumn;
extern FILE *yyin;
const char *current_filename = "<stdin>";

static Node *rootAst = NULL;
static bool should_dump_ir(void)
{
    const char *v = getenv("DUMP_IR");
    return v && v[0] != '\0' && strcmp(v, "0") != 0;
}

static bool should_dump_ir_only(void)
{
    const char *v = getenv("DUMP_IR_ONLY");
    return v && v[0] != '\0' && strcmp(v, "0") != 0;
}

static bool should_debug(void)
{
    const char *v = getenv("PSY_DEBUG");
    return v && v[0] != '\0' && strcmp(v, "0") != 0;
}

static Node *makeNode(NodeType type, int line, int col)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        yyerror("Out of memory while constructing AST");
        exit(1);
    }

    memset(node, 0, sizeof(Node));
    node->type = type;
    node->line = line;
    node->pos = col;
    node->symbol_table_index = SYMBOL_INDEX_UNRESOLVED;
    node->next = NULL;

    return node;
}

static Node *appendList(Node *list, Node *value)
{
    Node *cur;
    if (!value) return list;

    if (!list)
    {
        value->next = NULL;
        return value;
    }

    cur = list;
    while (cur->next)
    {
        cur = cur->next;
    }
    cur->next = value;
    value->next = NULL;
    return list;
}

static Node *makePrimaryNode(PrimaryNodeType type, int line, int col)
{
    Node *node = makeNode(NODE_PRIMARY, line, col);
    node->as.primary.type = type;
    return node;
}

static Node *makeUnaryNode(OperatorType op, Node *expr, int line, int col)
{
    Node *node = makeNode(NODE_UNARY_OP, line, col);
    node->as.unary.op = op;
    node->as.unary.expr = expr;
    return node;
}

static Node *makeBinaryNode(OperatorType op, Node *left, Node *right, int line, int col)
{
    Node *node = makeNode(NODE_BINARY_OP, line, col);
    node->as.binary.op = op;
    node->as.binary.left = left;
    node->as.binary.right = right;
    return node;
}

static Node *makeAssignNode(OperatorType op, Node *left, Node *right, int line, int col)
{
    Node *node = makeNode(NODE_ASSIGN_OP, line, col);
    node->as.assign.op = op;
    node->as.assign.left = left;
    node->as.assign.right = right;
    return node;
}


#line 182 "psy.tab.c"

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

#include "psy.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT_CONSTANT = 3,               /* INT_CONSTANT  */
  YYSYMBOL_FLOAT_CONSTANT = 4,             /* FLOAT_CONSTANT  */
  YYSYMBOL_IDENTIFIER = 5,                 /* IDENTIFIER  */
  YYSYMBOL_STRING_CONSTANT = 6,            /* STRING_CONSTANT  */
  YYSYMBOL_BOOL_CONSTANT = 7,              /* BOOL_CONSTANT  */
  YYSYMBOL_T_NULL = 8,                     /* "null"  */
  YYSYMBOL_VAR = 9,                        /* "var"  */
  YYSYMBOL_INT = 10,                       /* "int"  */
  YYSYMBOL_FLOAT = 11,                     /* "float"  */
  YYSYMBOL_STRING = 12,                    /* "string"  */
  YYSYMBOL_BOOL = 13,                      /* "bool"  */
  YYSYMBOL_LIST = 14,                      /* "list"  */
  YYSYMBOL_DYN = 15,                       /* "dyn"  */
  YYSYMBOL_PLUS = 16,                      /* "+"  */
  YYSYMBOL_MINUS = 17,                     /* "-"  */
  YYSYMBOL_STAR = 18,                      /* "*"  */
  YYSYMBOL_SLASH = 19,                     /* "/"  */
  YYSYMBOL_PERC = 20,                      /* "%"  */
  YYSYMBOL_LESS = 21,                      /* "<"  */
  YYSYMBOL_GREATER = 22,                   /* ">"  */
  YYSYMBOL_LESS_EQ = 23,                   /* "<="  */
  YYSYMBOL_GREATER_EQ = 24,                /* ">="  */
  YYSYMBOL_BANG_EQ = 25,                   /* "!="  */
  YYSYMBOL_EQ_EQ = 26,                     /* "=="  */
  YYSYMBOL_AND = 27,                       /* "&"  */
  YYSYMBOL_DOUBLE_AND = 28,                /* "&&"  */
  YYSYMBOL_PIPE = 29,                      /* "|"  */
  YYSYMBOL_DOUBLE_PIPE = 30,               /* "||"  */
  YYSYMBOL_TILDE = 31,                     /* "~"  */
  YYSYMBOL_XOR = 32,                       /* "^"  */
  YYSYMBOL_SHIFT_LEFT = 33,                /* "<<"  */
  YYSYMBOL_SHIFT_RIGHT = 34,               /* ">>"  */
  YYSYMBOL_EQ = 35,                        /* "="  */
  YYSYMBOL_PLUS_EQ = 36,                   /* "+="  */
  YYSYMBOL_MINUS_EQ = 37,                  /* "-="  */
  YYSYMBOL_STAR_EQ = 38,                   /* "*="  */
  YYSYMBOL_SLASH_EQ = 39,                  /* "/="  */
  YYSYMBOL_PERC_EQ = 40,                   /* "%="  */
  YYSYMBOL_PLUS_PLUS = 41,                 /* "++"  */
  YYSYMBOL_MINUS_MINUS = 42,               /* "--"  */
  YYSYMBOL_QUESTION_MARK = 43,             /* "?"  */
  YYSYMBOL_BANG = 44,                      /* "!"  */
  YYSYMBOL_ECH = 45,                       /* "echo"  */
  YYSYMBOL_IF = 46,                        /* "if"  */
  YYSYMBOL_ELSE = 47,                      /* "else"  */
  YYSYMBOL_FOR = 48,                       /* "for"  */
  YYSYMBOL_DO = 49,                        /* "do"  */
  YYSYMBOL_WHILE = 50,                     /* "while"  */
  YYSYMBOL_BREAK = 51,                     /* "break"  */
  YYSYMBOL_CONTINUE = 52,                  /* "continue"  */
  YYSYMBOL_TYPE = 53,                      /* "type"  */
  YYSYMBOL_FUN = 54,                       /* "fun"  */
  YYSYMBOL_RET = 55,                       /* "ret"  */
  YYSYMBOL_ARROW = 56,                     /* "->"  */
  YYSYMBOL_LAMBDA = 57,                    /* "lambda"  */
  YYSYMBOL_LEFT_PAREN = 58,                /* "("  */
  YYSYMBOL_RIGHT_PAREN = 59,               /* ")"  */
  YYSYMBOL_LEFT_BRACE = 60,                /* "{"  */
  YYSYMBOL_RIGHT_BRACE = 61,               /* "}"  */
  YYSYMBOL_LEFT_SQUARE = 62,               /* "["  */
  YYSYMBOL_RIGHT_SQUARE = 63,              /* "]"  */
  YYSYMBOL_DOT = 64,                       /* "."  */
  YYSYMBOL_COLON = 65,                     /* ":"  */
  YYSYMBOL_SEMI_COLON = 66,                /* ";"  */
  YYSYMBOL_COMMA = 67,                     /* ","  */
  YYSYMBOL_LOWER_THAN_ELSE = 68,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 69,                  /* $accept  */
  YYSYMBOL_program = 70,                   /* program  */
  YYSYMBOL_decList = 71,                   /* decList  */
  YYSYMBOL_declaration = 72,               /* declaration  */
  YYSYMBOL_typeDecl = 73,                  /* typeDecl  */
  YYSYMBOL_funDecl = 74,                   /* funDecl  */
  YYSYMBOL_function = 75,                  /* function  */
  YYSYMBOL_parameters = 76,                /* parameters  */
  YYSYMBOL_typedVar = 77,                  /* typedVar  */
  YYSYMBOL_var_type = 78,                  /* var_type  */
  YYSYMBOL_inTypeDeclList = 79,            /* inTypeDeclList  */
  YYSYMBOL_varDecl = 80,                   /* varDecl  */
  YYSYMBOL_statement = 81,                 /* statement  */
  YYSYMBOL_exprStmt = 82,                  /* exprStmt  */
  YYSYMBOL_forStmt = 83,                   /* forStmt  */
  YYSYMBOL_forInit = 84,                   /* forInit  */
  YYSYMBOL_optExpr = 85,                   /* optExpr  */
  YYSYMBOL_ifStmt = 86,                    /* ifStmt  */
  YYSYMBOL_echoStmt = 87,                  /* echoStmt  */
  YYSYMBOL_returnStmt = 88,                /* returnStmt  */
  YYSYMBOL_whileStmt = 89,                 /* whileStmt  */
  YYSYMBOL_dowhileStmt = 90,               /* dowhileStmt  */
  YYSYMBOL_breakStmt = 91,                 /* breakStmt  */
  YYSYMBOL_continueStmt = 92,              /* continueStmt  */
  YYSYMBOL_block = 93,                     /* block  */
  YYSYMBOL_expression = 94,                /* expression  */
  YYSYMBOL_assignment = 95,                /* assignment  */
  YYSYMBOL_ternary = 96,                   /* ternary  */
  YYSYMBOL_logic_or = 97,                  /* logic_or  */
  YYSYMBOL_logic_and = 98,                 /* logic_and  */
  YYSYMBOL_bitwise_or = 99,                /* bitwise_or  */
  YYSYMBOL_bitwise_xor = 100,              /* bitwise_xor  */
  YYSYMBOL_bitwise_and = 101,              /* bitwise_and  */
  YYSYMBOL_equality = 102,                 /* equality  */
  YYSYMBOL_comparison = 103,               /* comparison  */
  YYSYMBOL_shift = 104,                    /* shift  */
  YYSYMBOL_term = 105,                     /* term  */
  YYSYMBOL_factor = 106,                   /* factor  */
  YYSYMBOL_unary = 107,                    /* unary  */
  YYSYMBOL_call_access = 108,              /* call_access  */
  YYSYMBOL_primary = 109,                  /* primary  */
  YYSYMBOL_anonFunc = 110,                 /* anonFunc  */
  YYSYMBOL_arguments = 111,                /* arguments  */
  YYSYMBOL_elements = 112                  /* elements  */
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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  95
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   490

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  223

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   323


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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   166,   166,   175,   179,   186,   187,   188,   189,   193,
     202,   211,   220,   230,   231,   235,   244,   253,   262,   271,
     280,   289,   298,   307,   318,   319,   323,   332,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   355,   363,
     374,   375,   376,   380,   381,   385,   394,   404,   412,   419,
     428,   437,   446,   454,   462,   469,   478,   482,   483,   484,
     485,   486,   487,   488,   492,   501,   504,   505,   509,   510,
     514,   515,   519,   520,   524,   525,   529,   530,   531,   535,
     536,   537,   538,   539,   543,   544,   545,   549,   550,   551,
     555,   556,   557,   558,   562,   563,   564,   565,   566,   567,
     571,   579,   587,   595,   603,   611,   619,   622,   629,   631,
     637,   644,   650,   657,   659,   660,   667,   676,   684,   685,
     689,   690
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT_CONSTANT",
  "FLOAT_CONSTANT", "IDENTIFIER", "STRING_CONSTANT", "BOOL_CONSTANT",
  "\"null\"", "\"var\"", "\"int\"", "\"float\"", "\"string\"", "\"bool\"",
  "\"list\"", "\"dyn\"", "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"%\"",
  "\"<\"", "\">\"", "\"<=\"", "\">=\"", "\"!=\"", "\"==\"", "\"&\"",
  "\"&&\"", "\"|\"", "\"||\"", "\"~\"", "\"^\"", "\"<<\"", "\">>\"",
  "\"=\"", "\"+=\"", "\"-=\"", "\"*=\"", "\"/=\"", "\"%=\"", "\"++\"",
  "\"--\"", "\"?\"", "\"!\"", "\"echo\"", "\"if\"", "\"else\"", "\"for\"",
  "\"do\"", "\"while\"", "\"break\"", "\"continue\"", "\"type\"",
  "\"fun\"", "\"ret\"", "\"->\"", "\"lambda\"", "\"(\"", "\")\"", "\"{\"",
  "\"}\"", "\"[\"", "\"]\"", "\".\"", "\":\"", "\";\"", "\",\"",
  "LOWER_THAN_ELSE", "$accept", "program", "decList", "declaration",
  "typeDecl", "funDecl", "function", "parameters", "typedVar", "var_type",
  "inTypeDeclList", "varDecl", "statement", "exprStmt", "forStmt",
  "forInit", "optExpr", "ifStmt", "echoStmt", "returnStmt", "whileStmt",
  "dowhileStmt", "breakStmt", "continueStmt", "block", "expression",
  "assignment", "ternary", "logic_or", "logic_and", "bitwise_or",
  "bitwise_xor", "bitwise_and", "equality", "comparison", "shift", "term",
  "factor", "unary", "call_access", "primary", "anonFunc", "arguments",
  "elements", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-175)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     338,  -175,  -175,  -175,  -175,  -175,  -175,   107,   428,   428,
     428,   428,   428,   428,    -4,     5,   368,     9,   -60,   -58,
      51,    55,   152,    18,   428,   218,   171,    70,   338,  -175,
    -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,
    -175,  -175,  -175,  -175,    28,  -175,  -175,   -14,    76,    69,
      74,    83,    47,    65,    59,    80,    12,  -175,    43,  -175,
    -175,  -175,  -175,  -175,  -175,  -175,  -175,    54,  -175,   128,
    -175,    -7,  -175,  -175,  -175,  -175,    68,   428,    33,   104,
     428,  -175,  -175,   101,    18,  -175,    96,    -1,  -175,   105,
    -175,   278,  -175,  -175,   -52,  -175,  -175,  -175,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,  -175,  -175,   398,   428,   158,   162,   -22,
    -175,   108,  -175,  -175,  -175,   428,   113,   121,   163,  -175,
    -175,   116,   126,   -50,  -175,  -175,  -175,  -175,   428,    76,
     120,    69,    74,    83,    47,    65,    65,    59,    59,    59,
      59,    80,    80,    12,    12,  -175,  -175,  -175,  -175,  -175,
    -175,  -175,  -175,  -175,  -175,  -175,     2,   123,  -175,   124,
     428,  -175,   368,   125,  -175,   428,   368,    -2,  -175,   107,
     107,   134,   187,  -175,   428,  -175,   428,  -175,  -175,   129,
     151,   428,   140,  -175,  -175,  -175,  -175,   141,   107,  -175,
    -175,  -175,  -175,   368,   145,   139,  -175,   141,  -175,   368,
    -175,  -175,  -175
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,   109,   110,   112,   111,   107,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     4,
       5,     6,     7,     8,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,     0,    56,    63,    65,    67,    69,
      71,    73,    75,    78,    83,    86,    89,    93,    99,   106,
     113,    23,    22,    16,    17,    18,    19,     0,    20,     0,
      96,    99,    95,    97,    98,    94,     0,     0,     0,     0,
       0,    52,    53,     0,     0,    49,     0,     0,   117,     0,
      55,     0,   116,   121,     0,     1,     3,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,   105,     0,     0,     0,     0,     0,
      47,     0,    42,    40,    41,    44,     0,     0,     0,    10,
      48,     0,     0,     0,    14,   114,    54,   115,     0,    66,
       0,    68,    70,    72,    74,    76,    77,    80,    79,    82,
      81,    84,    85,    87,    88,    91,    90,    92,    57,    58,
      59,    60,    61,    62,   101,   119,     0,     0,   102,     0,
       0,    27,     0,     0,    43,     0,     0,     0,    25,     0,
       0,     0,     0,   120,     0,   100,     0,   103,    21,     0,
      46,    44,     0,    50,     9,    24,    15,     0,     0,    13,
      64,   118,    26,     0,     0,     0,    12,     0,    45,     0,
      51,    11,    39
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -175,  -175,   175,   -25,  -175,  -175,   122,  -175,    15,  -137,
    -175,   -76,   -16,   130,  -175,  -175,    16,  -175,  -175,  -175,
    -175,  -175,  -175,  -175,  -174,   -12,   -94,  -175,  -175,   118,
     111,   119,   117,   127,     4,    21,    -8,    19,    10,    37,
    -175,  -175,  -175,  -175
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    27,    28,    29,    30,    31,    88,   143,   144,    69,
     187,    32,    33,    34,    35,   135,   183,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,   176,    94
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      79,    76,   133,    96,   141,   150,    81,     7,    82,   191,
      86,   147,    89,   180,    93,   148,    98,   192,    70,    72,
      73,    74,    75,   168,   169,   170,   171,   172,   173,    99,
     114,   115,   116,   216,   123,   124,     1,     2,     3,     4,
       5,     6,     7,   221,   181,    71,    71,    71,    71,    71,
       8,   125,   206,   207,    77,   126,    83,   127,   142,   204,
      84,   195,   188,    78,     9,   131,    96,    80,   137,   196,
      95,   217,   104,   105,    10,    11,    87,    12,   117,   118,
     119,   120,   121,   122,   123,   124,   106,   107,   108,   109,
      23,    24,   110,   111,    97,    26,   112,   113,   101,   132,
     210,   125,   161,   162,   100,   126,   102,   127,   155,   156,
     103,   205,    61,   175,   177,    62,   128,    63,    64,    65,
      66,    67,    68,   184,   165,   166,   167,   157,   158,   159,
     160,   163,   164,   129,   130,    71,   193,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,   136,     1,     2,     3,     4,     5,
       6,   138,   140,   178,   145,   179,   200,   182,   199,     8,
     203,   185,     7,   202,     1,     2,     3,     4,     5,     6,
     186,   189,   190,     9,   211,   194,   197,   198,     8,   184,
     208,   201,   141,    10,    11,   212,    12,   218,   213,   215,
      91,    25,     9,   222,   219,   220,   139,   209,   134,    23,
      24,   151,    10,    11,    26,    12,   149,   214,    85,   153,
     152,     1,     2,     3,     4,     5,     6,     7,    23,    24,
     154,     0,     0,    26,    92,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,    12,    13,    14,     0,    15,    16,    17,    18,
      19,    20,    21,    22,     0,    23,    24,     0,    25,    90,
      26,     1,     2,     3,     4,     5,     6,     7,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,    12,    13,    14,     0,    15,    16,    17,    18,
      19,    20,    21,    22,     0,    23,    24,     0,    25,   146,
      26,     1,     2,     3,     4,     5,     6,     7,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
       0,     1,     2,     3,     4,     5,     6,     0,     0,    10,
      11,     0,    12,    13,    14,     8,    15,    16,    17,    18,
      19,    20,    21,    22,     0,    23,    24,     0,    25,     9,
      26,     1,     2,     3,     4,     5,     6,     0,     0,    10,
      11,     0,    12,    13,    14,     8,    15,    16,    17,    18,
      19,     0,     0,    22,     0,    23,    24,     0,    25,     9,
      26,     1,     2,     3,     4,     5,     6,     0,     0,    10,
      11,     0,    12,     0,     0,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,   174,     0,     9,
      26,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      11,     0,    12,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,     0,     0,     0,
      26
};

static const yytype_int16 yycheck[] =
{
      16,    13,    78,    28,     5,    99,    66,     9,    66,    59,
      22,    63,    24,    35,    26,    67,    30,    67,     8,     9,
      10,    11,    12,   117,   118,   119,   120,   121,   122,    43,
      18,    19,    20,   207,    41,    42,     3,     4,     5,     6,
       7,     8,     9,   217,    66,     8,     9,    10,    11,    12,
      17,    58,   189,   190,    58,    62,     5,    64,    59,    61,
       5,    59,   138,    58,    31,    77,    91,    58,    80,    67,
       0,   208,    25,    26,    41,    42,    58,    44,    35,    36,
      37,    38,    39,    40,    41,    42,    21,    22,    23,    24,
      57,    58,    33,    34,    66,    62,    16,    17,    29,    66,
     194,    58,   110,   111,    28,    62,    32,    64,   104,   105,
      27,   187,     5,   125,   126,     8,    62,    10,    11,    12,
      13,    14,    15,   135,   114,   115,   116,   106,   107,   108,
     109,   112,   113,     5,    66,    98,   148,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    50,     3,     4,     5,     6,     7,
       8,    60,    66,     5,    59,     3,   182,    59,   180,    17,
     186,    58,     9,   185,     3,     4,     5,     6,     7,     8,
      59,    65,    56,    31,   196,    65,    63,    63,    17,   201,
      56,    66,     5,    41,    42,    66,    44,   213,    47,    59,
      25,    60,    31,   219,    59,    66,    84,   192,    78,    57,
      58,   100,    41,    42,    62,    44,    98,   201,    66,   102,
     101,     3,     4,     5,     6,     7,     8,     9,    57,    58,
     103,    -1,    -1,    62,    63,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    44,    45,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    44,    45,    46,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,     3,     4,     5,     6,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    41,
      42,    -1,    44,    45,    46,    17,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    31,
      62,     3,     4,     5,     6,     7,     8,    -1,    -1,    41,
      42,    -1,    44,    45,    46,    17,    48,    49,    50,    51,
      52,    -1,    -1,    55,    -1,    57,    58,    -1,    60,    31,
      62,     3,     4,     5,     6,     7,     8,    -1,    -1,    41,
      42,    -1,    44,    -1,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    -1,    31,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      42,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    -1,    -1,    -1,
      62
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    17,    31,
      41,    42,    44,    45,    46,    48,    49,    50,    51,    52,
      53,    54,    55,    57,    58,    60,    62,    70,    71,    72,
      73,    74,    80,    81,    82,    83,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,     5,     8,    10,    11,    12,    13,    14,    15,    78,
     107,   108,   107,   107,   107,   107,    94,    58,    58,    81,
      58,    66,    66,     5,     5,    66,    94,    58,    75,    94,
      61,    71,    63,    94,   112,     0,    72,    66,    30,    43,
      28,    29,    32,    27,    25,    26,    21,    22,    23,    24,
      33,    34,    16,    17,    18,    19,    20,    35,    36,    37,
      38,    39,    40,    41,    42,    58,    62,    64,    62,     5,
      66,    94,    66,    80,    82,    84,    50,    94,    60,    75,
      66,     5,    59,    76,    77,    59,    61,    63,    67,    98,
      95,    99,   100,   101,   102,   103,   103,   104,   104,   104,
     104,   105,   105,   106,   106,   107,   107,   107,    95,    95,
      95,    95,    95,    95,    59,    94,   111,    94,     5,     3,
      35,    66,    59,    85,    94,    58,    59,    79,    80,    65,
      56,    59,    67,    94,    65,    59,    67,    63,    63,    94,
      81,    66,    94,    81,    61,    80,    78,    78,    56,    77,
      95,    94,    66,    47,    85,    59,    93,    78,    81,    59,
      66,    93,    81
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    69,    70,    71,    71,    72,    72,    72,    72,    73,
      74,    75,    75,    76,    76,    77,    78,    78,    78,    78,
      78,    78,    78,    78,    79,    79,    80,    80,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    82,    83,
      84,    84,    84,    85,    85,    86,    86,    87,    88,    88,
      89,    90,    91,    92,    93,    93,    94,    95,    95,    95,
      95,    95,    95,    95,    96,    96,    97,    97,    98,    98,
      99,    99,   100,   100,   101,   101,   102,   102,   102,   103,
     103,   103,   103,   103,   104,   104,   104,   105,   105,   105,
     106,   106,   106,   106,   107,   107,   107,   107,   107,   107,
     108,   108,   108,   108,   108,   108,   108,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   110,   111,   111,
     112,   112
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     5,
       3,     6,     5,     3,     1,     3,     1,     1,     1,     1,
       1,     4,     1,     1,     2,     1,     6,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     8,
       1,     1,     1,     1,     0,     7,     5,     3,     3,     2,
       5,     7,     2,     2,     3,     2,     1,     3,     3,     3,
       3,     3,     3,     1,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     2,     2,     2,     2,     2,     1,
       4,     3,     3,     4,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     2,     2,     3,     1,
       3,     1
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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
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
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif

#ifndef yytnamerr
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
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
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
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
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


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: decList  */
#line 167 "psy.y"
    {
        Node *node = makeNode(NODE_PROGRAM, (yyloc).first_line, (yyloc).first_column);
        node->as.program.declarations = (yyvsp[0].node);
        rootAst = node;
        (yyval.node) = node;
    }
#line 1892 "psy.tab.c"
    break;

  case 3: /* decList: decList declaration  */
#line 176 "psy.y"
    {
        (yyval.node) = appendList((yyvsp[-1].node), (yyvsp[0].node));
    }
#line 1900 "psy.tab.c"
    break;

  case 4: /* decList: declaration  */
#line 180 "psy.y"
    {
        (yyval.node) = appendList(NULL, (yyvsp[0].node));
    }
#line 1908 "psy.tab.c"
    break;

  case 5: /* declaration: typeDecl  */
#line 186 "psy.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 1914 "psy.tab.c"
    break;

  case 6: /* declaration: funDecl  */
#line 187 "psy.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1920 "psy.tab.c"
    break;

  case 7: /* declaration: varDecl  */
#line 188 "psy.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1926 "psy.tab.c"
    break;

  case 8: /* declaration: statement  */
#line 189 "psy.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1932 "psy.tab.c"
    break;

  case 9: /* typeDecl: "type" IDENTIFIER "{" inTypeDeclList "}"  */
#line 194 "psy.y"
    {
        Node *node = makeNode(NODE_TYPE_DECL, (yyloc).first_line, (yyloc).first_column);
        node->as.typeDecl.name = (yyvsp[-3].sval);
        node->as.typeDecl.fields = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 1943 "psy.tab.c"
    break;

  case 10: /* funDecl: "fun" IDENTIFIER function  */
#line 203 "psy.y"
    {
        Node *node = makeNode(NODE_FUN_DECL, (yyloc).first_line, (yyloc).first_column);
        node->as.funDecl.name = (yyvsp[-1].sval);
        node->as.funDecl.function = (yyvsp[0].node);
        (yyval.node) = node;
    }
#line 1954 "psy.tab.c"
    break;

  case 11: /* function: "(" parameters ")" "->" var_type block  */
#line 212 "psy.y"
    {
        Node *node = makeNode(NODE_FUNCTION, (yyloc).first_line, (yyloc).first_column);
        node->as.function.parameters = (yyvsp[-4].node);
        node->as.function.returnType = (yyvsp[-1].node);
        node->as.function.body = (yyvsp[0].node);
        (yyval.node) = node;
    }
#line 1966 "psy.tab.c"
    break;

  case 12: /* function: "(" ")" "->" var_type block  */
#line 221 "psy.y"
    {
        Node *node = makeNode(NODE_FUNCTION, (yyloc).first_line, (yyloc).first_column);
        node->as.function.parameters = NULL;
        node->as.function.returnType = (yyvsp[-1].node);
        node->as.function.body = (yyvsp[0].node);
        (yyval.node) = node;
    }
#line 1978 "psy.tab.c"
    break;

  case 13: /* parameters: parameters "," typedVar  */
#line 230 "psy.y"
                              { (yyval.node) = appendList((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1984 "psy.tab.c"
    break;

  case 14: /* parameters: typedVar  */
#line 231 "psy.y"
               { (yyval.node) = appendList(NULL, (yyvsp[0].node)); }
#line 1990 "psy.tab.c"
    break;

  case 15: /* typedVar: IDENTIFIER ":" var_type  */
#line 236 "psy.y"
    {
        Node *node = makeNode(NODE_TYPED_VAR, (yyloc).first_line, (yyloc).first_column);
        node->as.typedVar.name = (yyvsp[-2].sval);
        node->as.typedVar.varType = (yyvsp[0].node);
        (yyval.node) = node;
    }
#line 2001 "psy.tab.c"
    break;

  case 16: /* var_type: "int"  */
#line 245 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_TYPE, (yyloc).first_line, (yyloc).first_column);
        node->as.varType.kind = VAR_TYPE_INT;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        (yyval.node) = node;
    }
#line 2013 "psy.tab.c"
    break;

  case 17: /* var_type: "float"  */
#line 254 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_TYPE, (yyloc).first_line, (yyloc).first_column);
        node->as.varType.kind = VAR_TYPE_FLOAT;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        (yyval.node) = node;
    }
#line 2025 "psy.tab.c"
    break;

  case 18: /* var_type: "string"  */
#line 263 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_TYPE, (yyloc).first_line, (yyloc).first_column);
        node->as.varType.kind = VAR_TYPE_STRING;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        (yyval.node) = node;
    }
#line 2037 "psy.tab.c"
    break;

  case 19: /* var_type: "bool"  */
#line 272 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_TYPE, (yyloc).first_line, (yyloc).first_column);
        node->as.varType.kind = VAR_TYPE_BOOL;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        (yyval.node) = node;
    }
#line 2049 "psy.tab.c"
    break;

  case 20: /* var_type: "dyn"  */
#line 281 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_TYPE, (yyloc).first_line, (yyloc).first_column);
        node->as.varType.kind = VAR_TYPE_DYN;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        (yyval.node) = node;
    }
#line 2061 "psy.tab.c"
    break;

  case 21: /* var_type: "list" "[" INT_CONSTANT "]"  */
#line 290 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_TYPE, (yyloc).first_line, (yyloc).first_column);
        node->as.varType.kind = VAR_TYPE_LIST;
        node->as.varType.listSize = (yyvsp[-1].ival);
        node->as.varType.customTypeName = NULL;
        (yyval.node) = node;
    }
#line 2073 "psy.tab.c"
    break;

  case 22: /* var_type: "null"  */
#line 299 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_TYPE, (yyloc).first_line, (yyloc).first_column);
        node->as.varType.kind = VAR_TYPE_NULL;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        (yyval.node) = node;
    }
#line 2085 "psy.tab.c"
    break;

  case 23: /* var_type: IDENTIFIER  */
#line 308 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_TYPE, (yyloc).first_line, (yyloc).first_column);
        node->as.varType.kind = VAR_TYPE_CUSTOM;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = (yyvsp[0].sval);
        (yyval.node) = node;
    }
#line 2097 "psy.tab.c"
    break;

  case 24: /* inTypeDeclList: inTypeDeclList varDecl  */
#line 318 "psy.y"
                           { (yyval.node) = appendList((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2103 "psy.tab.c"
    break;

  case 25: /* inTypeDeclList: varDecl  */
#line 319 "psy.y"
              { (yyval.node) = appendList(NULL, (yyvsp[0].node)); }
#line 2109 "psy.tab.c"
    break;

  case 26: /* varDecl: "var" var_type IDENTIFIER "=" expression ";"  */
#line 324 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_DECL, (yyloc).first_line, (yyloc).first_column);
        node->as.varDecl.varType = (yyvsp[-4].node);
        node->as.varDecl.name = (yyvsp[-3].sval);
        node->as.varDecl.initializer = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2121 "psy.tab.c"
    break;

  case 27: /* varDecl: "var" var_type IDENTIFIER ";"  */
#line 333 "psy.y"
    {
        Node *node = makeNode(NODE_VAR_DECL, (yyloc).first_line, (yyloc).first_column);
        node->as.varDecl.varType = (yyvsp[-2].node);
        node->as.varDecl.name = (yyvsp[-1].sval);
        node->as.varDecl.initializer = NULL;
        (yyval.node) = node;
    }
#line 2133 "psy.tab.c"
    break;

  case 28: /* statement: exprStmt  */
#line 342 "psy.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2139 "psy.tab.c"
    break;

  case 29: /* statement: forStmt  */
#line 343 "psy.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2145 "psy.tab.c"
    break;

  case 30: /* statement: ifStmt  */
#line 344 "psy.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2151 "psy.tab.c"
    break;

  case 31: /* statement: echoStmt  */
#line 345 "psy.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2157 "psy.tab.c"
    break;

  case 32: /* statement: returnStmt  */
#line 346 "psy.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2163 "psy.tab.c"
    break;

  case 33: /* statement: whileStmt  */
#line 347 "psy.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2169 "psy.tab.c"
    break;

  case 34: /* statement: dowhileStmt  */
#line 348 "psy.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2175 "psy.tab.c"
    break;

  case 35: /* statement: breakStmt  */
#line 349 "psy.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2181 "psy.tab.c"
    break;

  case 36: /* statement: continueStmt  */
#line 350 "psy.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2187 "psy.tab.c"
    break;

  case 37: /* statement: block  */
#line 351 "psy.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2193 "psy.tab.c"
    break;

  case 38: /* exprStmt: expression ";"  */
#line 356 "psy.y"
    {
        Node *node = makeNode(NODE_EXPR_STMT, (yyloc).first_line, (yyloc).first_column);
        node->as.exprStmt.expr = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2203 "psy.tab.c"
    break;

  case 39: /* forStmt: "for" "(" forInit optExpr ";" optExpr ")" statement  */
#line 364 "psy.y"
    {
        Node *node = makeNode(NODE_FOR_STMT, (yyloc).first_line, (yyloc).first_column);
        node->as.forStmt.init = (yyvsp[-5].node);
        node->as.forStmt.condition = (yyvsp[-4].node);
        node->as.forStmt.increment = (yyvsp[-2].node);
        node->as.forStmt.body = (yyvsp[0].node);
        (yyval.node) = node;
    }
#line 2216 "psy.tab.c"
    break;

  case 40: /* forInit: varDecl  */
#line 374 "psy.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2222 "psy.tab.c"
    break;

  case 41: /* forInit: exprStmt  */
#line 375 "psy.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2228 "psy.tab.c"
    break;

  case 42: /* forInit: ";"  */
#line 376 "psy.y"
                 { (yyval.node) = NULL; }
#line 2234 "psy.tab.c"
    break;

  case 43: /* optExpr: expression  */
#line 380 "psy.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2240 "psy.tab.c"
    break;

  case 44: /* optExpr: %empty  */
#line 381 "psy.y"
      { (yyval.node) = NULL; }
#line 2246 "psy.tab.c"
    break;

  case 45: /* ifStmt: "if" "(" expression ")" statement "else" statement  */
#line 386 "psy.y"
    {
        Node *node = makeNode(NODE_IF_STMT, (yyloc).first_line, (yyloc).first_column);
        node->as.ifStmt.condition = (yyvsp[-4].node);
        node->as.ifStmt.thenBranch = (yyvsp[-2].node);
        node->as.ifStmt.elseBranch = (yyvsp[0].node);
        (yyval.node) = node;
    }
#line 2258 "psy.tab.c"
    break;

  case 46: /* ifStmt: "if" "(" expression ")" statement  */
#line 395 "psy.y"
    {
        Node *node = makeNode(NODE_IF_STMT, (yyloc).first_line, (yyloc).first_column);
        node->as.ifStmt.condition = (yyvsp[-2].node);
        node->as.ifStmt.thenBranch = (yyvsp[0].node);
        node->as.ifStmt.elseBranch = NULL;
        (yyval.node) = node;
    }
#line 2270 "psy.tab.c"
    break;

  case 47: /* echoStmt: "echo" expression ";"  */
#line 405 "psy.y"
    {
        Node *node = makeNode(NODE_ECHO_STMT, (yyloc).first_line, (yyloc).first_column);
        node->as.echoStmt.expr = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2280 "psy.tab.c"
    break;

  case 48: /* returnStmt: "ret" expression ";"  */
#line 413 "psy.y"
    {
        Node *node = makeNode(NODE_RETURN, (yyloc).first_line, (yyloc).first_column);
        node->as.returnStmt.expr = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2290 "psy.tab.c"
    break;

  case 49: /* returnStmt: "ret" ";"  */
#line 420 "psy.y"
    {
        Node *node = makeNode(NODE_RETURN, (yyloc).first_line, (yyloc).first_column);
        node->as.returnStmt.expr = NULL;
        (yyval.node) = node;
    }
#line 2300 "psy.tab.c"
    break;

  case 50: /* whileStmt: "while" "(" expression ")" statement  */
#line 429 "psy.y"
    {
        Node *node = makeNode(NODE_WHILE_STMT, (yyloc).first_line, (yyloc).first_column);
        node->as.whileStmt.condition = (yyvsp[-2].node);
        node->as.whileStmt.body = (yyvsp[0].node);
        (yyval.node) = node;
    }
#line 2311 "psy.tab.c"
    break;

  case 51: /* dowhileStmt: "do" statement "while" "(" expression ")" ";"  */
#line 438 "psy.y"
    {
        Node *node = makeNode(NODE_DO_WHILE_STMT, (yyloc).first_line, (yyloc).first_column);
        node->as.doWhileStmt.body = (yyvsp[-5].node);
        node->as.doWhileStmt.condition = (yyvsp[-2].node);
        (yyval.node) = node;
    }
#line 2322 "psy.tab.c"
    break;

  case 52: /* breakStmt: "break" ";"  */
#line 447 "psy.y"
    {
        Node *node = makeNode(NODE_BREAK, (yyloc).first_line, (yyloc).first_column);
        (yyval.node) = node;
    }
#line 2331 "psy.tab.c"
    break;

  case 53: /* continueStmt: "continue" ";"  */
#line 455 "psy.y"
    {
        Node *node = makeNode(NODE_CONTINUE, (yyloc).first_line, (yyloc).first_column);
        (yyval.node) = node;
    }
#line 2340 "psy.tab.c"
    break;

  case 54: /* block: "{" decList "}"  */
#line 463 "psy.y"
    {
        Node *node = makeNode(NODE_BLOCK, (yyloc).first_line, (yyloc).first_column);
        node->as.block.declarations = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2350 "psy.tab.c"
    break;

  case 55: /* block: "{" "}"  */
#line 470 "psy.y"
    {
        Node *node = makeNode(NODE_BLOCK, (yyloc).first_line, (yyloc).first_column);
        node->as.block.declarations = NULL;
        (yyval.node) = node;
    }
#line 2360 "psy.tab.c"
    break;

  case 56: /* expression: assignment  */
#line 478 "psy.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2366 "psy.tab.c"
    break;

  case 57: /* assignment: call_access "=" assignment  */
#line 482 "psy.y"
                              { (yyval.node) = makeAssignNode(OP_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2372 "psy.tab.c"
    break;

  case 58: /* assignment: call_access "+=" assignment  */
#line 483 "psy.y"
                                     { (yyval.node) = makeAssignNode(OP_PLUS_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2378 "psy.tab.c"
    break;

  case 59: /* assignment: call_access "-=" assignment  */
#line 484 "psy.y"
                                      { (yyval.node) = makeAssignNode(OP_MINUS_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2384 "psy.tab.c"
    break;

  case 60: /* assignment: call_access "*=" assignment  */
#line 485 "psy.y"
                                     { (yyval.node) = makeAssignNode(OP_STAR_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2390 "psy.tab.c"
    break;

  case 61: /* assignment: call_access "/=" assignment  */
#line 486 "psy.y"
                                      { (yyval.node) = makeAssignNode(OP_SLASH_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2396 "psy.tab.c"
    break;

  case 62: /* assignment: call_access "%=" assignment  */
#line 487 "psy.y"
                                     { (yyval.node) = makeAssignNode(OP_PERC_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2402 "psy.tab.c"
    break;

  case 63: /* assignment: ternary  */
#line 488 "psy.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2408 "psy.tab.c"
    break;

  case 64: /* ternary: logic_or "?" assignment ":" assignment  */
#line 493 "psy.y"
    {
        Node *node = makeNode(NODE_TERNARY_OP, (yyloc).first_line, (yyloc).first_column);
        node->as.ternary.condition = (yyvsp[-4].node);
        node->as.ternary.ifBranch = (yyvsp[-2].node);
        node->as.ternary.elseBranch = (yyvsp[0].node);
        (yyval.node) = node;
    }
#line 2420 "psy.tab.c"
    break;

  case 65: /* ternary: logic_or  */
#line 501 "psy.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2426 "psy.tab.c"
    break;

  case 66: /* logic_or: logic_or "||" logic_and  */
#line 504 "psy.y"
                                   { (yyval.node) = makeBinaryNode(OP_DOUBLE_PIPE, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2432 "psy.tab.c"
    break;

  case 67: /* logic_or: logic_and  */
#line 505 "psy.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2438 "psy.tab.c"
    break;

  case 68: /* logic_and: logic_and "&&" bitwise_or  */
#line 509 "psy.y"
                                    { (yyval.node) = makeBinaryNode(OP_DOUBLE_AND, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2444 "psy.tab.c"
    break;

  case 69: /* logic_and: bitwise_or  */
#line 510 "psy.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2450 "psy.tab.c"
    break;

  case 70: /* bitwise_or: bitwise_or "|" bitwise_xor  */
#line 514 "psy.y"
                                { (yyval.node) = makeBinaryNode(OP_PIPE, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2456 "psy.tab.c"
    break;

  case 71: /* bitwise_or: bitwise_xor  */
#line 515 "psy.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2462 "psy.tab.c"
    break;

  case 72: /* bitwise_xor: bitwise_xor "^" bitwise_and  */
#line 519 "psy.y"
                                { (yyval.node) = makeBinaryNode(OP_XOR, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2468 "psy.tab.c"
    break;

  case 73: /* bitwise_xor: bitwise_and  */
#line 520 "psy.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2474 "psy.tab.c"
    break;

  case 74: /* bitwise_and: bitwise_and "&" equality  */
#line 524 "psy.y"
                             { (yyval.node) = makeBinaryNode(OP_AND, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2480 "psy.tab.c"
    break;

  case 75: /* bitwise_and: equality  */
#line 525 "psy.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2486 "psy.tab.c"
    break;

  case 76: /* equality: equality "!=" comparison  */
#line 529 "psy.y"
                                { (yyval.node) = makeBinaryNode(OP_BANG_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2492 "psy.tab.c"
    break;

  case 77: /* equality: equality "==" comparison  */
#line 530 "psy.y"
                                { (yyval.node) = makeBinaryNode(OP_EQ_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2498 "psy.tab.c"
    break;

  case 78: /* equality: comparison  */
#line 531 "psy.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2504 "psy.tab.c"
    break;

  case 79: /* comparison: comparison ">" shift  */
#line 535 "psy.y"
                             { (yyval.node) = makeBinaryNode(OP_GREATER, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2510 "psy.tab.c"
    break;

  case 80: /* comparison: comparison "<" shift  */
#line 536 "psy.y"
                            { (yyval.node) = makeBinaryNode(OP_LESS, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2516 "psy.tab.c"
    break;

  case 81: /* comparison: comparison ">=" shift  */
#line 537 "psy.y"
                                  { (yyval.node) = makeBinaryNode(OP_GREATER_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2522 "psy.tab.c"
    break;

  case 82: /* comparison: comparison "<=" shift  */
#line 538 "psy.y"
                               { (yyval.node) = makeBinaryNode(OP_LESS_EQ, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2528 "psy.tab.c"
    break;

  case 83: /* comparison: shift  */
#line 539 "psy.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2534 "psy.tab.c"
    break;

  case 84: /* shift: shift "<<" term  */
#line 543 "psy.y"
                          { (yyval.node) = makeBinaryNode(OP_SHIFT_LEFT, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2540 "psy.tab.c"
    break;

  case 85: /* shift: shift ">>" term  */
#line 544 "psy.y"
                             { (yyval.node) = makeBinaryNode(OP_SHIFT_RIGHT, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2546 "psy.tab.c"
    break;

  case 86: /* shift: term  */
#line 545 "psy.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2552 "psy.tab.c"
    break;

  case 87: /* term: term "+" factor  */
#line 549 "psy.y"
                     { (yyval.node) = makeBinaryNode(OP_PLUS, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2558 "psy.tab.c"
    break;

  case 88: /* term: term "-" factor  */
#line 550 "psy.y"
                        { (yyval.node) = makeBinaryNode(OP_MINUS, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2564 "psy.tab.c"
    break;

  case 89: /* term: factor  */
#line 551 "psy.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2570 "psy.tab.c"
    break;

  case 90: /* factor: factor "/" unary  */
#line 555 "psy.y"
                       { (yyval.node) = makeBinaryNode(OP_SLASH, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2576 "psy.tab.c"
    break;

  case 91: /* factor: factor "*" unary  */
#line 556 "psy.y"
                        { (yyval.node) = makeBinaryNode(OP_STAR, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2582 "psy.tab.c"
    break;

  case 92: /* factor: factor "%" unary  */
#line 557 "psy.y"
                        { (yyval.node) = makeBinaryNode(OP_PERC, (yyvsp[-2].node), (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2588 "psy.tab.c"
    break;

  case 93: /* factor: unary  */
#line 558 "psy.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2594 "psy.tab.c"
    break;

  case 94: /* unary: "!" unary  */
#line 562 "psy.y"
               { (yyval.node) = makeUnaryNode(OP_BANG, (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2600 "psy.tab.c"
    break;

  case 95: /* unary: "~" unary  */
#line 563 "psy.y"
                  { (yyval.node) = makeUnaryNode(OP_TILDE, (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2606 "psy.tab.c"
    break;

  case 96: /* unary: "-" unary  */
#line 564 "psy.y"
                  { (yyval.node) = makeUnaryNode(OP_MINUS, (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2612 "psy.tab.c"
    break;

  case 97: /* unary: "++" unary  */
#line 565 "psy.y"
                      { (yyval.node) = makeUnaryNode(OP_PLUS_PLUS, (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2618 "psy.tab.c"
    break;

  case 98: /* unary: "--" unary  */
#line 566 "psy.y"
                        { (yyval.node) = makeUnaryNode(OP_MINUS_MINUS, (yyvsp[0].node), (yyloc).first_line, (yyloc).first_column); }
#line 2624 "psy.tab.c"
    break;

  case 99: /* unary: call_access  */
#line 567 "psy.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 2630 "psy.tab.c"
    break;

  case 100: /* call_access: call_access "(" arguments ")"  */
#line 572 "psy.y"
    {
        Node *node = makeNode(NODE_CALL, (yyloc).first_line, (yyloc).first_column);
        node->as.call.callee = (yyvsp[-3].node);
        node->as.call.arguments = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2641 "psy.tab.c"
    break;

  case 101: /* call_access: call_access "(" ")"  */
#line 580 "psy.y"
    {
        Node *node = makeNode(NODE_CALL, (yyloc).first_line, (yyloc).first_column);
        node->as.call.callee = (yyvsp[-2].node);
        node->as.call.arguments = NULL;
        (yyval.node) = node;
    }
#line 2652 "psy.tab.c"
    break;

  case 102: /* call_access: call_access "." IDENTIFIER  */
#line 588 "psy.y"
    {
        Node *node = makeNode(NODE_MEMBER_ACCESS, (yyloc).first_line, (yyloc).first_column);
        node->as.memberAccess.object = (yyvsp[-2].node);
        node->as.memberAccess.member = (yyvsp[0].sval);
        (yyval.node) = node;
    }
#line 2663 "psy.tab.c"
    break;

  case 103: /* call_access: call_access "[" expression "]"  */
#line 596 "psy.y"
    {
        Node *node = makeNode(NODE_INDEX_ACCESS, (yyloc).first_line, (yyloc).first_column);
        node->as.indexAccess.object = (yyvsp[-3].node);
        node->as.indexAccess.index = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2674 "psy.tab.c"
    break;

  case 104: /* call_access: call_access "++"  */
#line 604 "psy.y"
    {
        Node *node = makeNode(NODE_POSTFIX_OP, (yyloc).first_line, (yyloc).first_column);
        node->as.postfix.op = OP_PLUS_PLUS;
        node->as.postfix.expr = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2685 "psy.tab.c"
    break;

  case 105: /* call_access: call_access "--"  */
#line 612 "psy.y"
    {
        Node *node = makeNode(NODE_POSTFIX_OP, (yyloc).first_line, (yyloc).first_column);
        node->as.postfix.op = OP_MINUS_MINUS;
        node->as.postfix.expr = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2696 "psy.tab.c"
    break;

  case 106: /* call_access: primary  */
#line 619 "psy.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2702 "psy.tab.c"
    break;

  case 107: /* primary: BOOL_CONSTANT  */
#line 623 "psy.y"
    {
        Node *node = makePrimaryNode(PRIMARY_BOOL, (yyloc).first_line, (yyloc).first_column);
        node->as.primary.val.boolval = (yyvsp[0].bval);
        (yyval.node) = node;
    }
#line 2712 "psy.tab.c"
    break;

  case 108: /* primary: "null"  */
#line 629 "psy.y"
           { (yyval.node) = makePrimaryNode(PRIMARY_NULL, (yyloc).first_line, (yyloc).first_column); }
#line 2718 "psy.tab.c"
    break;

  case 109: /* primary: INT_CONSTANT  */
#line 632 "psy.y"
    {
        Node *node = makePrimaryNode(PRIMARY_INT, (yyloc).first_line, (yyloc).first_column);
        node->as.primary.val.intval = (yyvsp[0].ival);
        (yyval.node) = node;
    }
#line 2728 "psy.tab.c"
    break;

  case 110: /* primary: FLOAT_CONSTANT  */
#line 638 "psy.y"
    {
        Node *node = makePrimaryNode(PRIMARY_FLOAT, (yyloc).first_line, (yyloc).first_column);
        node->as.primary.val.floatval = (yyvsp[0].fval);
        (yyval.node) = node;
    }
#line 2738 "psy.tab.c"
    break;

  case 111: /* primary: STRING_CONSTANT  */
#line 645 "psy.y"
    {
        Node *node = makePrimaryNode(PRIMARY_STRING, (yyloc).first_line, (yyloc).first_column);
        node->as.primary.val.stringval = (yyvsp[0].sval);
        (yyval.node) = node;
    }
#line 2748 "psy.tab.c"
    break;

  case 112: /* primary: IDENTIFIER  */
#line 651 "psy.y"
    {
        Node *node = makePrimaryNode(PRIMARY_IDENTIFIER, (yyloc).first_line, (yyloc).first_column);
        node->as.primary.val.stringval = (yyvsp[0].sval);
        (yyval.node) = node;
    }
#line 2758 "psy.tab.c"
    break;

  case 113: /* primary: anonFunc  */
#line 657 "psy.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2764 "psy.tab.c"
    break;

  case 114: /* primary: "(" expression ")"  */
#line 659 "psy.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2770 "psy.tab.c"
    break;

  case 115: /* primary: "[" elements "]"  */
#line 661 "psy.y"
    {
        Node *node = makeNode(NODE_LIST_LITERAL, (yyloc).first_line, (yyloc).first_column);
        node->as.list.elements = (yyvsp[-1].node);
        (yyval.node) = node;
    }
#line 2780 "psy.tab.c"
    break;

  case 116: /* primary: "[" "]"  */
#line 668 "psy.y"
    {
        Node *node = makeNode(NODE_LIST_LITERAL, (yyloc).first_line, (yyloc).first_column);
        node->as.list.elements = NULL;
        (yyval.node) = node;
    }
#line 2790 "psy.tab.c"
    break;

  case 117: /* anonFunc: "lambda" function  */
#line 677 "psy.y"
    {
        Node *node = makeNode(NODE_LAMBDA, (yyloc).first_line, (yyloc).first_column);
        node->as.lambda.function = (yyvsp[0].node);
        (yyval.node) = node;
    }
#line 2800 "psy.tab.c"
    break;

  case 118: /* arguments: arguments "," expression  */
#line 684 "psy.y"
                               { (yyval.node) = appendList((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2806 "psy.tab.c"
    break;

  case 119: /* arguments: expression  */
#line 685 "psy.y"
                 { (yyval.node) = appendList(NULL, (yyvsp[0].node)); }
#line 2812 "psy.tab.c"
    break;

  case 120: /* elements: elements "," expression  */
#line 689 "psy.y"
                              { (yyval.node) = appendList((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2818 "psy.tab.c"
    break;

  case 121: /* elements: expression  */
#line 690 "psy.y"
                 { (yyval.node) = appendList(NULL, (yyvsp[0].node)); }
#line 2824 "psy.tab.c"
    break;


#line 2828 "psy.tab.c"

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
  *++yylsp = yyloc;

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 693 "psy.y"


int main(int argc, char **argv) {
    if (argc > 1) {
        current_filename = argv[1];
        yyin = fopen(current_filename, "r");

        if (!yyin) {
            fprintf(stderr, "Error: Could not open file '%s'\n", current_filename);
            return 1;
        }
    } else {
        current_filename = "<stdin>";
    }

    int result = yyparse();
    if (result == 0)
    {
        printf("Parsed OK\n");
        if (!should_dump_ir_only()) {
            printAst(rootAst, 0);
        }

        if (should_dump_ir()) {
            bool debug = should_debug();
            if (!analyze_ast(rootAst, debug)) {
                fprintf(stderr, "\nSemantic analysis failed. Compilation stopped.\n");
                return 1;
            }

            generateIr(rootAst);
            printf("IR\n");
            irDump(stdout);

            if (debug) {
                printf("\n--- Symbol Table (Debug) ---");
                print_symbol_table();

                printf("\n--- Type Registry (Debug) ---");
                print_type_registry();
            }
        }
    }

    if (yyin && yyin != stdin) {
        fclose(yyin);
    }

    return result;
}

void print_fancy_error(int line, int col, const char *message) {
    fflush(stdout);
    int use_color = isatty(fileno(stderr));

    const char *color_red = use_color ? "\033[1;31m" : "";
    const char *color_bold = use_color ? "\033[1m" : "";
    const char *color_reset = use_color ? "\033[0m" : "";

    fprintf(stderr, "%serror:%s %s%s%s\n", color_red, color_reset, color_bold, message, color_reset);
    fprintf(stderr, "  --> %s:%d:%d\n", current_filename, line, col);

    if (strcmp(current_filename, "<stdin>") == 0) {
        return;
    }

    FILE *file = fopen(current_filename, "r");
    if (!file) return;

    char buffer[1024];
    int current_line = 1;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (current_line == line) {
            buffer[strcspn(buffer, "\n")] = 0;
            fprintf(stderr, "   |\n");
            fprintf(stderr, "%3d| %s\n", line, buffer);
            fprintf(stderr, "   | ");
            for (int i = 1; i < col; i++) {
                if (buffer[i-1] == '\t') fprintf(stderr, "\t");
                else fprintf(stderr, " ");
            }
            fprintf(stderr, "%s^--- Here%s\n\n", color_red, color_reset);
            break;
        }
        current_line++;
    }
    fclose(file);
}

void yyerror(const char *message) {
    print_fancy_error(yylineno, yycolumn > 0 ? yycolumn : 1, message);
}
