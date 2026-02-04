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
    VAR = 258,                     /* VAR  */
    INT = 259,                     /* INT  */
    FLOAT = 260,                   /* FLOAT  */
    STRING = 261,                  /* STRING  */
    DYN = 262,                     /* DYN  */
    INT_CONSTANT = 263,            /* INT_CONSTANT  */
    FLOAT_CONSTANT = 264,          /* FLOAT_CONSTANT  */
    STRING_CONSTANT = 265,         /* STRING_CONSTANT  */
    PLUS = 266,                    /* PLUS  */
    MINUS = 267,                   /* MINUS  */
    STAR = 268,                    /* STAR  */
    SLASH = 269,                   /* SLASH  */
    PERC = 270,                    /* PERC  */
    LESS = 271,                    /* LESS  */
    GREATER = 272,                 /* GREATER  */
    LESS_EQ = 273,                 /* LESS_EQ  */
    GREATER_EQ = 274,              /* GREATER_EQ  */
    BANG_EQ = 275,                 /* BANG_EQ  */
    EQ_EQ = 276,                   /* EQ_EQ  */
    AND = 277,                     /* AND  */
    PIPE = 278,                    /* PIPE  */
    EXP = 279,                     /* EXP  */
    TILDE = 280,                   /* TILDE  */
    LESS_LESS = 281,               /* LESS_LESS  */
    GREATER_GREATER = 282,         /* GREATER_GREATER  */
    EQ = 283,                      /* EQ  */
    PLUS_EQ = 284,                 /* PLUS_EQ  */
    MINUS_EQ = 285,                /* MINUS_EQ  */
    PLUS_PLUS = 286,               /* PLUS_PLUS  */
    MINUS_MINUS = 287,             /* MINUS_MINUS  */
    QUESTION_MARK = 288,           /* QUESTION_MARK  */
    SINGLE_QUOTE = 289,            /* SINGLE_QUOTE  */
    DOUBLE_QUOTE = 290,            /* DOUBLE_QUOTE  */
    ECHO = 291,                    /* ECHO  */
    IF = 292,                      /* IF  */
    ELSE = 293,                    /* ELSE  */
    FOR = 294,                     /* FOR  */
    DO = 295,                      /* DO  */
    WHILE = 296,                   /* WHILE  */
    TYPE = 297,                    /* TYPE  */
    FUN = 298,                     /* FUN  */
    RET = 299,                     /* RET  */
    ARROW = 300,                   /* ARROW  */
    LEFT_PAREN = 301,              /* LEFT_PAREN  */
    RIGHT_PAREN = 302,             /* RIGHT_PAREN  */
    LEFT_BRACE = 303,              /* LEFT_BRACE  */
    RIGHT_BRACE = 304,             /* RIGHT_BRACE  */
    LEFT_SQUARE = 305,             /* LEFT_SQUARE  */
    RIGHT_SQUARE = 306,            /* RIGHT_SQUARE  */
    DOT = 307,                     /* DOT  */
    COLON = 308,                   /* COLON  */
    SEMI_COLON = 309               /* SEMI_COLON  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PSY_TAB_H_INCLUDED  */
