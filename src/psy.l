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
">"            return IF;
"<"            return ELSE;
"!"            return BANG;
"var"          return VAR;
"int"          return INT;
"float"        return FLOAT;
"string"       return STRING;
"\""           return DOUBLE_QUOTE;
"'"            return SINGLE_QUOTE;
"&"            return AND;
"&&"           return DOUBLE_AND;
"|"            return PIPE;
"||"           return DOUBLE_PIPE;

%%
 
int yywrap(void) {
    return 1;
}
