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

#ifndef YY_YY_PSY_TAB_H_INCLUDED
# define YY_YY_PSY_TAB_H_INCLUDED
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
    VAR = 262,                     /* VAR  */
    INT = 263,                     /* INT  */
    FLOAT = 264,                   /* FLOAT  */
    STRING = 265,                  /* STRING  */
    DYN = 266,                     /* DYN  */
    PLUS = 267,                    /* PLUS  */
    MINUS = 268,                   /* MINUS  */
    STAR = 269,                    /* STAR  */
    SLASH = 270,                   /* SLASH  */
    PERC = 271,                    /* PERC  */
    LESS = 272,                    /* LESS  */
    GREATER = 273,                 /* GREATER  */
    LESS_EQ = 274,                 /* LESS_EQ  */
    GREATER_EQ = 275,              /* GREATER_EQ  */
    BANG_EQ = 276,                 /* BANG_EQ  */
    EQ_EQ = 277,                   /* EQ_EQ  */
    AND = 278,                     /* AND  */
    DOUBLE_AND = 279,              /* DOUBLE_AND  */
    PIPE = 280,                    /* PIPE  */
    DOUBLE_PIPE = 281,             /* DOUBLE_PIPE  */
    TILDE = 282,                   /* TILDE  */
    LESS_LESS = 283,               /* LESS_LESS  */
    GREATER_GREATER = 284,         /* GREATER_GREATER  */
    EQ = 285,                      /* EQ  */
    PLUS_EQ = 286,                 /* PLUS_EQ  */
    MINUS_EQ = 287,                /* MINUS_EQ  */
    PLUS_PLUS = 288,               /* PLUS_PLUS  */
    MINUS_MINUS = 289,             /* MINUS_MINUS  */
    QUESTION_MARK = 290,           /* QUESTION_MARK  */
    BANG = 291,                    /* BANG  */
    SINGLE_QUOTE = 292,            /* SINGLE_QUOTE  */
    DOUBLE_QUOTE = 293,            /* DOUBLE_QUOTE  */
    ECH = 294,                     /* ECH  */
    IF = 295,                      /* IF  */
    ELSE = 296,                    /* ELSE  */
    FOR = 297,                     /* FOR  */
    DO = 298,                      /* DO  */
    WHILE = 299,                   /* WHILE  */
    BREAK = 300,                   /* BREAK  */
    CONTINUE = 301,                /* CONTINUE  */
    TYPE = 302,                    /* TYPE  */
    FUN = 303,                     /* FUN  */
    RET = 304,                     /* RET  */
    ARROW = 305,                   /* ARROW  */
    LEFT_PAREN = 306,              /* LEFT_PAREN  */
    RIGHT_PAREN = 307,             /* RIGHT_PAREN  */
    LEFT_BRACE = 308,              /* LEFT_BRACE  */
    RIGHT_BRACE = 309,             /* RIGHT_BRACE  */
    LEFT_SQUARE = 310,             /* LEFT_SQUARE  */
    RIGHT_SQUARE = 311,            /* RIGHT_SQUARE  */
    DOT = 312,                     /* DOT  */
    COLON = 313,                   /* COLON  */
    SEMI_COLON = 314,              /* SEMI_COLON  */
    COMMA = 315                    /* COMMA  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 9 "psy.y"

    int ival;
    float fval;
    char* sval;

#line 130 "psy.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PSY_TAB_H_INCLUDED  */
