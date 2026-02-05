%{
#include <stdlib.h>
#include "psy.tab.h"
%}

%%

"+"            return PLUS;
"-"            return MINUS;
"*"            return STAR;
"/"            return SLASH;
"%"            return PERC;

"var"          return VAR;
"int"          return INT;
"float"        return FLOAT;
"string"       return STRING;
"dyn"          return DYN;

"\""           return DOUBLE_QUOTE;
"'"            return SINGLE_QUOTE;
"&"            return AND;
"&&"           return DOUBLE_AND;
"~"            return TILDE;
"!"            return BANG;
"|"            return PIPE;
"||"           return DOUBLE_PIPE;

"<"            return LESS;
">"            return GREATER;
">="           return GREATER_EQ;
"<="           return LESS_EQ;
"!="           return BANG_EQ;
"=="           return EQ_EQ;

"="            return EQ;
"+="           return PLUS_EQ;
"-="           return MINUS_EQ;
"++"           return PLUS_PLUS;
"--"           return MINUS_MINUS;

"echo"         return ECHO;
"if"           return IF;
"else"         return ELSE;
"while"        return WHILE;
"for"          return FOR;
"do"           return DO;

"type"         return TYPE;
"fun"          return FUN;
"ret"          return RET;
"->"           return ARROW;

"{"            return LEFT_BRACE;
"}"            return RIGHT_BRACE;
"("            return LEFT_PAREN;
")"            return RIGHT_PAREN;
"["            return LEFT_SQUARE;
"]"            return RIGHT_SQUARE;

"."            return DOT;
":"            return COLON;
";"            return SEMI_COLON;
%%
 
int yywrap(void) {
    return 1;
}
