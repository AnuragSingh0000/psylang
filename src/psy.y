%{
#include <stdio.h>
%}

%token VAR INT FLOAT STRING DYN // tokens for mentioning the types
%token INT_CONSTANT FLOAT_CONSTANT STRING_CONSTANT NULL// token to store the constants like ints, floats, strings eg 1, 1.0, "hello"

%token PLUS MINUS STAR SLASH PERC // tokens for arithmetic ops
%token LESS GREATER LESS_EQ GREATER_EQ BANG_EQ EQ_EQ // comparision operators
%token AND PIPE EXP TILDE // bitwise operators
%token LESS_LESS GREATER_GREATER // bitshift operators 
%token EQ PLUS_EQ MINUS_EQ PLUS_PLUS MINUS_MINUS // assignment operators
%token QUESTION_MARK // for ternary operator
%token BANG // for unary

%token SINGLE_QUOTE DOUBLE_QUOTE // double quotes for string

%token ECHO // for print statemnts
%token IF ELSE // conditionals
%token FOR DO WHILE  // loops

%token TYPE // user defined types
%token FUN RET ARROW // for functions

%token LEFT_PAREN RIGHT_PAREN LEFT_BRACE RIGHT_BRACE LEFT_SQUARE RIGHT_SQUARE 
%token DOT COLON SEMI_COLON  

%%
dummy: INT_CONSTANT;
%%

int main(void)
{
    yyparse();
    return 0;
}
