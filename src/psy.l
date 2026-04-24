%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "defs.h"
#include "psy.tab.h"

int yycolumn = 1;

/* This macro runs before EVERY matched token to update the location */
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

#define MAX_STR_CONST 65536
char string_buf[MAX_STR_CONST];
char *string_pos;

void yyerror(char *);
%}

%option yylineno

%x STR_DOUBLE STR_SINGLE

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
"bool"         return BOOL;
"dyn"          return DYN;
"null"         return T_NULL;
"list"         return LIST;

"&"            return AND;
"&&"           return DOUBLE_AND;
"~"            return TILDE;
"^"            return XOR;
"!"            return BANG;
"|"            return PIPE;
"||"           return DOUBLE_PIPE;
"<<"           return SHIFT_LEFT;
">>"           return SHIFT_RIGHT;

"<"            return LESS;
">"            return GREATER;
">="           return GREATER_EQ;
"<="           return LESS_EQ;
"!="           return BANG_EQ;
"=="           return EQ_EQ;

"="            return EQ;
"+="           return PLUS_EQ;
"-="           return MINUS_EQ;
"*="           return STAR_EQ;
"/="           return SLASH_EQ;
"%="           return PERC_EQ;

"++"           return PLUS_PLUS;
"--"           return MINUS_MINUS;

"echo"         return ECH;
"if"           return IF;
"else"         return ELSE;
"while"        return WHILE;
"for"          return FOR;
"do"           return DO;
"break"        return BREAK;
"continue"     return CONTINUE;

"type"         return TYPE;
"fun"          return FUN;
"lambda"       return LAMBDA;
"ret"          return RET;
"->"           return ARROW;

"{"            return LEFT_BRACE;
"}"            return RIGHT_BRACE;
"("            return LEFT_PAREN;
")"            return RIGHT_PAREN;
"["            return LEFT_SQUARE;
"]"            return RIGHT_SQUARE;

"."            return DOT;
","            return COMMA;
":"            return COLON;
";"            return SEMI_COLON;
"?"            return QUESTION_MARK;

"true"         { yylval.bval = true; return BOOL_CONSTANT; }
"false"        { yylval.bval = false; return BOOL_CONSTANT; }

\"      {
    BEGIN(STR_DOUBLE);
    string_pos = string_buf;
}

\'      {
    BEGIN(STR_SINGLE);
    string_pos = string_buf;
}

<STR_DOUBLE>\" {
    BEGIN(INITIAL);
    *string_pos = '\0';
    yylval.sval = strdup(string_buf);
    return STRING_CONSTANT;
}

<STR_SINGLE>\' {
    BEGIN(INITIAL);
    *string_pos = '\0';
    yylval.sval = strdup(string_buf);
    return STRING_CONSTANT;
}

<STR_DOUBLE,STR_SINGLE>\\\"   { *string_pos++ = '"';  }
<STR_DOUBLE,STR_SINGLE>\\\'   { *string_pos++ = '\''; }

<STR_DOUBLE,STR_SINGLE>\\n    { *string_pos++ = '\n'; }
<STR_DOUBLE,STR_SINGLE>\\t    { *string_pos++ = '\t'; }
<STR_DOUBLE,STR_SINGLE>\\\\   { *string_pos++ = '\\'; }

<STR_DOUBLE,STR_SINGLE>\\.    {
    fflush(stdout);
    fprintf(stderr, "[Error at Line %d]: Invalid escape sequence \"\%s\".\n", yylineno, yytext);
    exit(1);
} // In case the escape sequence is invalid, an error is raised

<STR_DOUBLE,STR_SINGLE>\n     {
    fflush(stdout);
    fprintf(stderr, "[Error at Line %d]: Unterminated string literal (found newline).\n", yylineno);
    exit(1);
}

<STR_DOUBLE,STR_SINGLE><<EOF>> {
    fflush(stdout);
    fprintf(stderr, "[Error at Line %d]: Unterminated string literal (end of file).\n", yylineno);
    exit(1);
}

<STR_DOUBLE>[^\\\n"]+ {
    char *yptr = yytext;
    while ( *yptr ) *string_pos++ = *yptr++;
}

<STR_SINGLE>[^\\\n']+ {
    char *yptr = yytext;
    while ( *yptr ) *string_pos++ = *yptr++;
}

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

[0-9][_A-Za-z0-9]* {
    fflush(stdout);
    fprintf(stderr, "[Error at Line %d]: Identifier cannot begin with a digit: %s\n", yylineno, yytext);
    exit(1);
}

\$\$[^\n]*      {}  /* ignore rest of the line after $$ (but do not consume the newline). For comments */
  
[ \t\r]+      {}

[\n]+         { yycolumn = 1; }

.               { fflush(stdout); fprintf(stderr, "[Error at Line %d]: Unexpected token: %s\n", yylineno, yytext); exit(1); }

%% 

int yywrap(void) {
    return 1;
}