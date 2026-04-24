#include <stdio.h>
#include "defs.h"

static const char *opName(OperatorType op) {
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
    case OP_PLUS_EQ: return "+=";
    case OP_MINUS_EQ: return "-=";
    case OP_STAR_EQ: return "*=";
    case OP_SLASH_EQ: return "/=";
    case OP_PERC_EQ: return "%=";
    case OP_PLUS_PLUS: return "++";
    case OP_MINUS_MINUS: return "--";
    case OP_BANG: return "!";
    default: return "?";
    }
}

static const char *irName(IrOp op) {
    switch (op) {
    case IR_LABEL: return "LABEL";
    case IR_GOTO: return "GOTO";
    case IR_IF_FALSE: return "IF_FALSE";
    case IR_FUNC: return "FUNC";
    case IR_ENDFUNC: return "ENDFUNC";
    case IR_FUNC_PARAM: return "FUNC_PARAM";
    case IR_ECHO: return "ECHO";
    case IR_RETURN: return "RETURN";
    case IR_PARAM: return "PARAM";
    case IR_CALL: return "CALL";
    case IR_MEMBER_ACCESS: return "MEMBER_ACCESS";
    case IR_INDEX_ACCESS: return "INDEX_ACCESS";
    case IR_STORE_MEMBER: return "STORE_MEMBER";
    case IR_STORE_INDEX: return "STORE_INDEX";
    case IR_LIST_ELEM: return "LIST_ELEM";
    case IR_LIST_LITERAL: return "LIST_LITERAL";
    case IR_TYPE_BEGIN: return "TYPE_BEGIN";
    case IR_TYPE_FIELD: return "TYPE_FIELD";
    case IR_TYPE_END: return "TYPE_END";
    default: return "IR?";
    }
}

void irDump(FILE *out) {
    FILE *fp = out;
    int idx = 0;

fprintf(fp, "+------+----------------+----------------------------------+----------------------------------+----------------------------------+\n");
fprintf(fp, "| idx  | opcode         | arg1                             | arg2                             | res                              |\n");
fprintf(fp, "+------+----------------+----------------------------------+----------------------------------+----------------------------------+\n");

for (Inst *it = irGetHead(); it; it = it->next, ++idx) {
    const char *name = it->is_special ? irName(it->as.special) : opName(it->as.op);
    fprintf(fp, "| %04d | %-14s | %-32s | %-32s | %-32s |\n",
            idx,
            name,
            it->arg1 ? it->arg1 : "_",
            it->arg2 ? it->arg2 : "_",
            it->res ? it->res : "_");
}

fprintf(fp, "+------+----------------+----------------------------------+----------------------------------+----------------------------------+\n");
}
