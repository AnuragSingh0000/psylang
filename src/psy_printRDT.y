%{
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include "rdt.h"

int yylex(void);
void yyerror(const char *);
extern int yylineno;
extern int yycolumn;

extern FILE *yyin; 
const char *current_filename = "<stdin>";

static DTNode *rootTree = NULL;

static DTNode *makeTerminalDTNode(const char *name, const char *lexeme)
{
    DTNode *node = (DTNode *)malloc(sizeof(DTNode));
    if (!node) {
        yyerror("Out of memory while constructing Parse Tree");
        exit(1);
    }
    node->name = strdup(name);
    node->lexeme = lexeme ? strdup(lexeme) : NULL;
    node->num_children = 0;
    node->children = NULL;
    return node;
}

static DTNode *makeNonTerminalDTNode(const char *name, int num_children, ...)
{
    DTNode *node = (DTNode *)malloc(sizeof(DTNode));
    if (!node) {
        yyerror("Out of memory while constructing Parse Tree");
        exit(1);
    }
    node->name = strdup(name);
    node->lexeme = NULL;
    node->num_children = num_children;
    
    if (num_children > 0) {
        node->children = (DTNode **)malloc(num_children * sizeof(DTNode *));
        va_list args;
        va_start(args, num_children);
        for (int i = 0; i < num_children; i++) {
            node->children[i] = va_arg(args, DTNode *);
        }
        va_end(args);
    } else {
        node->children = NULL;
    }

    // Print the Reverse Derivation steps as reductions happen (Bottom-Up)
    printf(" %s ->", name);
    if (num_children == 0) {
        printf(" epsilon");
    } else {
        for (int i = 0; i < num_children; i++) {
            printf(" %s", node->children[i]->name);
        }
    }
    printf("\n");

    return node;
}

void printDerivationTree(DTNode *node, int depth)
{
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");
    if (node->lexeme) {
        printf("%s (%s)\n", node->name, node->lexeme);
    } else {
        printf("%s\n", node->name);
    }
    for (int i = 0; i < node->num_children; i++) {
        printDerivationTree(node->children[i], depth + 1);
    }
}
%}

%define parse.error verbose

%union {
    int ival;
    float fval;
    char* sval;
    bool bval;
    struct DTNode* node;
}

%token <ival> INT_CONSTANT
%token <fval> FLOAT_CONSTANT
%token <sval> IDENTIFIER STRING_CONSTANT
%token <bval> BOOL_CONSTANT
%token T_NULL "null"

%token VAR "var" INT "int" FLOAT "float" STRING "string" BOOL "bool" LIST "list" DYN "dyn"

%token PLUS "+" MINUS "-" STAR "*" SLASH "/" PERC "%"
%token LESS "<" GREATER ">" LESS_EQ "<=" GREATER_EQ ">=" BANG_EQ "!=" EQ_EQ "=="
%token AND "&" DOUBLE_AND "&&" PIPE "|" DOUBLE_PIPE "||" TILDE "~" XOR "^"
%token SHIFT_LEFT "<<" SHIFT_RIGHT ">>"
%token EQ "=" PLUS_EQ "+=" MINUS_EQ "-=" STAR_EQ "*=" SLASH_EQ "/=" PERC_EQ "%="
%token PLUS_PLUS "++" MINUS_MINUS "--"
%token QUESTION_MARK "?"
%token BANG "!"

%token ECH "echo"
%token IF "if" ELSE "else"
%token FOR "for" DO "do" WHILE "while" BREAK "break" CONTINUE "continue"

%token TYPE "type"
%token FUN "fun" RET "ret" ARROW "->" LAMBDA "lambda"

%token LEFT_PAREN "(" RIGHT_PAREN ")"
%token LEFT_BRACE "{" RIGHT_BRACE "}"
%token LEFT_SQUARE "[" RIGHT_SQUARE "]"
%token DOT "." COLON ":" SEMI_COLON ";" COMMA ","

%type <node> program decList declaration typeDecl funDecl function parameters typedVar var_type
%type <node> inTypeDeclList varDecl statement exprStmt forStmt forInit optExpr ifStmt echoStmt
%type <node> returnStmt
%type <node> whileStmt dowhileStmt breakStmt continueStmt block expression assignment
%type <node> ternary logic_or logic_and bitwise_or bitwise_xor bitwise_and equality comparison
%type <node> shift term factor unary call_access primary anonFunc arguments elements

%precedence LOWER_THAN_ELSE
%precedence ELSE
%locations

%%

program:
    decList
    {
        $$ = makeNonTerminalDTNode("program", 1, $1);
        rootTree = $$;
    }
    ;

decList:
    decList declaration { $$ = makeNonTerminalDTNode("decList", 2, $1, $2); }
    | declaration { $$ = makeNonTerminalDTNode("decList", 1, $1); }
    ;

declaration:
    typeDecl { $$ = makeNonTerminalDTNode("declaration", 1, $1); }
    | funDecl { $$ = makeNonTerminalDTNode("declaration", 1, $1); }
    | varDecl { $$ = makeNonTerminalDTNode("declaration", 1, $1); }
    | statement { $$ = makeNonTerminalDTNode("declaration", 1, $1); }
    ;

typeDecl:
    TYPE IDENTIFIER LEFT_BRACE inTypeDeclList RIGHT_BRACE
    {
        $$ = makeNonTerminalDTNode("typeDecl", 5,
            makeTerminalDTNode("TYPE", "type"), makeTerminalDTNode("IDENTIFIER", $2), makeTerminalDTNode("LEFT_BRACE", "{"), $4, makeTerminalDTNode("RIGHT_BRACE", "}"));
    }
    ;

funDecl:
    FUN IDENTIFIER function
    {
        $$ = makeNonTerminalDTNode("funDecl", 3, makeTerminalDTNode("FUN", "fun"), makeTerminalDTNode("IDENTIFIER", $2), $3);
    }
    ;

function:
    LEFT_PAREN parameters RIGHT_PAREN ARROW var_type block
    {
        $$ = makeNonTerminalDTNode("function", 6, makeTerminalDTNode("LEFT_PAREN", "("), $2, makeTerminalDTNode("RIGHT_PAREN", ")"), makeTerminalDTNode("ARROW", "->"), $5, $6);
    }
    | LEFT_PAREN RIGHT_PAREN ARROW var_type block
    {
        $$ = makeNonTerminalDTNode("function", 5, makeTerminalDTNode("LEFT_PAREN", "("), makeTerminalDTNode("RIGHT_PAREN", ")"), makeTerminalDTNode("ARROW", "->"), $4, $5);
    }
    ;

parameters:
    parameters COMMA typedVar { $$ = makeNonTerminalDTNode("parameters", 3, $1, makeTerminalDTNode("COMMA", ","), $3); }
    | typedVar { $$ = makeNonTerminalDTNode("parameters", 1, $1); }
    ;

typedVar:
    IDENTIFIER COLON var_type { $$ = makeNonTerminalDTNode("typedVar", 3, makeTerminalDTNode("IDENTIFIER", $1), makeTerminalDTNode("COLON", ":"), $3); }
    ;

var_type:
    INT { $$ = makeNonTerminalDTNode("var_type", 1, makeTerminalDTNode("INT", "int")); }
    | FLOAT { $$ = makeNonTerminalDTNode("var_type", 1, makeTerminalDTNode("FLOAT", "float")); }
    | STRING { $$ = makeNonTerminalDTNode("var_type", 1, makeTerminalDTNode("STRING", "string")); }
    | BOOL { $$ = makeNonTerminalDTNode("var_type", 1, makeTerminalDTNode("BOOL", "bool")); }
    | DYN { $$ = makeNonTerminalDTNode("var_type", 1, makeTerminalDTNode("DYN", "dyn")); }
    | T_NULL { $$ = makeNonTerminalDTNode("var_type", 1, makeTerminalDTNode("NULL", "null")); }
    | LIST LEFT_SQUARE INT_CONSTANT RIGHT_SQUARE
    {
        char buf[32]; snprintf(buf, sizeof(buf), "%d", $3);
        $$ = makeNonTerminalDTNode("var_type", 4, makeTerminalDTNode("LIST", "list"), makeTerminalDTNode("LEFT_SQUARE", "["), makeTerminalDTNode("INT_CONSTANT", buf), makeTerminalDTNode("RIGHT_SQUARE", "]"));
    }
    | IDENTIFIER { $$ = makeNonTerminalDTNode("var_type", 1, makeTerminalDTNode("IDENTIFIER", $1)); }
    ;

inTypeDeclList:
    inTypeDeclList varDecl { $$ = makeNonTerminalDTNode("inTypeDeclList", 2, $1, $2); }
    | varDecl { $$ = makeNonTerminalDTNode("inTypeDeclList", 1, $1); }
    ;

varDecl:
    VAR var_type IDENTIFIER EQ expression SEMI_COLON
    {
        $$ = makeNonTerminalDTNode("varDecl", 6, makeTerminalDTNode("VAR", "var"), $2, makeTerminalDTNode("IDENTIFIER", $3), makeTerminalDTNode("EQ", "="), $5, makeTerminalDTNode("SEMI_COLON", ";"));
    }
    | VAR var_type IDENTIFIER SEMI_COLON
    {
        $$ = makeNonTerminalDTNode("varDecl", 4, makeTerminalDTNode("VAR", "var"), $2, makeTerminalDTNode("IDENTIFIER", $3), makeTerminalDTNode("SEMI_COLON", ";"));
    }
    ;

statement:
    exprStmt { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    | forStmt { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    | ifStmt { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    | echoStmt { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    | returnStmt { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    | whileStmt { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    | dowhileStmt { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    | breakStmt { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    | continueStmt { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    | block { $$ = makeNonTerminalDTNode("statement", 1, $1); }
    ;

exprStmt:
    expression SEMI_COLON { $$ = makeNonTerminalDTNode("exprStmt", 2, $1, makeTerminalDTNode("SEMI_COLON", ";")); }
    ;

forStmt:
    FOR LEFT_PAREN forInit optExpr SEMI_COLON optExpr RIGHT_PAREN statement
    {
        $$ = makeNonTerminalDTNode("forStmt", 8, makeTerminalDTNode("FOR", "for"), makeTerminalDTNode("LEFT_PAREN", "("), $3, $4, makeTerminalDTNode("SEMI_COLON", ";"), $6, makeTerminalDTNode("RIGHT_PAREN", ")"), $8);
    }
    ;

forInit:
    varDecl { $$ = makeNonTerminalDTNode("forInit", 1, $1); }
    | exprStmt { $$ = makeNonTerminalDTNode("forInit", 1, $1); }
    | SEMI_COLON { $$ = makeNonTerminalDTNode("forInit", 1, makeTerminalDTNode("SEMI_COLON", ";")); }
    ;

optExpr:
    expression { $$ = makeNonTerminalDTNode("optExpr", 1, $1); }
    | /* empty */ { $$ = makeNonTerminalDTNode("optExpr", 0); }
    ;

ifStmt:
    IF LEFT_PAREN expression RIGHT_PAREN statement ELSE statement
    {
        $$ = makeNonTerminalDTNode("ifStmt", 7, makeTerminalDTNode("IF", "if"), makeTerminalDTNode("LEFT_PAREN", "("), $3, makeTerminalDTNode("RIGHT_PAREN", ")"), $5, makeTerminalDTNode("ELSE", "else"), $7);
    }
    | IF LEFT_PAREN expression RIGHT_PAREN statement %prec LOWER_THAN_ELSE
    {
        $$ = makeNonTerminalDTNode("ifStmt", 5, makeTerminalDTNode("IF", "if"), makeTerminalDTNode("LEFT_PAREN", "("), $3, makeTerminalDTNode("RIGHT_PAREN", ")"), $5);
    }
    ;

echoStmt:
    ECH expression SEMI_COLON { $$ = makeNonTerminalDTNode("echoStmt", 3, makeTerminalDTNode("ECH", "echo"), $2, makeTerminalDTNode("SEMI_COLON", ";")); }
    ;

returnStmt:
    RET expression SEMI_COLON { $$ = makeNonTerminalDTNode("returnStmt", 3, makeTerminalDTNode("RET", "ret"), $2, makeTerminalDTNode("SEMI_COLON", ";")); }
    | RET SEMI_COLON { $$ = makeNonTerminalDTNode("returnStmt", 2, makeTerminalDTNode("RET", "ret"), makeTerminalDTNode("SEMI_COLON", ";")); }
    ;

whileStmt:
    WHILE LEFT_PAREN expression RIGHT_PAREN statement { $$ = makeNonTerminalDTNode("whileStmt", 5, makeTerminalDTNode("WHILE", "while"), makeTerminalDTNode("LEFT_PAREN", "("), $3, makeTerminalDTNode("RIGHT_PAREN", ")"), $5); }
    ;

dowhileStmt:
    DO statement WHILE LEFT_PAREN expression RIGHT_PAREN SEMI_COLON { $$ = makeNonTerminalDTNode("dowhileStmt", 7, makeTerminalDTNode("DO", "do"), $2, makeTerminalDTNode("WHILE", "while"), makeTerminalDTNode("LEFT_PAREN", "("), $5, makeTerminalDTNode("RIGHT_PAREN", ")"), makeTerminalDTNode("SEMI_COLON", ";")); }
    ;

breakStmt:
    BREAK SEMI_COLON { $$ = makeNonTerminalDTNode("breakStmt", 2, makeTerminalDTNode("BREAK", "break"), makeTerminalDTNode("SEMI_COLON", ";")); }
    ;

continueStmt:
    CONTINUE SEMI_COLON { $$ = makeNonTerminalDTNode("continueStmt", 2, makeTerminalDTNode("CONTINUE", "continue"), makeTerminalDTNode("SEMI_COLON", ";")); }
    ;

block:
    LEFT_BRACE decList RIGHT_BRACE { $$ = makeNonTerminalDTNode("block", 3, makeTerminalDTNode("LEFT_BRACE", "{"), $2, makeTerminalDTNode("RIGHT_BRACE", "}")); }
    | LEFT_BRACE RIGHT_BRACE { $$ = makeNonTerminalDTNode("block", 2, makeTerminalDTNode("LEFT_BRACE", "{"), makeTerminalDTNode("RIGHT_BRACE", "}")); }
    ;

expression:
    assignment { $$ = makeNonTerminalDTNode("expression", 1, $1); }
    ;

assignment: 
    call_access EQ assignment { $$ = makeNonTerminalDTNode("assignment", 3, $1, makeTerminalDTNode("EQ", "="), $3); }
    | call_access PLUS_EQ assignment { $$ = makeNonTerminalDTNode("assignment", 3, $1, makeTerminalDTNode("PLUS_EQ", "+="), $3); }
    | call_access MINUS_EQ assignment { $$ = makeNonTerminalDTNode("assignment", 3, $1, makeTerminalDTNode("MINUS_EQ", "-="), $3); }
    | call_access STAR_EQ assignment { $$ = makeNonTerminalDTNode("assignment", 3, $1, makeTerminalDTNode("STAR_EQ", "*="), $3); }
    | call_access SLASH_EQ assignment { $$ = makeNonTerminalDTNode("assignment", 3, $1, makeTerminalDTNode("SLASH_EQ", "/="), $3); }
    | call_access PERC_EQ assignment { $$ = makeNonTerminalDTNode("assignment", 3, $1, makeTerminalDTNode("PERC_EQ", "%="), $3); }
    | ternary { $$ = makeNonTerminalDTNode("assignment", 1, $1); }
    ;

ternary:
    logic_or QUESTION_MARK assignment COLON assignment { $$ = makeNonTerminalDTNode("ternary", 5, $1, makeTerminalDTNode("QUESTION_MARK", "?"), $3, makeTerminalDTNode("COLON", ":"), $5); }
    | logic_or { $$ = makeNonTerminalDTNode("ternary", 1, $1); }
    ;

logic_or:
    logic_or DOUBLE_PIPE logic_and { $$ = makeNonTerminalDTNode("logic_or", 3, $1, makeTerminalDTNode("DOUBLE_PIPE", "||"), $3); }
    | logic_and { $$ = makeNonTerminalDTNode("logic_or", 1, $1); }
    ;

logic_and:
    logic_and DOUBLE_AND bitwise_or { $$ = makeNonTerminalDTNode("logic_and", 3, $1, makeTerminalDTNode("DOUBLE_AND", "&&"), $3); }
    | bitwise_or { $$ = makeNonTerminalDTNode("logic_and", 1, $1); }
    ;

bitwise_or:
    bitwise_or PIPE bitwise_xor { $$ = makeNonTerminalDTNode("bitwise_or", 3, $1, makeTerminalDTNode("PIPE", "|"), $3); }
    | bitwise_xor { $$ = makeNonTerminalDTNode("bitwise_or", 1, $1); }
    ;

bitwise_xor:
    bitwise_xor XOR bitwise_and { $$ = makeNonTerminalDTNode("bitwise_xor", 3, $1, makeTerminalDTNode("XOR", "^"), $3); }
    | bitwise_and { $$ = makeNonTerminalDTNode("bitwise_xor", 1, $1); }
    ;

bitwise_and:
    bitwise_and AND equality { $$ = makeNonTerminalDTNode("bitwise_and", 3, $1, makeTerminalDTNode("AND", "&"), $3); }
    | equality { $$ = makeNonTerminalDTNode("bitwise_and", 1, $1); }
    ;

equality:
    equality BANG_EQ comparison { $$ = makeNonTerminalDTNode("equality", 3, $1, makeTerminalDTNode("BANG_EQ", "!="), $3); }
    | equality EQ_EQ comparison { $$ = makeNonTerminalDTNode("equality", 3, $1, makeTerminalDTNode("EQ_EQ", "=="), $3); }
    | comparison { $$ = makeNonTerminalDTNode("equality", 1, $1); }
    ;

comparison:
    comparison GREATER shift { $$ = makeNonTerminalDTNode("comparison", 3, $1, makeTerminalDTNode("GREATER", ">"), $3); }
    | comparison LESS shift { $$ = makeNonTerminalDTNode("comparison", 3, $1, makeTerminalDTNode("LESS", "<"), $3); }
    | comparison GREATER_EQ shift { $$ = makeNonTerminalDTNode("comparison", 3, $1, makeTerminalDTNode("GREATER_EQ", ">="), $3); }
    | comparison LESS_EQ shift { $$ = makeNonTerminalDTNode("comparison", 3, $1, makeTerminalDTNode("LESS_EQ", "<="), $3); }
    | shift { $$ = makeNonTerminalDTNode("comparison", 1, $1); }
    ;

shift:
    shift SHIFT_LEFT term { $$ = makeNonTerminalDTNode("shift", 3, $1, makeTerminalDTNode("SHIFT_LEFT", "<<"), $3); }
    | shift SHIFT_RIGHT term { $$ = makeNonTerminalDTNode("shift", 3, $1, makeTerminalDTNode("SHIFT_RIGHT", ">>"), $3); }
    | term { $$ = makeNonTerminalDTNode("shift", 1, $1); }
    ;

term:
    term PLUS factor { $$ = makeNonTerminalDTNode("term", 3, $1, makeTerminalDTNode("PLUS", "+"), $3); }
    | term MINUS factor { $$ = makeNonTerminalDTNode("term", 3, $1, makeTerminalDTNode("MINUS", "-"), $3); }
    | factor { $$ = makeNonTerminalDTNode("term", 1, $1); }
    ;

factor:
    factor SLASH unary { $$ = makeNonTerminalDTNode("factor", 3, $1, makeTerminalDTNode("SLASH", "/"), $3); }
    | factor STAR unary { $$ = makeNonTerminalDTNode("factor", 3, $1, makeTerminalDTNode("STAR", "*"), $3); }
    | factor PERC unary { $$ = makeNonTerminalDTNode("factor", 3, $1, makeTerminalDTNode("PERC", "%"), $3); }
    | unary { $$ = makeNonTerminalDTNode("factor", 1, $1); }
    ;

unary:
    BANG unary { $$ = makeNonTerminalDTNode("unary", 2, makeTerminalDTNode("BANG", "!"), $2); }
    | TILDE unary { $$ = makeNonTerminalDTNode("unary", 2, makeTerminalDTNode("TILDE", "~"), $2); }
    | MINUS unary { $$ = makeNonTerminalDTNode("unary", 2, makeTerminalDTNode("MINUS", "-"), $2); }
    | PLUS_PLUS unary { $$ = makeNonTerminalDTNode("unary", 2, makeTerminalDTNode("PLUS_PLUS", "++"), $2); }
    | MINUS_MINUS unary { $$ = makeNonTerminalDTNode("unary", 2, makeTerminalDTNode("MINUS_MINUS", "--"), $2); }
    | call_access { $$ = makeNonTerminalDTNode("unary", 1, $1); }
    ;

call_access:
    call_access LEFT_PAREN arguments RIGHT_PAREN { $$ = makeNonTerminalDTNode("call_access", 4, $1, makeTerminalDTNode("LEFT_PAREN", "("), $3, makeTerminalDTNode("RIGHT_PAREN", ")")); }
    | call_access LEFT_PAREN RIGHT_PAREN { $$ = makeNonTerminalDTNode("call_access", 3, $1, makeTerminalDTNode("LEFT_PAREN", "("), makeTerminalDTNode("RIGHT_PAREN", ")")); }
    | call_access DOT IDENTIFIER { $$ = makeNonTerminalDTNode("call_access", 3, $1, makeTerminalDTNode("DOT", "."), makeTerminalDTNode("IDENTIFIER", $3)); }
    | call_access LEFT_SQUARE expression RIGHT_SQUARE { $$ = makeNonTerminalDTNode("call_access", 4, $1, makeTerminalDTNode("LEFT_SQUARE", "["), $3, makeTerminalDTNode("RIGHT_SQUARE", "]")); }
    | call_access PLUS_PLUS { $$ = makeNonTerminalDTNode("call_access", 2, $1, makeTerminalDTNode("PLUS_PLUS", "++")); }
    | call_access MINUS_MINUS { $$ = makeNonTerminalDTNode("call_access", 2, $1, makeTerminalDTNode("MINUS_MINUS", "--")); }
    | primary { $$ = makeNonTerminalDTNode("call_access", 1, $1); }
    ;

primary:
    BOOL_CONSTANT { $$ = makeNonTerminalDTNode("primary", 1, makeTerminalDTNode("BOOL_CONSTANT", $1 ? "true" : "false")); }
    | T_NULL { $$ = makeNonTerminalDTNode("primary", 1, makeTerminalDTNode("T_NULL", "null")); }
    | INT_CONSTANT { 
        char buf[32]; snprintf(buf, sizeof(buf), "%d", $1); 
        $$ = makeNonTerminalDTNode("primary", 1, makeTerminalDTNode("INT_CONSTANT", buf)); 
    }
    | FLOAT_CONSTANT { 
        char buf[32]; snprintf(buf, sizeof(buf), "%f", $1); 
        $$ = makeNonTerminalDTNode("primary", 1, makeTerminalDTNode("FLOAT_CONSTANT", buf)); 
    }
    | STRING_CONSTANT { $$ = makeNonTerminalDTNode("primary", 1, makeTerminalDTNode("STRING_CONSTANT", $1)); }
    | IDENTIFIER { $$ = makeNonTerminalDTNode("primary", 1, makeTerminalDTNode("IDENTIFIER", $1)); }
    | anonFunc { $$ = makeNonTerminalDTNode("primary", 1, $1); }
    | LEFT_PAREN expression RIGHT_PAREN { $$ = makeNonTerminalDTNode("primary", 3, makeTerminalDTNode("LEFT_PAREN", "("), $2, makeTerminalDTNode("RIGHT_PAREN", ")")); }
    | LEFT_SQUARE elements RIGHT_SQUARE { $$ = makeNonTerminalDTNode("primary", 3, makeTerminalDTNode("LEFT_SQUARE", "["), $2, makeTerminalDTNode("RIGHT_SQUARE", "]")); }
    | LEFT_SQUARE RIGHT_SQUARE { $$ = makeNonTerminalDTNode("primary", 2, makeTerminalDTNode("LEFT_SQUARE", "["), makeTerminalDTNode("RIGHT_SQUARE", "]")); }
    ;

anonFunc:
    LAMBDA function { $$ = makeNonTerminalDTNode("anonFunc", 2, makeTerminalDTNode("LAMBDA", "lambda"), $2); }
    ;

arguments:
    arguments COMMA expression { $$ = makeNonTerminalDTNode("arguments", 3, $1, makeTerminalDTNode("COMMA", ","), $3); }
    | expression { $$ = makeNonTerminalDTNode("arguments", 1, $1); }
    ;

elements:
    elements COMMA expression { $$ = makeNonTerminalDTNode("elements", 3, $1, makeTerminalDTNode("COMMA", ","), $3); }
    | expression { $$ = makeNonTerminalDTNode("elements", 1, $1); }
    ;

%%


int main(int argc, char **argv) {
    if (argc > 1) {
        current_filename = argv[1]; 
        yyin = fopen(current_filename, "r");
        
        if (!yyin) {
            fprintf(stderr, "Error: Could not open file '%s'\n", current_filename);
            return 1;
        }
    } else {
        current_filename = "<stdin>";
    }

    int result = yyparse();
    if (result == 0)
    {
        printf("\nParsed successfully.\n");
        printf("\n=== VISUAL DERIVATION TREE (Parse Tree) ===\n");
        printDerivationTree(rootTree, 0);
    }

    if (yyin && yyin != stdin) {
        fclose(yyin);
    }

    return result;
}

void print_fancy_error(int line, int col, const char *message) {
    fflush(stdout);
    int use_color = isatty(fileno(stderr));

    const char *color_red = use_color ? "\033[1;31m" : "";
    const char *color_bold = use_color ? "\033[1m" : "";
    const char *color_reset = use_color ? "\033[0m" : "";

    fprintf(stderr, "%serror:%s %s%s%s\n", color_red, color_reset, color_bold, message, color_reset);
    fprintf(stderr, "  --> %s:%d:%d\n", current_filename, line, col);

    if (strcmp(current_filename, "<stdin>") == 0) {
        return; 
    }

    FILE *file = fopen(current_filename, "r");
    if (!file) return;
    
    char buffer[1024];
    int current_line = 1;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (current_line == line) {
            buffer[strcspn(buffer, "\n")] = 0;
            fprintf(stderr, "   |\n");
            fprintf(stderr, "%3d| %s\n", line, buffer);
            fprintf(stderr, "   | ");
            for (int i = 1; i < col - 1; i++) {
                if (buffer[i-1] == '\t') fprintf(stderr, "\t");
                else fprintf(stderr, " ");
            }
            fprintf(stderr, "%s^--- Here%s\n\n", color_red, color_reset);
            break;
        }
        current_line++;
    }
    fclose(file);
}

void yyerror(const char *message) {
    print_fancy_error(yylineno, yycolumn, message);
}