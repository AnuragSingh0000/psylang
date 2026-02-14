%{
#include <stdio.h>
#include <stdbool.h>
int yylex(void);
void yyerror(char *);
%}

// Define the union for the possible token values

%union {
    int ival;
    float fval;
    char* sval;
    bool bval;
}

%token <ival> INT_CONSTANT // tokens for int literals
%token <fval> FLOAT_CONSTANT // tokens for float literals
%token <sval> IDENTIFIER STRING_CONSTANT // tokens for identifiers and string literals
%token <bval> BOOL_CONSTANT
%token T_NULL

%token VAR INT FLOAT STRING BOOL LIST DYN // tokens for mentioning the types

%token PLUS MINUS STAR SLASH PERC // tokens for arithmetic ops
%token LESS GREATER LESS_EQ GREATER_EQ BANG_EQ EQ_EQ // comparision operators
%token AND DOUBLE_AND PIPE DOUBLE_PIPE TILDE  // bitwise operators
%token SHIFT_LEFT SHIFT_RIGHT // bitshift operators 
%token EQ PLUS_EQ MINUS_EQ PLUS_PLUS MINUS_MINUS // assignment operators
%token QUESTION_MARK // for ternary operator
%token BANG // for unary

%token ECH // for print statemnts
%token IF ELSE // conditionals
%token FOR DO WHILE BREAK CONTINUE// loops

%token TYPE // user defined types
%token FUN RET ARROW // for functions

%token LEFT_PAREN RIGHT_PAREN LEFT_BRACE RIGHT_BRACE LEFT_SQUARE RIGHT_SQUARE 
%token DOT COLON SEMI_COLON COMMA

%%

// The grammar rules for the language. For now, we will just print out the tokens as they are parsed.

input:
      /* empty */
    | input line
    ;

line:
      token
    | '\n'
    ;

token:
      INT_CONSTANT      { printf("INT_CONSTANT: %d\n", $1); }
    | FLOAT_CONSTANT    { printf("FLOAT_CONSTANT: %f\n", $1); }
    | STRING_CONSTANT   { printf("STRING_CONSTANT: %s\n", $1); }
    | IDENTIFIER        { printf("IDENTIFIER: %s\n", $1); }
    | BOOL_CONSTANT     { printf("BOOL_CONSTANT: %d\n", $1); }
    | T_NULL            { printf("NULL\n"); }

    | PLUS              { printf("PLUS\n"); }
    | MINUS             { printf("MINUS\n"); }
    | STAR              { printf("STAR\n"); }
    | SLASH             { printf("SLASH\n"); }
    | PERC              { printf("PERC\n"); }

    | VAR               { printf("VAR\n"); }
    | INT               { printf("INT\n"); }
    | FLOAT             { printf("FLOAT\n"); }
    | STRING            { printf("STRING\n"); }
    | BOOL              { printf("BOOLEAN\n"); }
    | DYN               { printf("DYN\n"); }
    | LIST              { printf("LIST\n"); } 

    | IF                { printf("IF\n"); }
    | ELSE              { printf("ELSE\n"); }
    | FOR               { printf("FOR\n"); }
    | WHILE             { printf("WHILE\n"); }
    | DO                { printf("DO\n"); }
    | BREAK             { printf("BREAK\n"); }
    | CONTINUE          { printf("CONTINUE\n"); }

    | ECH               { printf("ECHO\n"); }

    | EQ                { printf("EQ\n"); }
    | PLUS_EQ           { printf("PLUS_EQ\n"); }
    | MINUS_EQ          { printf("MINUS_EQ\n"); }
    | PLUS_PLUS         { printf("PLUS_PLUS\n"); }
    | MINUS_MINUS       { printf("MINUS_MINUS\n"); }

    | LESS              { printf("LESS\n"); }
    | GREATER           { printf("GREATER\n"); }
    | LESS_EQ           { printf("LESS_EQ\n"); }
    | GREATER_EQ        { printf("GREATER_EQ\n"); }
    | EQ_EQ             { printf("EQ_EQ\n"); }
    | BANG_EQ           { printf("BANG_EQ\n"); }

    | AND               { printf("AND\n"); }
    | DOUBLE_AND        { printf("DOUBLE_AND\n"); }
    | PIPE              { printf("PIPE\n"); }
    | DOUBLE_PIPE       { printf("DOUBLE_PIPE\n"); }
    | TILDE             { printf("TILDE\n"); }
    | BANG              { printf("BANG\n"); }
    | SHIFT_LEFT        { printf("SHIFT_LEFT\n"); }
    | SHIFT_RIGHT       { printf("SHIFT_RIGHT\n"); }

    | TYPE              { printf("TYPE\n");}
    | FUN               { printf("FUN\n");}
    | RET               { printf("RET\n");}
    | ARROW             { printf("ARROW\n");}

    | LEFT_PAREN        { printf("LEFT_PAREN\n"); }
    | RIGHT_PAREN       { printf("RIGHT_PAREN\n"); }
    | LEFT_BRACE        { printf("LEFT_BRACE\n"); }
    | RIGHT_BRACE       { printf("RIGHT_BRACE\n"); }
    | LEFT_SQUARE       { printf("LEFT_SQUARE\n"); }
    | RIGHT_SQUARE      { printf("RIGHT_SQUARE\n"); }

    | DOT               { printf("DOT\n"); }
    | COLON             { printf("COLON\n"); }
    | SEMI_COLON        { printf("SEMI_COLON\n"); }
    | QUESTION_MARK     { printf("QUESTION_MARK\n"); }
    | COMMA             { printf("COMMA\n"); }
    ;

%%

int main(void)
{
    yyparse();
    return 0;
}

void yyerror(char *s)
{
    fprintf(stderr, "Parse error: %s\n", s);
}
