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
    BOOL_CONSTANT = 262,           /* BOOL_CONSTANT  */
    T_NULL = 263,                  /* "null"  */
    VAR = 264,                     /* "var"  */
    INT = 265,                     /* "int"  */
    FLOAT = 266,                   /* "float"  */
    STRING = 267,                  /* "string"  */
    BOOL = 268,                    /* "bool"  */
    LIST = 269,                    /* "list"  */
    DYN = 270,                     /* "dyn"  */
    PLUS = 271,                    /* "+"  */
    MINUS = 272,                   /* "-"  */
    STAR = 273,                    /* "*"  */
    SLASH = 274,                   /* "/"  */
    PERC = 275,                    /* "%"  */
    LESS = 276,                    /* "<"  */
    GREATER = 277,                 /* ">"  */
    LESS_EQ = 278,                 /* "<="  */
    GREATER_EQ = 279,              /* ">="  */
    BANG_EQ = 280,                 /* "!="  */
    EQ_EQ = 281,                   /* "=="  */
    AND = 282,                     /* "&"  */
    DOUBLE_AND = 283,              /* "&&"  */
    PIPE = 284,                    /* "|"  */
    DOUBLE_PIPE = 285,             /* "||"  */
    TILDE = 286,                   /* "~"  */
    XOR = 287,                     /* "^"  */
    SHIFT_LEFT = 288,              /* "<<"  */
    SHIFT_RIGHT = 289,             /* ">>"  */
    EQ = 290,                      /* "="  */
    PLUS_EQ = 291,                 /* "+="  */
    MINUS_EQ = 292,                /* "-="  */
    STAR_EQ = 293,                 /* "*="  */
    SLASH_EQ = 294,                /* "/="  */
    PERC_EQ = 295,                 /* "%="  */
    PLUS_PLUS = 296,               /* "++"  */
    MINUS_MINUS = 297,             /* "--"  */
    QUESTION_MARK = 298,           /* "?"  */
    BANG = 299,                    /* "!"  */
    ECH = 300,                     /* "echo"  */
    IF = 301,                      /* "if"  */
    ELSE = 302,                    /* "else"  */
    FOR = 303,                     /* "for"  */
    DO = 304,                      /* "do"  */
    WHILE = 305,                   /* "while"  */
    BREAK = 306,                   /* "break"  */
    CONTINUE = 307,                /* "continue"  */
    TYPE = 308,                    /* "type"  */
    FUN = 309,                     /* "fun"  */
    RET = 310,                     /* "ret"  */
    ARROW = 311,                   /* "->"  */
    LAMBDA = 312,                  /* "lambda"  */
    LEFT_PAREN = 313,              /* "("  */
    RIGHT_PAREN = 314,             /* ")"  */
    LEFT_BRACE = 315,              /* "{"  */
    RIGHT_BRACE = 316,             /* "}"  */
    LEFT_SQUARE = 317,             /* "["  */
    RIGHT_SQUARE = 318,            /* "]"  */
    DOT = 319,                     /* "."  */
    COLON = 320,                   /* ":"  */
    SEMI_COLON = 321,              /* ";"  */
    COMMA = 322,                   /* ","  */
    LOWER_THAN_ELSE = 323          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 113 "psy.y"

    int ival;
    float fval;
    char* sval;
    bool bval;
    Node* node;

#line 140 "psy.tab.h"

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


#endif /* !YY_YY_PSY_TAB_H_INCLUDED  */
