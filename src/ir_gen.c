#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

static Inst *head, *tail;
static int ir_error_count;

extern void print_fancy_error(int line, int col, const char *message);

static void appendInst(Inst *inst) {
    if (!inst) return;
    if (!head) { head = tail = inst; return; }
    tail->next = inst;
    tail = inst;
}

static void freeList(void) {
    for (Inst *it = head, *nx; it; it = nx) {
        nx = it->next;
        free(it->arg1); free(it->arg2); free(it->res); free(it);
    }
    head = tail = NULL;
}

Inst *irGetHead(void) { return head; }
bool irHasError(void) { return ir_error_count > 0; }

static int temp_n, label_n, lambda_n;

typedef struct {
    const char *continue_label;
    const char *break_label;
    const char *type_name;
    const char *function_name;
} GenCtx;


static char *dupString(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static char *symbolRefName(const Node *node, const char *fallback_name) {
    if (node && node->symbol_table_index != SYMBOL_INDEX_UNRESOLVED) {
        char buf[32];
        snprintf(buf, sizeof(buf), "_sym%d", node->symbol_table_index);
        return dupString(buf);
    }
    return dupString(fallback_name);
}

static char *newTemp(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "t%d", ++temp_n);
    return dupString(buf);
}

static char *newLabel(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "_L%d", label_n++);
    return dupString(buf);
}

static char *newLambdaName(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "_lambda%d", ++lambda_n);
    return dupString(buf);
}

static char *varTypeToString(const Node *typeNode) {
    char buf[64];
    switch (typeNode->as.varType.kind) {
    case VAR_TYPE_INT:    return dupString("int");
    case VAR_TYPE_FLOAT:  return dupString("float");
    case VAR_TYPE_STRING: return dupString("string");
    case VAR_TYPE_BOOL:   return dupString("bool");
    case VAR_TYPE_DYN:    return dupString("dyn");
    case VAR_TYPE_NULL:   return dupString("null");
    case VAR_TYPE_CUSTOM: return symbolRefName(typeNode, typeNode->as.varType.customTypeName);
    case VAR_TYPE_LIST:
        snprintf(buf, sizeof(buf), "list[%d]", typeNode->as.varType.listSize);
        return dupString(buf);
    default:
        return dupString("unknown");
    }
}

static int parameterCount(const Node *params) {
    int count = 0;
    for (const Node *p = params; p; p = p->next)
        if (p->type == NODE_TYPED_VAR) count++;
    return count;
}

static char *primaryToString(const Node *node) {
    const PrimaryNode *p = &node->as.primary;
    char buf[64];
    switch (p->type) {
    case PRIMARY_INT:        snprintf(buf, sizeof(buf), "%d", p->val.intval);   return dupString(buf);
    case PRIMARY_FLOAT:      snprintf(buf, sizeof(buf), "%g", p->val.floatval); return dupString(buf);
    case PRIMARY_STRING:     return dupString(p->val.stringval);
    case PRIMARY_BOOL:       return dupString(p->val.boolval ? "true" : "false");
    case PRIMARY_NULL:       return dupString("null");
    case PRIMARY_IDENTIFIER: return symbolRefName(node, p->val.stringval);
    default:                 return dupString("0");
    }
}

static OperatorType compoundToBinary(OperatorType op) {
    switch (op) {
    case OP_PLUS_EQ:  return OP_PLUS;
    case OP_MINUS_EQ: return OP_MINUS;
    case OP_STAR_EQ:  return OP_STAR;
    case OP_SLASH_EQ: return OP_SLASH;
    case OP_PERC_EQ:  return OP_PERC;
    default:          return OP_EQ;
    }
}

static void emitOp(OperatorType op, const char *a1, const char *a2, const char *r) {
    Inst *i = calloc(1, sizeof(Inst));
    if (!i) return;
    i->is_special = 0; i->as.op = op;
    i->arg1 = dupString(a1); i->arg2 = dupString(a2); i->res = dupString(r);
    appendInst(i);
}

static void emitIr(IrOp op, const char *a1, const char *a2, const char *r) {
    Inst *i = calloc(1, sizeof(Inst));
    if (!i) return;
    i->is_special = 1; i->as.special = op;
    i->arg1 = dupString(a1); i->arg2 = dupString(a2); i->res = dupString(r);
    appendInst(i);
}

static char *visitNode(const Node *node, GenCtx ctx);

static void reportExpectedLvalue(const Node *node) {
    int line = 1;
    int col = 1;
    if (node) {
        if (node->line > 0) line = node->line;
        if (node->pos > 0) col = node->pos;
    }
    ir_error_count++;
    print_fancy_error(line, col, "expected lvalue");
}

static void visitList(const Node *node, GenCtx ctx) {
    for (const Node *n = node; n; n = n->next)
        free(visitNode(n, ctx));
}

static char *loadLValue(const Node *lhs, GenCtx ctx) {
    if (!lhs) {
        reportExpectedLvalue(lhs);
        return NULL;
    }

    if (lhs->type == NODE_PRIMARY && lhs->as.primary.type == PRIMARY_IDENTIFIER)
        return symbolRefName(lhs, lhs->as.primary.val.stringval);

    if (lhs->symbol_table_index != SYMBOL_INDEX_UNRESOLVED)
        return symbolRefName(lhs, NULL);

    if (lhs->type == NODE_MEMBER_ACCESS) {
        char *tmp = newTemp();
        char *obj = visitNode(lhs->as.memberAccess.object, ctx);
        emitIr(IR_MEMBER_ACCESS, obj, lhs->as.memberAccess.member, tmp);
        free(obj);
        return tmp;
    }

    if (lhs->type == NODE_INDEX_ACCESS) {
        char *tmp = newTemp();
        char *obj = visitNode(lhs->as.indexAccess.object, ctx);
        char *idx = visitNode(lhs->as.indexAccess.index, ctx);
        emitIr(IR_INDEX_ACCESS, obj, idx, tmp);
        free(obj); free(idx);
        return tmp;
    }

    reportExpectedLvalue(lhs);
    return NULL;
}

static bool storeLValue(const Node *lhs, const char *val, GenCtx ctx) {
    if (!lhs) {
        reportExpectedLvalue(lhs);
        return false;
    }

    if (lhs->type == NODE_PRIMARY && lhs->as.primary.type == PRIMARY_IDENTIFIER) {
        char *target = symbolRefName(lhs, lhs->as.primary.val.stringval);
        emitOp(OP_EQ, val, NULL, target);
        free(target);
        return true;
    }

    if (lhs->symbol_table_index != SYMBOL_INDEX_UNRESOLVED) {
        char *target = symbolRefName(lhs, NULL);
        emitOp(OP_EQ, val, NULL, target);
        free(target);
        return true;
    }

    if (lhs->type == NODE_MEMBER_ACCESS) {
        char *obj = visitNode(lhs->as.memberAccess.object, ctx);
        emitIr(IR_STORE_MEMBER, obj, lhs->as.memberAccess.member, val);
        free(obj);
        return true;
    }

    if (lhs->type == NODE_INDEX_ACCESS) {
        char *obj = visitNode(lhs->as.indexAccess.object, ctx);
        char *idx = visitNode(lhs->as.indexAccess.index, ctx);
        emitIr(IR_STORE_INDEX, obj, idx, val);
        free(obj); free(idx);
        return true;
    }

    reportExpectedLvalue(lhs);
    return false;
}


static void visitFunctionBody(const Node *fn_node, const char *fn_name, GenCtx ctx) {
    ctx.function_name = fn_name;
    ctx.break_label = NULL;
    ctx.continue_label = NULL;

    int param_pos = 0;
    for (Node *p = fn_node->as.function.parameters; p; p = p->next) {
        if (p->type != NODE_TYPED_VAR) continue;
        char *param_type = varTypeToString(p->as.typedVar.varType);
        char *param_ref  = symbolRefName(p, p->as.typedVar.name);
        char arg2_buf[128];
        snprintf(arg2_buf, sizeof(arg2_buf), "%d:%s",
                 param_pos, param_ref ? param_ref : "_");
        emitIr(IR_FUNC_PARAM, fn_name, arg2_buf, param_type);
        free(param_ref); free(param_type);
        param_pos++;
    }
    visitList(fn_node->as.function.body, ctx);
}

static void visitFunDecl(const Node *node, GenCtx ctx) {
    const char *raw_name = node->as.funDecl.name;
    char *fn_name = symbolRefName(node, raw_name);

    const Node *fn_node = node->as.funDecl.function;
    char *ret_type  = varTypeToString(fn_node->as.function.returnType);
    char  argc_buf[32];
    snprintf(argc_buf, sizeof(argc_buf), "%d",
             parameterCount(fn_node->as.function.parameters));

    emitIr(IR_FUNC, fn_name, argc_buf, ret_type);
    visitFunctionBody(fn_node, fn_name, ctx);
    emitIr(IR_ENDFUNC, NULL, NULL, NULL);

    free(ret_type); free(fn_name);
}

static void visitTypeDecl(const Node *node, GenCtx ctx) {
    char *name = symbolRefName(node, node->as.typeDecl.name);
    emitIr(IR_TYPE_BEGIN, name, NULL, NULL);
    ctx.type_name = name;
    visitList(node->as.typeDecl.fields, ctx);
    emitIr(IR_TYPE_END, name, NULL, NULL);
    free(name);
}

static void visitVarDecl(const Node *node, GenCtx ctx) {
    if (ctx.type_name) {
        emitIr(IR_TYPE_FIELD, ctx.type_name, node->as.varDecl.name, NULL);
        return;
    }

    if (node->as.varDecl.initializer) {
        char *init   = visitNode(node->as.varDecl.initializer, ctx);
        char *target = symbolRefName(node, node->as.varDecl.name);
        emitOp(OP_EQ, init, NULL, target);
        free(target); free(init);
    }
}

static void visitForStmt(const Node *node, GenCtx ctx) {
    char *ls = newLabel(), *lc = newLabel(), *le = newLabel();

    free(visitNode(node->as.forStmt.init, ctx));
    emitIr(IR_LABEL, NULL, NULL, ls);
    if (node->as.forStmt.condition) {
        char *cond = visitNode(node->as.forStmt.condition, ctx);
        emitIr(IR_IF_FALSE, cond, le, NULL);
        free(cond);
    }

    GenCtx loop_ctx    = ctx;
    loop_ctx.continue_label = lc;
    loop_ctx.break_label    = le;
    free(visitNode(node->as.forStmt.body, loop_ctx));

    emitIr(IR_LABEL, NULL, NULL, lc);
    free(visitNode(node->as.forStmt.increment, ctx));
    emitIr(IR_GOTO, ls, NULL, NULL);
    emitIr(IR_LABEL, NULL, NULL, le);

    free(ls); free(lc); free(le);
}

static void visitWhileStmt(const Node *node, GenCtx ctx) {
    char *ls = newLabel(), *le = newLabel();

    emitIr(IR_LABEL, NULL, NULL, ls);
    char *cond = visitNode(node->as.whileStmt.condition, ctx);
    emitIr(IR_IF_FALSE, cond, le, NULL);
    free(cond);

    GenCtx loop_ctx    = ctx;
    loop_ctx.continue_label = ls;
    loop_ctx.break_label    = le;
    free(visitNode(node->as.whileStmt.body, loop_ctx));

    emitIr(IR_GOTO, ls, NULL, NULL);
    emitIr(IR_LABEL, NULL, NULL, le);
    free(ls); free(le);
}

static void visitDoWhileStmt(const Node *node, GenCtx ctx) {
    char *ls = newLabel(), *lc = newLabel(), *le = newLabel();

    emitIr(IR_LABEL, NULL, NULL, ls);

    GenCtx loop_ctx    = ctx;
    loop_ctx.continue_label = lc;
    loop_ctx.break_label    = le;
    free(visitNode(node->as.doWhileStmt.body, loop_ctx));

    emitIr(IR_LABEL, NULL, NULL, lc);
    char *cond = visitNode(node->as.doWhileStmt.condition, ctx);
    emitIr(IR_IF_FALSE, cond, le, NULL);
    free(cond);
    emitIr(IR_GOTO, ls, NULL, NULL);
    emitIr(IR_LABEL, NULL, NULL, le);

    free(ls); free(lc); free(le);
}

static void visitIfStmt(const Node *node, GenCtx ctx) {
    char *le   = newLabel();
    char *cond = visitNode(node->as.ifStmt.condition, ctx);
    emitIr(IR_IF_FALSE, cond, le, NULL);
    free(cond);

    free(visitNode(node->as.ifStmt.thenBranch, ctx));

    if (node->as.ifStmt.elseBranch) {
        char *lend = newLabel();
        emitIr(IR_GOTO, lend, NULL, NULL);
        emitIr(IR_LABEL, NULL, NULL, le);
        free(visitNode(node->as.ifStmt.elseBranch, ctx));
        emitIr(IR_LABEL, NULL, NULL, lend);
        free(lend);
    } else {
        emitIr(IR_LABEL, NULL, NULL, le);
    }
    free(le);
}

static char *visitUnaryOp(const Node *node, GenCtx ctx) {
    if (node->as.unary.op == OP_PLUS_PLUS || node->as.unary.op == OP_MINUS_MINUS) {
        char *lv  = loadLValue(node->as.unary.expr, ctx);
        if (!lv) {
            return NULL;
        }
        char *upd = newTemp();
        emitOp(node->as.unary.op == OP_PLUS_PLUS ? OP_PLUS : OP_MINUS,
               lv, "1", upd);
        if (!storeLValue(node->as.unary.expr, upd, ctx)) {
            free(lv);
            free(upd);
            return NULL;
        }
        free(lv);
        return upd;
    }
    char *tmp  = newTemp();
    char *expr = visitNode(node->as.unary.expr, ctx);
    emitOp(node->as.unary.op, expr, NULL, tmp);
    free(expr);
    return tmp;
}

static char *visitBinaryOp(const Node *node, GenCtx ctx) {
    OperatorType op = node->as.binary.op;

    if (op == OP_DOUBLE_AND) {
        char *l = visitNode(node->as.binary.left, ctx);
        char *lf = newLabel(), *lend = newLabel(), *tmp = newTemp();

        emitIr(IR_IF_FALSE, l, lf, NULL);
        free(l);

        char *r = visitNode(node->as.binary.right, ctx);
        emitOp(OP_EQ, r, NULL, tmp);
        free(r);
        emitIr(IR_GOTO, lend, NULL, NULL);

        emitIr(IR_LABEL, NULL, NULL, lf);
        emitOp(OP_EQ, "false", NULL, tmp);
        emitIr(IR_LABEL, NULL, NULL, lend);

        free(lf); free(lend);
        return tmp;
    }

    if (op == OP_DOUBLE_PIPE) {
        char *l = visitNode(node->as.binary.left, ctx);
        char *lf = newLabel(), *lend = newLabel(), *tmp = newTemp();

        emitIr(IR_IF_FALSE, l, lf, NULL);
        free(l);

        emitOp(OP_EQ, "true", NULL, tmp);
        emitIr(IR_GOTO, lend, NULL, NULL);

        emitIr(IR_LABEL, NULL, NULL, lf);
        char *r = visitNode(node->as.binary.right, ctx);
        emitOp(OP_EQ, r, NULL, tmp);
        free(r);

        emitIr(IR_LABEL, NULL, NULL, lend);

        free(lf); free(lend);
        return tmp;
    }

    char *tmp = newTemp();
    char *l   = visitNode(node->as.binary.left, ctx);
    char *r   = visitNode(node->as.binary.right, ctx);
    emitOp(op, l, r, tmp);
    free(l); free(r);
    return tmp;
}

static char *visitTernaryOp(const Node *node, GenCtx ctx) {
    char *lf   = newLabel(), *lend = newLabel(), *tmp = newTemp();
    char *cond = visitNode(node->as.ternary.condition, ctx);
    emitIr(IR_IF_FALSE, cond, lf, NULL);
    free(cond);

    char *if_val = visitNode(node->as.ternary.ifBranch, ctx);
    emitOp(OP_EQ, if_val, NULL, tmp);
    free(if_val);
    emitIr(IR_GOTO, lend, NULL, NULL);
    emitIr(IR_LABEL, NULL, NULL, lf);

    char *else_val = visitNode(node->as.ternary.elseBranch, ctx);
    emitOp(OP_EQ, else_val, NULL, tmp);
    free(else_val);
    emitIr(IR_LABEL, NULL, NULL, lend);

    free(lf); free(lend);
    return tmp;
}

static char *visitAssignOp(const Node *node, GenCtx ctx) {
    char *rhs = visitNode(node->as.assign.right, ctx);
    if (!rhs) return NULL;

    if (node->as.assign.op == OP_EQ) {
        if (!storeLValue(node->as.assign.left, rhs, ctx)) {
            free(rhs);
            return NULL;
        }
        return rhs;
    }
    char *lv  = loadLValue(node->as.assign.left, ctx);
    if (!lv) {
        free(rhs);
        return NULL;
    }
    char *tmp = newTemp();
    emitOp(compoundToBinary(node->as.assign.op), lv, rhs, tmp);
    if (!storeLValue(node->as.assign.left, tmp, ctx)) {
        free(lv);
        free(rhs);
        free(tmp);
        return NULL;
    }
    free(lv); free(rhs);
    return tmp;
}

static char *visitCall(const Node *node, GenCtx ctx) {
    char *callee = symbolRefName(node, NULL);
    if (!callee)
        callee = visitNode(node->as.call.callee, ctx);
    int   argc   = 0;
    for (Node *a = node->as.call.arguments; a; a = a->next) {
        char *arg = visitNode(a, ctx);
        emitIr(IR_PARAM, arg, NULL, NULL);
        free(arg);
        argc++;
    }
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", argc);
    char *tmp = newTemp();
    emitIr(IR_CALL, callee, buf, tmp);
    free(callee);
    return tmp;
}

static char *visitPostfixOp(const Node *node, GenCtx ctx) {
    char *old = newTemp(), *upd = newTemp();
    char *lv  = loadLValue(node->as.postfix.expr, ctx);
    if (!lv) {
        free(old);
        free(upd);
        return NULL;
    }
    emitOp(OP_EQ, lv, NULL, old);
    emitOp(node->as.postfix.op == OP_PLUS_PLUS ? OP_PLUS : OP_MINUS,
           lv, "1", upd);
    if (!storeLValue(node->as.postfix.expr, upd, ctx)) {
        free(lv);
        free(old);
        free(upd);
        return NULL;
    }
    free(lv); free(upd);
    return old;
}

static char *visitListLiteral(const Node *node, GenCtx ctx) {
    char *tmp   = newTemp();
    int   count = 0;
    for (Node *e = node->as.list.elements; e; e = e->next) {
        char *elem = visitNode(e, ctx);
        emitIr(IR_LIST_ELEM, elem, NULL, NULL);
        free(elem);
        count++;
    }
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", count);
    emitIr(IR_LIST_LITERAL, buf, NULL, tmp);
    return tmp;
}

static char *visitLambda(const Node *node, GenCtx ctx) {
    char       *fn      = newLambdaName();
    const Node *fn_node = node->as.lambda.function;
    char       *ret_type = varTypeToString(fn_node->as.function.returnType);
    char        argc_buf[32];
    snprintf(argc_buf, sizeof(argc_buf), "%d",
             parameterCount(fn_node->as.function.parameters));

    emitIr(IR_FUNC, fn, argc_buf, ret_type);
    visitFunctionBody(fn_node, fn, ctx);
    emitIr(IR_ENDFUNC, NULL, NULL, NULL);
    free(ret_type);

    char *tmp = newTemp();
    emitOp(OP_EQ, fn, NULL, tmp);
    free(fn);
    return tmp;
}

static char *visitNode(const Node *node, GenCtx ctx) {
    if (!node) return NULL;

    switch (node->type) {

    case NODE_PROGRAM:
        visitList(node->as.program.declarations, ctx);
        break;

    case NODE_TYPE_DECL:
        visitTypeDecl(node, ctx);
        break;

    case NODE_FUN_DECL:
        visitFunDecl(node, ctx);
        break;

    case NODE_FUNCTION:
        visitFunctionBody(node, ctx.function_name, ctx);
        break;

    case NODE_TYPED_VAR:
    case NODE_VAR_TYPE:
        break;

    case NODE_VAR_DECL:
        visitVarDecl(node, ctx);
        break;

    case NODE_EXPR_STMT:
        free(visitNode(node->as.exprStmt.expr, ctx));
        break;

    case NODE_FOR_STMT:
        visitForStmt(node, ctx);
        break;

    case NODE_WHILE_STMT:
        visitWhileStmt(node, ctx);
        break;

    case NODE_DO_WHILE_STMT:
        visitDoWhileStmt(node, ctx);
        break;

    case NODE_IF_STMT:
        visitIfStmt(node, ctx);
        break;

    case NODE_ECHO_STMT: {
        char *expr = visitNode(node->as.echoStmt.expr, ctx);
        emitIr(IR_ECHO, expr, NULL, NULL);
        free(expr);
        break;
    }

    case NODE_BLOCK:
        visitList(node->as.block.declarations, ctx);
        break;

    case NODE_CONTINUE:
        if (ctx.continue_label) emitIr(IR_GOTO, ctx.continue_label, NULL, NULL);
        break;

    case NODE_BREAK:
        if (ctx.break_label) emitIr(IR_GOTO, ctx.break_label, NULL, NULL);
        break;

    case NODE_RETURN: {
        char *value = visitNode(node->as.returnStmt.expr, ctx);
        emitIr(IR_RETURN, value, NULL, NULL);
        free(value);
        break;
    }

    case NODE_PRIMARY:
        return primaryToString(node);

    case NODE_UNARY_OP:
        return visitUnaryOp(node, ctx);

    case NODE_BINARY_OP:
        return visitBinaryOp(node, ctx);

    case NODE_TERNARY_OP:
        return visitTernaryOp(node, ctx);

    case NODE_ASSIGN_OP:
        return visitAssignOp(node, ctx);

    case NODE_CALL:
        return visitCall(node, ctx);

    case NODE_MEMBER_ACCESS: {
        char *tmp = newTemp();
        char *obj = visitNode(node->as.memberAccess.object, ctx);
        char *member = dupString(node->as.memberAccess.member);
        emitIr(IR_MEMBER_ACCESS, obj, member, tmp);
        free(obj);
        free(member);
        return tmp;
    }

    case NODE_INDEX_ACCESS: {
        char *tmp = newTemp();
        char *obj = visitNode(node->as.indexAccess.object, ctx);
        char *idx = visitNode(node->as.indexAccess.index, ctx);
        emitIr(IR_INDEX_ACCESS, obj, idx, tmp);
        free(obj); free(idx);
        return tmp;
    }

    case NODE_POSTFIX_OP:
        return visitPostfixOp(node, ctx);

    case NODE_LIST_LITERAL:
        return visitListLiteral(node, ctx);

    case NODE_LAMBDA:
        return visitLambda(node, ctx);
    }

    return NULL;
}

void generateIr(const Node *ast_root) {
    freeList();
    ir_error_count = 0;
    temp_n = 1; label_n = 0; lambda_n = 0;

    GenCtx ctx = { NULL, NULL, NULL, NULL };
    visitList(ast_root, ctx);
}