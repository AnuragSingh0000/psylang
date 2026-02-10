%{
#include <stdlib.h>
#include <string.h>
#include "psy.tab.h"

int lexerror(void)
{
    fprintf(stderr, "Unexpected token %s, at line %d\n", yytext, yylineno);
    return 0;
}
%}

%option yylineno

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

"echo"         return ECH;
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

[_A-Za-z][_A-Za-z0-9]* {
    yylval.sval = strdup(yytext);
    return IDENTIFIER;
}

[0-9]+ {
    yylval.ival = atoi(yytext);
    return INT_CONSTANT;
}

[0-9]+[\.][0-9]+ {
    yylval.fval = atof(yytext);
    return FLOAT_CONSTANT;
}
                        
\"[^\n"]*\" {
    yylval.sval = strdup(yytext);
    return STRING_CONSTANT;
}    

\'[^\n']*\' {
    yylval.sval = strdup(yytext);
    return STRING_CONSTANT;
}    
  
[ \t\r\n]+      {}

.     {lexerror(); exit(1);}

%%
 

int yywrap(void) {
    return 1;
}
