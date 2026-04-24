/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PSY_PRINTTOKENS_TAB_H_INCLUDED
# define YY_YY_PSY_PRINTTOKENS_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT_CONSTANT = 258,            /* INT_CONSTANT  */
    FLOAT_CONSTANT = 259,          /* FLOAT_CONSTANT  */
    IDENTIFIER = 260,              /* IDENTIFIER  */
    STRING_CONSTANT = 261,         /* STRING_CONSTANT  */
    BOOL_CONSTANT = 262,           /* BOOL_CONSTANT  */
    T_NULL = 263,                  /* T_NULL  */
    VAR = 264,                     /* VAR  */
    INT = 265,                     /* INT  */
    FLOAT = 266,                   /* FLOAT  */
    STRING = 267,                  /* STRING  */
    BOOL = 268,                    /* BOOL  */
    LIST = 269,                    /* LIST  */
    DYN = 270,                     /* DYN  */
    PLUS = 271,                    /* PLUS  */
    MINUS = 272,                   /* MINUS  */
    STAR = 273,                    /* STAR  */
    SLASH = 274,                   /* SLASH  */
    PERC = 275,                    /* PERC  */
    LESS = 276,                    /* LESS  */
    GREATER = 277,                 /* GREATER  */
    LESS_EQ = 278,                 /* LESS_EQ  */
    GREATER_EQ = 279,              /* GREATER_EQ  */
    BANG_EQ = 280,                 /* BANG_EQ  */
    EQ_EQ = 281,                   /* EQ_EQ  */
    AND = 282,                     /* AND  */
    DOUBLE_AND = 283,              /* DOUBLE_AND  */
    PIPE = 284,                    /* PIPE  */
    DOUBLE_PIPE = 285,             /* DOUBLE_PIPE  */
    TILDE = 286,                   /* TILDE  */
    XOR = 287,                     /* XOR  */
    SHIFT_LEFT = 288,              /* SHIFT_LEFT  */
    SHIFT_RIGHT = 289,             /* SHIFT_RIGHT  */
    EQ = 290,                      /* EQ  */
    PLUS_EQ = 291,                 /* PLUS_EQ  */
    MINUS_EQ = 292,                /* MINUS_EQ  */
    STAR_EQ = 293,                 /* STAR_EQ  */
    SLASH_EQ = 294,                /* SLASH_EQ  */
    PERC_EQ = 295,                 /* PERC_EQ  */
    PLUS_PLUS = 296,               /* PLUS_PLUS  */
    MINUS_MINUS = 297,             /* MINUS_MINUS  */
    QUESTION_MARK = 298,           /* QUESTION_MARK  */
    BANG = 299,                    /* BANG  */
    ECH = 300,                     /* ECH  */
    IF = 301,                      /* IF  */
    ELSE = 302,                    /* ELSE  */
    FOR = 303,                     /* FOR  */
    DO = 304,                      /* DO  */
    WHILE = 305,                   /* WHILE  */
    BREAK = 306,                   /* BREAK  */
    CONTINUE = 307,                /* CONTINUE  */
    TYPE = 308,                    /* TYPE  */
    FUN = 309,                     /* FUN  */
    RET = 310,                     /* RET  */
    ARROW = 311,                   /* ARROW  */
    LAMBDA = 312,                  /* LAMBDA  */
    LEFT_PAREN = 313,              /* LEFT_PAREN  */
    RIGHT_PAREN = 314,             /* RIGHT_PAREN  */
    LEFT_BRACE = 315,              /* LEFT_BRACE  */
    RIGHT_BRACE = 316,             /* RIGHT_BRACE  */
    LEFT_SQUARE = 317,             /* LEFT_SQUARE  */
    RIGHT_SQUARE = 318,            /* RIGHT_SQUARE  */
    DOT = 319,                     /* DOT  */
    COLON = 320,                   /* COLON  */
    SEMI_COLON = 321,              /* SEMI_COLON  */
    COMMA = 322                    /* COMMA  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 9 "psy_printtokens.y"

    int ival;
    float fval;
    char* sval;
    bool bval;

#line 138 "psy_printtokens.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_PSY_PRINTTOKENS_TAB_H_INCLUDED  */
