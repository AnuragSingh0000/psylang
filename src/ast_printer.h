#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

const char *nodeTypeName(NodeType t)
{
    switch (t)
    {
        case NODE_PROGRAM: return "PROGRAM";
        case NODE_TYPE_DECL: return "TYPE_DECL";
        case NODE_FUN_DECL: return "FUN_DECL";
        case NODE_FUNCTION: return "FUNCTION";
        case NODE_TYPED_VAR: return "TYPED_VAR";
        case NODE_VAR_TYPE: return "VAR_TYPE";
        case NODE_VAR_DECL: return "VAR_DECL";
        case NODE_EXPR_STMT: return "EXPR_STMT";
        case NODE_FOR_STMT: return "FOR";
        case NODE_IF_STMT: return "IF";
        case NODE_ECHO_STMT: return "ECHO";
        case NODE_PRIMARY: return "PRIMARY";
        case NODE_UNARY_OP: return "UNARY";
        case NODE_BINARY_OP: return "BINARY";
        case NODE_TERNARY_OP: return "TERNARY";
        case NODE_ASSIGN_OP: return "ASSIGN";
        case NODE_WHILE_STMT: return "WHILE";
        case NODE_DO_WHILE_STMT: return "DO_WHILE";
        case NODE_BLOCK: return "BLOCK";
        case NODE_CONTINUE: return "CONTINUE";
        case NODE_BREAK: return "BREAK";
        case NODE_RETURN: return "RETURN";
        case NODE_CALL: return "CALL";
        case NODE_MEMBER_ACCESS: return "MEMBER";
        case NODE_INDEX_ACCESS: return "INDEX";
        case NODE_POSTFIX_OP: return "POSTFIX";
        case NODE_LIST_LITERAL: return "LIST";
        case NODE_LAMBDA: return "LAMBDA";
        default: return "UNKNOWN";
    }
}

const char *operatorString(OperatorType op)
{
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_STAR: return "*";
        case OP_SLASH: return "/";
        case OP_PERC: return "%";
        case OP_LESS: return "<";
        case OP_GREATER: return ">";
        case OP_LESS_EQ: return "<=";
        case OP_GREATER_EQ: return ">=";
        case OP_BANG_EQ: return "!=";
        case OP_EQ_EQ: return "==";
        case OP_AND: return "&";
        case OP_DOUBLE_AND: return "&&";
        case OP_PIPE: return "|";
        case OP_DOUBLE_PIPE: return "||";
        case OP_TILDE: return "~";
        case OP_XOR: return "^";
        case OP_SHIFT_LEFT: return "<<";
        case OP_SHIFT_RIGHT: return ">>";
        case OP_EQ: return "=";
        case OP_PLUS_EQ: return "+";
        case OP_MINUS_EQ: return "-=";
        case OP_STAR_EQ: return "*=";
        case OP_SLASH_EQ: return "/=";
        case OP_PERC_EQ: return "%=";
        case OP_PLUS_PLUS: return "++";
        case OP_MINUS_MINUS: return "--";
        case OP_BANG: return "!";
        default: return "UNKNOWN_OP";
    }
}

void printAst(Node *n, int depth)
{
    if (!n) {
        return;
    }

    for (int i = 0; i < depth; i++)
        printf("  ");

    printf("%s", nodeTypeName(n->type));

    switch (n->type)
    {
        case NODE_PRIMARY:
            switch(n->as.primary.type) {
                case PRIMARY_INT: printf(" (%d)", n->as.primary.val.intval); break;
                case PRIMARY_FLOAT: printf(" (%f)", n->as.primary.val.floatval); break;
                case PRIMARY_STRING: printf(" (\"%s\")", n->as.primary.val.stringval); break;
                case PRIMARY_IDENTIFIER: printf(" (%s)", n->as.primary.val.stringval); break;
                case PRIMARY_BOOL: printf(" (%s)", n->as.primary.val.boolval ? "true" : "false"); break;
                case PRIMARY_NULL: printf(" (null)"); break;
            }
            break;
        case NODE_UNARY_OP:
            printf(" (%s)", operatorString(n->as.unary.op));
            break;
        case NODE_BINARY_OP:
            printf(" (%s)", operatorString(n->as.binary.op));
            break;
        case NODE_ASSIGN_OP:
            printf(" (%s)", operatorString(n->as.assign.op));
            break;
        case NODE_POSTFIX_OP:
            printf(" (%s)", operatorString(n->as.postfix.op));
            break;
        case NODE_VAR_DECL:
            printf(" (%s)", n->as.varDecl.name);
            break;
        case NODE_TYPED_VAR:
            printf(" (%s)", n->as.typedVar.name);
            break;
        case NODE_TYPE_DECL:
            printf(" (%s)", n->as.typeDecl.name);
            break;
        case NODE_FUN_DECL:
            printf(" (%s)", n->as.funDecl.name);
            break;
        case NODE_MEMBER_ACCESS:
            printf(" (.%s)", n->as.memberAccess.member);
            break;
        case NODE_VAR_TYPE:
            switch(n->as.varType.kind) {
                case VAR_TYPE_INT: printf(" (int)"); break;
                case VAR_TYPE_FLOAT: printf(" (float)"); break;
                case VAR_TYPE_STRING: printf(" (string)"); break;
                case VAR_TYPE_BOOL: printf(" (bool)"); break;
                case VAR_TYPE_DYN: printf(" (dyn)"); break;
                case VAR_TYPE_LIST: printf(" (list[%d])", n->as.varType.listSize); break;
                case VAR_TYPE_NULL: printf(" (null)"); break;
                case VAR_TYPE_CUSTOM: printf(" (%s)", n->as.varType.customTypeName); break;
            }
            break;
        default:
            break;
    }

    printf("\n");

    switch (n->type)
    {
        case NODE_PROGRAM:
            printAst(n->as.program.declarations, depth + 1);
            break;

        case NODE_TYPE_DECL:
            printAst(n->as.typeDecl.fields, depth + 1);
            break;

        case NODE_FUN_DECL:
            printAst(n->as.funDecl.function, depth + 1);
            break;

        case NODE_FUNCTION:
            printAst(n->as.function.parameters, depth + 1);
            printAst(n->as.function.returnType, depth + 1);
            printAst(n->as.function.body, depth + 1);
            break;

        case NODE_TYPED_VAR:
            printAst(n->as.typedVar.varType, depth + 1);
            break;

        case NODE_VAR_TYPE:
            // var type is a leaf node, so no need to print children
            break;

        case NODE_VAR_DECL:
            printAst(n->as.varDecl.varType, depth + 1);
            printAst(n->as.varDecl.initializer, depth + 1);
            break;

        case NODE_EXPR_STMT:
            printAst(n->as.exprStmt.expr, depth + 1);
            break;

        case NODE_FOR_STMT:
            printAst(n->as.forStmt.init, depth + 1);
            printAst(n->as.forStmt.condition, depth + 1);
            printAst(n->as.forStmt.increment, depth + 1);
            printAst(n->as.forStmt.body, depth + 1);
            break;

        case NODE_IF_STMT:
            printAst(n->as.ifStmt.condition, depth + 1);
            printAst(n->as.ifStmt.thenBranch, depth + 1);
            printAst(n->as.ifStmt.elseBranch, depth + 1);
            break;

        case NODE_ECHO_STMT:
            printAst(n->as.echoStmt.expr, depth + 1);
            break;

        case NODE_PRIMARY:
            // primary is a leaf node, so no need to print children
            break;

        case NODE_WHILE_STMT:
            printAst(n->as.whileStmt.condition, depth + 1);
            printAst(n->as.whileStmt.body, depth + 1);
            break;

        case NODE_DO_WHILE_STMT:
            printAst(n->as.doWhileStmt.body, depth + 1);
            printAst(n->as.doWhileStmt.condition, depth + 1);
            break;

        case NODE_BLOCK:
            printAst(n->as.block.declarations, depth + 1);
            break;

        case NODE_CONTINUE:
            break;

        case NODE_BREAK:
            break;

        case NODE_RETURN:
            printAst(n->as.returnStmt.expr, depth + 1);
            break;

        case NODE_UNARY_OP:
            printAst(n->as.unary.expr, depth + 1);
            break;

        case NODE_BINARY_OP:
            printAst(n->as.binary.left, depth + 1);
            printAst(n->as.binary.right, depth + 1);
            break;

        case NODE_ASSIGN_OP:
            printAst(n->as.assign.left, depth + 1);
            printAst(n->as.assign.right, depth + 1);
            break;

        case NODE_TERNARY_OP:
            printAst(n->as.ternary.condition, depth + 1);
            printAst(n->as.ternary.ifBranch, depth + 1);
            printAst(n->as.ternary.elseBranch, depth + 1);
            break;

        case NODE_CALL:
            printAst(n->as.call.callee, depth + 1);
            printAst(n->as.call.arguments, depth + 1);
            break;

        case NODE_MEMBER_ACCESS:
            printAst(n->as.memberAccess.object, depth + 1);
            break;

        case NODE_INDEX_ACCESS:
            printAst(n->as.indexAccess.object, depth + 1);
            printAst(n->as.indexAccess.index, depth + 1);
            break;

        case NODE_POSTFIX_OP:
            printAst(n->as.postfix.expr, depth + 1);
            break;

        case NODE_LIST_LITERAL:
            printAst(n->as.list.elements, depth + 1);
            break;

        case NODE_LAMBDA:
            printAst(n->as.lambda.function, depth + 1);
            break;

        default:
            break;
    }
    // traverse list
    printAst(n->next, depth);
}