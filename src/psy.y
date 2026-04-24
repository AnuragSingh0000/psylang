%{
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "defs.h"
#include "ast_printer.h"
#include "semantic.h"

int yylex(void);
void yyerror(const char *);
extern int yylineno;
extern int yycolumn;
extern FILE *yyin;
const char *current_filename = "<stdin>";

static Node *rootAst = NULL;
static bool should_dump_ir(void)
{
    const char *v = getenv("DUMP_IR");
    return v && v[0] != '\0' && strcmp(v, "0") != 0;
}

static bool should_dump_ir_only(void)
{
    const char *v = getenv("DUMP_IR_ONLY");
    return v && v[0] != '\0' && strcmp(v, "0") != 0;
}

static bool should_debug(void)
{
    const char *v = getenv("PSY_DEBUG");
    return v && v[0] != '\0' && strcmp(v, "0") != 0;
}

static Node *makeNode(NodeType type, int line, int col)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        yyerror("Out of memory while constructing AST");
        exit(1);
    }

    memset(node, 0, sizeof(Node));
    node->type = type;
    node->line = line;
    node->pos = col;
    node->symbol_table_index = SYMBOL_INDEX_UNRESOLVED;
    node->next = NULL;

    return node;
}

static Node *appendList(Node *list, Node *value)
{
    Node *cur;
    if (!value) return list;

    if (!list)
    {
        value->next = NULL;
        return value;
    }

    cur = list;
    while (cur->next)
    {
        cur = cur->next;
    }
    cur->next = value;
    value->next = NULL;
    return list;
}

static Node *makePrimaryNode(PrimaryNodeType type, int line, int col)
{
    Node *node = makeNode(NODE_PRIMARY, line, col);
    node->as.primary.type = type;
    return node;
}

static Node *makeUnaryNode(OperatorType op, Node *expr, int line, int col)
{
    Node *node = makeNode(NODE_UNARY_OP, line, col);
    node->as.unary.op = op;
    node->as.unary.expr = expr;
    return node;
}

static Node *makeBinaryNode(OperatorType op, Node *left, Node *right, int line, int col)
{
    Node *node = makeNode(NODE_BINARY_OP, line, col);
    node->as.binary.op = op;
    node->as.binary.left = left;
    node->as.binary.right = right;
    return node;
}

static Node *makeAssignNode(OperatorType op, Node *left, Node *right, int line, int col)
{
    Node *node = makeNode(NODE_ASSIGN_OP, line, col);
    node->as.assign.op = op;
    node->as.assign.left = left;
    node->as.assign.right = right;
    return node;
}

%}


%union {
    int ival;
    float fval;
    char* sval;
    bool bval;
    Node* node;
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
%token DOT "."
COLON ":" SEMI_COLON ";" COMMA ","

%type <node> program decList declaration typeDecl funDecl function parameters typedVar var_type
%type <node> inTypeDeclList varDecl statement exprStmt forStmt forInit optExpr ifStmt echoStmt
%type <node> returnStmt whileStmt dowhileStmt breakStmt continueStmt block expression assignment
%type <node> ternary logic_or logic_and bitwise_or bitwise_xor bitwise_and equality comparison
%type <node> shift term factor unary call_access primary anonFunc arguments elements

%precedence LOWER_THAN_ELSE
%precedence ELSE
%locations
%define parse.error verbose

%%


program:
    decList
    {
        Node *node = makeNode(NODE_PROGRAM, @$.first_line, @$.first_column);
        node->as.program.declarations = $1;
        rootAst = node;
        $$ = node;
    }
    ;
decList:
    decList declaration
    {
        $$ = appendList($1, $2);
    }
    | declaration
    {
        $$ = appendList(NULL, $1);
    }
    ;

declaration:
    typeDecl { $$ = $1; }
    | funDecl { $$ = $1; }
    | varDecl { $$ = $1; }
    | statement { $$ = $1; }
    ;

typeDecl:
    TYPE IDENTIFIER LEFT_BRACE inTypeDeclList RIGHT_BRACE
    {
        Node *node = makeNode(NODE_TYPE_DECL, @$.first_line, @$.first_column);
        node->as.typeDecl.name = $2;
        node->as.typeDecl.fields = $4;
        $$ = node;
    }
    ;
funDecl:
    FUN IDENTIFIER function
    {
        Node *node = makeNode(NODE_FUN_DECL, @$.first_line, @$.first_column);
        node->as.funDecl.name = $2;
        node->as.funDecl.function = $3;
        $$ = node;
    }
    ;
function:
    LEFT_PAREN parameters RIGHT_PAREN ARROW var_type block
    {
        Node *node = makeNode(NODE_FUNCTION, @$.first_line, @$.first_column);
        node->as.function.parameters = $2;
        node->as.function.returnType = $5;
        node->as.function.body = $6;
        $$ = node;
    }
    |
    LEFT_PAREN RIGHT_PAREN ARROW var_type block
    {
        Node *node = makeNode(NODE_FUNCTION, @$.first_line, @$.first_column);
        node->as.function.parameters = NULL;
        node->as.function.returnType = $4;
        node->as.function.body = $5;
        $$ = node;
    }
    ;
parameters:
    parameters COMMA typedVar { $$ = appendList($1, $3); }
    | typedVar { $$ = appendList(NULL, $1); }
    ;

typedVar:
    IDENTIFIER COLON var_type
    {
        Node *node = makeNode(NODE_TYPED_VAR, @$.first_line, @$.first_column);
        node->as.typedVar.name = $1;
        node->as.typedVar.varType = $3;
        $$ = node;
    }
    ;
var_type:
    INT
    {
        Node *node = makeNode(NODE_VAR_TYPE, @$.first_line, @$.first_column);
        node->as.varType.kind = VAR_TYPE_INT;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        $$ = node;
    }
    |
    FLOAT
    {
        Node *node = makeNode(NODE_VAR_TYPE, @$.first_line, @$.first_column);
        node->as.varType.kind = VAR_TYPE_FLOAT;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        $$ = node;
    }
    |
    STRING
    {
        Node *node = makeNode(NODE_VAR_TYPE, @$.first_line, @$.first_column);
        node->as.varType.kind = VAR_TYPE_STRING;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        $$ = node;
    }
    |
    BOOL
    {
        Node *node = makeNode(NODE_VAR_TYPE, @$.first_line, @$.first_column);
        node->as.varType.kind = VAR_TYPE_BOOL;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        $$ = node;
    }
    |
    DYN
    {
        Node *node = makeNode(NODE_VAR_TYPE, @$.first_line, @$.first_column);
        node->as.varType.kind = VAR_TYPE_DYN;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        $$ = node;
    }
    |
    LIST LEFT_SQUARE INT_CONSTANT RIGHT_SQUARE
    {
        Node *node = makeNode(NODE_VAR_TYPE, @$.first_line, @$.first_column);
        node->as.varType.kind = VAR_TYPE_LIST;
        node->as.varType.listSize = $3;
        node->as.varType.customTypeName = NULL;
        $$ = node;
    }
    |
    T_NULL
    {
        Node *node = makeNode(NODE_VAR_TYPE, @$.first_line, @$.first_column);
        node->as.varType.kind = VAR_TYPE_NULL;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = NULL;
        $$ = node;
    }
    |
    IDENTIFIER
    {
        Node *node = makeNode(NODE_VAR_TYPE, @$.first_line, @$.first_column);
        node->as.varType.kind = VAR_TYPE_CUSTOM;
        node->as.varType.listSize = 0;
        node->as.varType.customTypeName = $1;
        $$ = node;
    }
    ;

inTypeDeclList:
    inTypeDeclList varDecl { $$ = appendList($1, $2); }
    | varDecl { $$ = appendList(NULL, $1); }
    ;

varDecl:
    VAR var_type IDENTIFIER EQ expression SEMI_COLON
    {
        Node *node = makeNode(NODE_VAR_DECL, @$.first_line, @$.first_column);
        node->as.varDecl.varType = $2;
        node->as.varDecl.name = $3;
        node->as.varDecl.initializer = $5;
        $$ = node;
    }
    |
    VAR var_type IDENTIFIER SEMI_COLON
    {
        Node *node = makeNode(NODE_VAR_DECL, @$.first_line, @$.first_column);
        node->as.varDecl.varType = $2;
        node->as.varDecl.name = $3;
        node->as.varDecl.initializer = NULL;
        $$ = node;
    }
    ;
statement:
    exprStmt { $$ = $1; }
    | forStmt { $$ = $1; }
    | ifStmt { $$ = $1; }
    | echoStmt { $$ = $1; }
    | returnStmt { $$ = $1; }
    | whileStmt { $$ = $1; }
    | dowhileStmt { $$ = $1; }
    | breakStmt { $$ = $1; }
    | continueStmt { $$ = $1; }
    | block { $$ = $1; }
    ;

exprStmt:
    expression SEMI_COLON
    {
        Node *node = makeNode(NODE_EXPR_STMT, @$.first_line, @$.first_column);
        node->as.exprStmt.expr = $1;
        $$ = node;
    }
    ;
forStmt:
    FOR LEFT_PAREN forInit optExpr SEMI_COLON optExpr RIGHT_PAREN statement
    {
        Node *node = makeNode(NODE_FOR_STMT, @$.first_line, @$.first_column);
        node->as.forStmt.init = $3;
        node->as.forStmt.condition = $4;
        node->as.forStmt.increment = $6;
        node->as.forStmt.body = $8;
        $$ = node;
    }
    ;
forInit:
    varDecl { $$ = $1; }
    | exprStmt { $$ = $1; }
    | SEMI_COLON { $$ = NULL; }
    ;

optExpr:
    expression { $$ = $1; }
    | { $$ = NULL; }
    ;

ifStmt:
    IF LEFT_PAREN expression RIGHT_PAREN statement ELSE statement
    {
        Node *node = makeNode(NODE_IF_STMT, @$.first_line, @$.first_column);
        node->as.ifStmt.condition = $3;
        node->as.ifStmt.thenBranch = $5;
        node->as.ifStmt.elseBranch = $7;
        $$ = node;
    }
    |
    IF LEFT_PAREN expression RIGHT_PAREN statement %prec LOWER_THAN_ELSE
    {
        Node *node = makeNode(NODE_IF_STMT, @$.first_line, @$.first_column);
        node->as.ifStmt.condition = $3;
        node->as.ifStmt.thenBranch = $5;
        node->as.ifStmt.elseBranch = NULL;
        $$ = node;
    }
    ;
echoStmt:
    ECH expression SEMI_COLON
    {
        Node *node = makeNode(NODE_ECHO_STMT, @$.first_line, @$.first_column);
        node->as.echoStmt.expr = $2;
        $$ = node;
    }
    ;
returnStmt:
    RET expression SEMI_COLON
    {
        Node *node = makeNode(NODE_RETURN, @$.first_line, @$.first_column);
        node->as.returnStmt.expr = $2;
        $$ = node;
    }
    |
    RET SEMI_COLON
    {
        Node *node = makeNode(NODE_RETURN, @$.first_line, @$.first_column);
        node->as.returnStmt.expr = NULL;
        $$ = node;
    }
    ;

whileStmt:
    WHILE LEFT_PAREN expression RIGHT_PAREN statement
    {
        Node *node = makeNode(NODE_WHILE_STMT, @$.first_line, @$.first_column);
        node->as.whileStmt.condition = $3;
        node->as.whileStmt.body = $5;
        $$ = node;
    }
    ;
dowhileStmt:
    DO statement WHILE LEFT_PAREN expression RIGHT_PAREN SEMI_COLON
    {
        Node *node = makeNode(NODE_DO_WHILE_STMT, @$.first_line, @$.first_column);
        node->as.doWhileStmt.body = $2;
        node->as.doWhileStmt.condition = $5;
        $$ = node;
    }
    ;
breakStmt:
    BREAK SEMI_COLON
    {
        Node *node = makeNode(NODE_BREAK, @$.first_line, @$.first_column);
        $$ = node;
    }
    ;

continueStmt:
    CONTINUE SEMI_COLON
    {
        Node *node = makeNode(NODE_CONTINUE, @$.first_line, @$.first_column);
        $$ = node;
    }
    ;

block:
    LEFT_BRACE decList RIGHT_BRACE
    {
        Node *node = makeNode(NODE_BLOCK, @$.first_line, @$.first_column);
        node->as.block.declarations = $2;
        $$ = node;
    }
    |
    LEFT_BRACE RIGHT_BRACE
    {
        Node *node = makeNode(NODE_BLOCK, @$.first_line, @$.first_column);
        node->as.block.declarations = NULL;
        $$ = node;
    }
    ;

expression:
    assignment { $$ = $1; }
    ;

assignment:
    call_access EQ assignment { $$ = makeAssignNode(OP_EQ, $1, $3, @$.first_line, @$.first_column); }
    | call_access PLUS_EQ assignment { $$ = makeAssignNode(OP_PLUS_EQ, $1, $3, @$.first_line, @$.first_column); }
    | call_access MINUS_EQ assignment { $$ = makeAssignNode(OP_MINUS_EQ, $1, $3, @$.first_line, @$.first_column); }
    | call_access STAR_EQ assignment { $$ = makeAssignNode(OP_STAR_EQ, $1, $3, @$.first_line, @$.first_column); }
    | call_access SLASH_EQ assignment { $$ = makeAssignNode(OP_SLASH_EQ, $1, $3, @$.first_line, @$.first_column); }
    | call_access PERC_EQ assignment { $$ = makeAssignNode(OP_PERC_EQ, $1, $3, @$.first_line, @$.first_column); }
    | ternary { $$ = $1; }
    ;

ternary:
    logic_or QUESTION_MARK assignment COLON assignment
    {
        Node *node = makeNode(NODE_TERNARY_OP, @$.first_line, @$.first_column);
        node->as.ternary.condition = $1;
        node->as.ternary.ifBranch = $3;
        node->as.ternary.elseBranch = $5;
        $$ = node;
    }
    |
    logic_or { $$ = $1; }
    ;
logic_or:
    logic_or DOUBLE_PIPE logic_and { $$ = makeBinaryNode(OP_DOUBLE_PIPE, $1, $3, @$.first_line, @$.first_column); }
    | logic_and { $$ = $1; }
    ;

logic_and:
    logic_and DOUBLE_AND bitwise_or { $$ = makeBinaryNode(OP_DOUBLE_AND, $1, $3, @$.first_line, @$.first_column); }
    | bitwise_or { $$ = $1; }
    ;

bitwise_or:
    bitwise_or PIPE bitwise_xor { $$ = makeBinaryNode(OP_PIPE, $1, $3, @$.first_line, @$.first_column); }
    | bitwise_xor { $$ = $1; }
    ;

bitwise_xor:
    bitwise_xor XOR bitwise_and { $$ = makeBinaryNode(OP_XOR, $1, $3, @$.first_line, @$.first_column); }
    | bitwise_and { $$ = $1; }
    ;

bitwise_and:
    bitwise_and AND equality { $$ = makeBinaryNode(OP_AND, $1, $3, @$.first_line, @$.first_column); }
    | equality { $$ = $1; }
    ;

equality:
    equality BANG_EQ comparison { $$ = makeBinaryNode(OP_BANG_EQ, $1, $3, @$.first_line, @$.first_column); }
    | equality EQ_EQ comparison { $$ = makeBinaryNode(OP_EQ_EQ, $1, $3, @$.first_line, @$.first_column); }
    | comparison { $$ = $1; }
    ;

comparison:
    comparison GREATER shift { $$ = makeBinaryNode(OP_GREATER, $1, $3, @$.first_line, @$.first_column); }
    | comparison LESS shift { $$ = makeBinaryNode(OP_LESS, $1, $3, @$.first_line, @$.first_column); }
    | comparison GREATER_EQ shift { $$ = makeBinaryNode(OP_GREATER_EQ, $1, $3, @$.first_line, @$.first_column); }
    | comparison LESS_EQ shift { $$ = makeBinaryNode(OP_LESS_EQ, $1, $3, @$.first_line, @$.first_column); }
    | shift { $$ = $1; }
    ;

shift:
    shift SHIFT_LEFT term { $$ = makeBinaryNode(OP_SHIFT_LEFT, $1, $3, @$.first_line, @$.first_column); }
    | shift SHIFT_RIGHT term { $$ = makeBinaryNode(OP_SHIFT_RIGHT, $1, $3, @$.first_line, @$.first_column); }
    | term { $$ = $1; }
    ;

term:
    term PLUS factor { $$ = makeBinaryNode(OP_PLUS, $1, $3, @$.first_line, @$.first_column); }
    | term MINUS factor { $$ = makeBinaryNode(OP_MINUS, $1, $3, @$.first_line, @$.first_column); }
    | factor { $$ = $1; }
    ;

factor:
    factor SLASH unary { $$ = makeBinaryNode(OP_SLASH, $1, $3, @$.first_line, @$.first_column); }
    | factor STAR unary { $$ = makeBinaryNode(OP_STAR, $1, $3, @$.first_line, @$.first_column); }
    | factor PERC unary { $$ = makeBinaryNode(OP_PERC, $1, $3, @$.first_line, @$.first_column); }
    | unary { $$ = $1; }
    ;

unary:
    BANG unary { $$ = makeUnaryNode(OP_BANG, $2, @$.first_line, @$.first_column); }
    | TILDE unary { $$ = makeUnaryNode(OP_TILDE, $2, @$.first_line, @$.first_column); }
    | MINUS unary { $$ = makeUnaryNode(OP_MINUS, $2, @$.first_line, @$.first_column); }
    | PLUS_PLUS unary { $$ = makeUnaryNode(OP_PLUS_PLUS, $2, @$.first_line, @$.first_column); }
    | MINUS_MINUS unary { $$ = makeUnaryNode(OP_MINUS_MINUS, $2, @$.first_line, @$.first_column); }
    | call_access { $$ = $1; }
    ;

call_access:
    call_access LEFT_PAREN arguments RIGHT_PAREN
    {
        Node *node = makeNode(NODE_CALL, @$.first_line, @$.first_column);
        node->as.call.callee = $1;
        node->as.call.arguments = $3;
        $$ = node;
    }
    |
    call_access LEFT_PAREN RIGHT_PAREN
    {
        Node *node = makeNode(NODE_CALL, @$.first_line, @$.first_column);
        node->as.call.callee = $1;
        node->as.call.arguments = NULL;
        $$ = node;
    }
    |
    call_access DOT IDENTIFIER
    {
        Node *node = makeNode(NODE_MEMBER_ACCESS, @$.first_line, @$.first_column);
        node->as.memberAccess.object = $1;
        node->as.memberAccess.member = $3;
        $$ = node;
    }
    |
    call_access LEFT_SQUARE expression RIGHT_SQUARE
    {
        Node *node = makeNode(NODE_INDEX_ACCESS, @$.first_line, @$.first_column);
        node->as.indexAccess.object = $1;
        node->as.indexAccess.index = $3;
        $$ = node;
    }
    |
    call_access PLUS_PLUS
    {
        Node *node = makeNode(NODE_POSTFIX_OP, @$.first_line, @$.first_column);
        node->as.postfix.op = OP_PLUS_PLUS;
        node->as.postfix.expr = $1;
        $$ = node;
    }
    |
    call_access MINUS_MINUS
    {
        Node *node = makeNode(NODE_POSTFIX_OP, @$.first_line, @$.first_column);
        node->as.postfix.op = OP_MINUS_MINUS;
        node->as.postfix.expr = $1;
        $$ = node;
    }
    |
    primary { $$ = $1; }
    ;
primary:
    BOOL_CONSTANT
    {
        Node *node = makePrimaryNode(PRIMARY_BOOL, @$.first_line, @$.first_column);
        node->as.primary.val.boolval = $1;
        $$ = node;
    }
    |
    T_NULL { $$ = makePrimaryNode(PRIMARY_NULL, @$.first_line, @$.first_column); }
    |
    INT_CONSTANT
    {
        Node *node = makePrimaryNode(PRIMARY_INT, @$.first_line, @$.first_column);
        node->as.primary.val.intval = $1;
        $$ = node;
    }
    | FLOAT_CONSTANT
    {
        Node *node = makePrimaryNode(PRIMARY_FLOAT, @$.first_line, @$.first_column);
        node->as.primary.val.floatval = $1;
        $$ = node;
    }
    |
    STRING_CONSTANT
    {
        Node *node = makePrimaryNode(PRIMARY_STRING, @$.first_line, @$.first_column);
        node->as.primary.val.stringval = $1;
        $$ = node;
    }
    | IDENTIFIER
    {
        Node *node = makePrimaryNode(PRIMARY_IDENTIFIER, @$.first_line, @$.first_column);
        node->as.primary.val.stringval = $1;
        $$ = node;
    }
    |
    anonFunc { $$ = $1; }
    |
    LEFT_PAREN expression RIGHT_PAREN { $$ = $2; }
    | LEFT_SQUARE elements RIGHT_SQUARE
    {
        Node *node = makeNode(NODE_LIST_LITERAL, @$.first_line, @$.first_column);
        node->as.list.elements = $2;
        $$ = node;
    }
    |
    LEFT_SQUARE RIGHT_SQUARE
    {
        Node *node = makeNode(NODE_LIST_LITERAL, @$.first_line, @$.first_column);
        node->as.list.elements = NULL;
        $$ = node;
    }
    ;

anonFunc:
    LAMBDA function
    {
        Node *node = makeNode(NODE_LAMBDA, @$.first_line, @$.first_column);
        node->as.lambda.function = $2;
        $$ = node;
    }
    ;
arguments:
    arguments COMMA expression { $$ = appendList($1, $3); }
    | expression { $$ = appendList(NULL, $1); }
    ;

elements:
    elements COMMA expression { $$ = appendList($1, $3); }
    | expression { $$ = appendList(NULL, $1); }
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
        printf("Parsed OK\n");
        if (!should_dump_ir_only()) {
            printAst(rootAst, 0);
        }

        if (should_dump_ir()) {
            bool debug = should_debug();
            if (!analyze_ast(rootAst, debug)) {
                fprintf(stderr, "\nSemantic analysis failed. Compilation stopped.\n");
                return 1;
            }

            generateIr(rootAst);
            printf("IR\n");
            irDump(stdout);

            if (debug) {
                printf("\n--- Symbol Table (Debug) ---");
                print_symbol_table();

                printf("\n--- Type Registry (Debug) ---");
                print_type_registry();
            }
        }
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
            for (int i = 1; i < col; i++) {
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
    print_fancy_error(yylineno, yycolumn > 0 ? yycolumn : 1, message);
}