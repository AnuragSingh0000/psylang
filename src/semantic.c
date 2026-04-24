#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "semantic.h"

#define MAX_SYMBOLS 2048

typedef struct {
    VarTypeKind kind;
    char *custom_name;
} TypeInfo;

static TypeInfo make_type(VarTypeKind kind, char *name) {
    TypeInfo t;
    t.kind = kind;
    t.custom_name = name ? strdup(name) : NULL;
    return t;
}

static bool types_equal(TypeInfo t1, TypeInfo t2) {
    if (t1.kind != t2.kind) return false;
    if (t1.kind == VAR_TYPE_CUSTOM) {
        if (!t1.custom_name || !t2.custom_name) return false;
        return strcmp(t1.custom_name, t2.custom_name) == 0;
    }
    return true;
}

static bool is_valid_assignment(TypeInfo left, TypeInfo right) {
    if (left.kind == VAR_TYPE_DYN || right.kind == VAR_TYPE_DYN) return true;
    if (right.kind == VAR_TYPE_NULL) {
        return left.kind == VAR_TYPE_STRING || left.kind == VAR_TYPE_LIST ||
               left.kind == VAR_TYPE_CUSTOM || left.kind == VAR_TYPE_NULL;
    }
    if (types_equal(left, right)) return true;
    if (left.kind == VAR_TYPE_FLOAT && right.kind == VAR_TYPE_INT) return true;
    return false;
}

typedef struct {
    char *name;
    int depth;
    int sym_index;
    TypeInfo type;
    bool active;

    bool is_type_def;
    Node *type_fields;
} Symbol;

static Symbol sym_table[MAX_SYMBOLS];
static int sym_count = 0;
static int current_depth = 0;
static int next_sym_index = 1;

static int loop_depth = 0;
static int function_depth = 0;
static TypeInfo current_return_type;

static bool has_errors = false;
static bool is_debug_mode = false;
extern void print_fancy_error(int line, int col, const char *message);

static void sem_error(int line, int col, const char *msg) {
    print_fancy_error(line, col > 0 ? col : 1, msg);
    has_errors = true;
}

static void begin_scope() {
    current_depth++;
}

static void end_scope() {
    if (is_debug_mode) {
        for (int i = sym_count - 1; i >= 0; i--) {
            if (sym_table[i].depth == current_depth && sym_table[i].active) {
                sym_table[i].active = false;
            } else if (sym_table[i].depth < current_depth) {
                break;
            }
        }
    } else {
        while (sym_count > 0 && sym_table[sym_count - 1].depth == current_depth) {
            sym_count--;
        }
    }
    current_depth--;
}

static int declare_symbol(const char *name, TypeInfo type, bool is_type_def, Node *type_fields, int line, int col) {
    for (int i = sym_count - 1; i >= 0; i--) {
        if (sym_table[i].depth < current_depth) break;
        if (sym_table[i].active && strcmp(sym_table[i].name, name) == 0) {
            char err[256];
            snprintf(err, sizeof(err), "Redeclaration of '%s' in the same scope.", name);
            sem_error(line, col, err);
            return -1;
        }
    }

    int idx = next_sym_index++;
    sym_table[sym_count].name = strdup(name);
    sym_table[sym_count].depth = current_depth;
    sym_table[sym_count].sym_index = idx;
    sym_table[sym_count].type = type;
    sym_table[sym_count].active = true;
    sym_table[sym_count].is_type_def = is_type_def;
    sym_table[sym_count].type_fields = type_fields;
    sym_count++;

    return idx;
}

static Symbol* find_symbol_quiet(const char *name) {
    for (int i = sym_count - 1; i >= 0; i--) {
        if (sym_table[i].active && strcmp(sym_table[i].name, name) == 0) {
            return &sym_table[i];
        }
    }
    return NULL;
}

static Symbol* resolve_symbol(const char *name, int line, int col) {
    Symbol *sym = find_symbol_quiet(name);
    if (sym) return sym;

    char err[256];
    snprintf(err, sizeof(err), "Undeclared variable, function, or type '%s'.", name);
    sem_error(line, col, err);
    return NULL;
}

static bool type_exists(const char *name) {
    Symbol *sym = find_symbol_quiet(name);
    return sym != NULL && sym->is_type_def;
}

static TypeInfo get_member_type(const char *type_name, const char *member_name, int line, int col) {
    Symbol *sym = find_symbol_quiet(type_name);
    if (sym && sym->is_type_def) {
        for (Node *f = sym->type_fields; f; f = f->next) {
            if (f->type == NODE_VAR_DECL && strcmp(f->as.varDecl.name, member_name) == 0) {
                return make_type(f->as.varDecl.varType->as.varType.kind,
                                 f->as.varDecl.varType->as.varType.customTypeName);
            }
        }
        char err[256];
        snprintf(err, sizeof(err), "Member '%s' does not exist in type '%s'.", member_name, type_name);
        sem_error(line, col, err);
        return make_type(VAR_TYPE_DYN, NULL);
    }

    char err[256];
    snprintf(err, sizeof(err), "Unknown type '%s'.", type_name);
    sem_error(line, col, err);
    return make_type(VAR_TYPE_DYN, NULL);
}

static const char* get_type_string(VarTypeKind kind, const char* custom_name) {
    switch (kind) {
        case VAR_TYPE_INT: return "int";
        case VAR_TYPE_FLOAT: return "float";
        case VAR_TYPE_STRING: return "string";
        case VAR_TYPE_BOOL: return "bool";
        case VAR_TYPE_DYN: return "dyn";
        case VAR_TYPE_LIST: return "list";
        case VAR_TYPE_CUSTOM: return custom_name ? custom_name : "custom";
        case VAR_TYPE_NULL: return "null";
        default: return "unknown";
    }
}

void print_type_registry(void) {
    printf("\n+------------------------------+----------------------------------------------------+\n");
    printf("| Index | Type Name            | Members (Name : Type)                              |\n");
    printf("+------------------------------+----------------------------------------------------+\n");

    for (int i = 0; i < sym_count; i++) {
        if (sym_table[i].is_type_def) {
            printf("| _sym%-2d| %-20.20s | ", sym_table[i].sym_index, sym_table[i].name);

            bool first = true;
            for (Node *f = sym_table[i].type_fields; f; f = f->next) {
                if (!first) printf(", ");
                if (f->type == NODE_VAR_DECL) {
                    const char *t_str = get_type_string(f->as.varDecl.varType->as.varType.kind,
                                                        f->as.varDecl.varType->as.varType.customTypeName);
                    printf("%s: %s", f->as.varDecl.name, t_str);
                }
                first = false;
            }
            if (first) printf("(empty)");
            printf("\n");
        }
    }
    printf("+------------------------------+----------------------------------------------------+\n");
}

void print_symbol_table(void) {
    printf("\n+-------+----------------------------------+-------+--------------------------+\n");
    printf("| Index | Name                             | Depth | Type                     |\n");
    printf("+-------+----------------------------------+-------+--------------------------+\n");

    for (int i = 0; i < sym_count; i++) {
        const char *type_str = sym_table[i].is_type_def ? "type" : get_type_string(sym_table[i].type.kind, sym_table[i].type.custom_name);

        printf("| _sym%-2d| %-32s | %-5d | %-24s |\n",
               sym_table[i].sym_index,
               sym_table[i].name,
               sym_table[i].depth,
               type_str);
    }
    printf("+-------+----------------------------------+-------+--------------------------+\n");
}

static bool is_lvalue(Node *node) {
    if (!node) return false;
    if (node->type == NODE_PRIMARY && node->as.primary.type == PRIMARY_IDENTIFIER) return true;
    if (node->type == NODE_INDEX_ACCESS) return true;
    if (node->type == NODE_MEMBER_ACCESS) return true;
    return false;
}

static bool check_returns(Node *node);
static bool check_returns_list(Node *node) {
    for (Node *n = node; n; n = n->next) {
        if (check_returns(n)) return true;
    }
    return false;
}

static bool check_returns(Node *node) {
    if (!node) return false;
    switch (node->type) {
        case NODE_RETURN: return true;
        case NODE_BLOCK: return check_returns_list(node->as.block.declarations);
        case NODE_IF_STMT:
            if (node->as.ifStmt.elseBranch) {
                return check_returns(node->as.ifStmt.thenBranch) && check_returns(node->as.ifStmt.elseBranch);
            }
            return false;
        default: return false;
    }
}

static TypeInfo analyze_node(Node *node);

static void analyze_list(Node *node) {
    for (Node *n = node; n; n = n->next) analyze_node(n);
}

static TypeInfo analyze_node(Node *node) {
    if (!node) return make_type(VAR_TYPE_NULL, NULL);

    switch (node->type) {
        case NODE_PROGRAM:
            analyze_list(node->as.program.declarations);
            return make_type(VAR_TYPE_NULL, NULL);

        case NODE_BLOCK:
            begin_scope();
            analyze_list(node->as.block.declarations);
            end_scope();
            return make_type(VAR_TYPE_NULL, NULL);

        case NODE_TYPE_DECL: {
            TypeInfo dummy_type = make_type(VAR_TYPE_NULL, NULL);
            declare_symbol(node->as.typeDecl.name, dummy_type, true, node->as.typeDecl.fields, node->line, node->pos);

            for (Node *f = node->as.typeDecl.fields; f; f = f->next) {
                if (f->type == NODE_VAR_DECL) {
                    const char *fname = f->as.varDecl.name;

                    for (Node *chk = node->as.typeDecl.fields; chk != f; chk = chk->next) {
                        if (chk->type == NODE_VAR_DECL && strcmp(fname, chk->as.varDecl.name) == 0) {
                            char err[256];
                            snprintf(err, sizeof(err), "Duplicate member '%s' in type '%s'.", fname, node->as.typeDecl.name);
                            sem_error(f->line, f->pos, err);
                        }
                    }

                    TypeInfo field_type = make_type(f->as.varDecl.varType->as.varType.kind,
                                                    f->as.varDecl.varType->as.varType.customTypeName);
                    if (field_type.kind == VAR_TYPE_CUSTOM && !type_exists(field_type.custom_name)) {
                        char err[256];
                        snprintf(err, sizeof(err), "Unknown type '%s' for field '%s'.", field_type.custom_name, fname);
                        sem_error(f->line, f->pos, err);
                    }

                    if (f->as.varDecl.initializer != NULL) {
                        sem_error(f->line, f->pos, "Default field initializers are not allowed in C-style structs.");
                    }

                } else {
                    sem_error(f->line, f->pos, "Only variable declarations are allowed in C-style structs.");
                }
            }
            return make_type(VAR_TYPE_NULL, NULL);
        }

        case NODE_VAR_DECL: {
            TypeInfo init_type = make_type(VAR_TYPE_NULL, NULL);
            if (node->as.varDecl.initializer) {
                init_type = analyze_node(node->as.varDecl.initializer);

                Node *declTypeNode = node->as.varDecl.varType;
                if (declTypeNode->as.varType.kind == VAR_TYPE_LIST && node->as.varDecl.initializer->type == NODE_LIST_LITERAL) {
                    int expected_size = declTypeNode->as.varType.listSize;
                    int actual_size = 0;
                    for (Node *e = node->as.varDecl.initializer->as.list.elements; e; e = e->next) actual_size++;

                    if (actual_size != expected_size) {
                        char err[128];
                        snprintf(err, sizeof(err), "List size mismatch: expected %d, got %d.", expected_size, actual_size);
                        sem_error(node->as.varDecl.initializer->line, node->as.varDecl.initializer->pos, err);
                    }
                }
            }

            TypeInfo decl_type = make_type(node->as.varDecl.varType->as.varType.kind,
                                           node->as.varDecl.varType->as.varType.customTypeName);

            if (decl_type.kind == VAR_TYPE_CUSTOM && !type_exists(decl_type.custom_name)) {
                char err[256];
                snprintf(err, sizeof(err), "Unknown type '%s'.", decl_type.custom_name);
                sem_error(node->line, node->pos, err);
            }

            if (node->as.varDecl.initializer && !is_valid_assignment(decl_type, init_type)) {
                char err[128];
                snprintf(err, sizeof(err), "Type mismatch in declaration of '%s'.", node->as.varDecl.name);
                sem_error(node->line, node->pos, err);
            }

            node->symbol_table_index = declare_symbol(node->as.varDecl.name, decl_type, false, NULL, node->line, node->pos);
            return make_type(VAR_TYPE_NULL, NULL);
        }

        case NODE_FUN_DECL: {
            TypeInfo ret_type = make_type(node->as.funDecl.function->as.function.returnType->as.varType.kind,
                                          node->as.funDecl.function->as.function.returnType->as.varType.customTypeName);

            node->symbol_table_index = declare_symbol(node->as.funDecl.name, ret_type, false, NULL, node->line, node->pos);
            analyze_node(node->as.funDecl.function);
            return make_type(VAR_TYPE_NULL, NULL);
        }

        case NODE_LIST_LITERAL: {
            for (Node *e = node->as.list.elements; e; e = e->next) analyze_node(e);
            return make_type(VAR_TYPE_LIST, NULL);
        }

        case NODE_FUNCTION:
        case NODE_LAMBDA: {
            begin_scope();

            Node *fn_node = (node->type == NODE_LAMBDA) ? node->as.lambda.function : node;

            int prev_fn_depth = function_depth;
            TypeInfo prev_ret_type = current_return_type;
            int prev_loop_depth = loop_depth;

            function_depth++;
            loop_depth = 0;
            current_return_type = make_type(fn_node->as.function.returnType->as.varType.kind,
                                            fn_node->as.function.returnType->as.varType.customTypeName);

            for (Node *p = fn_node->as.function.parameters; p; p = p->next) {
                if (p->type == NODE_TYPED_VAR) {
                    TypeInfo p_type = make_type(p->as.typedVar.varType->as.varType.kind,
                                                p->as.typedVar.varType->as.varType.customTypeName);

                    if (p_type.kind == VAR_TYPE_CUSTOM && !type_exists(p_type.custom_name)) {
                        char err[256];
                        snprintf(err, sizeof(err), "Unknown type '%s' in parameter '%s'.", p_type.custom_name, p->as.typedVar.name);
                        sem_error(p->line, p->pos, err);
                    }

                    p->symbol_table_index = declare_symbol(p->as.typedVar.name, p_type, false, NULL, p->line, p->pos);
                }
            }

            if (fn_node->as.function.body && fn_node->as.function.body->type == NODE_BLOCK) {
                analyze_list(fn_node->as.function.body->as.block.declarations);
            } else {
                analyze_node(fn_node->as.function.body);
            }

            if (current_return_type.kind != VAR_TYPE_NULL && current_return_type.kind != VAR_TYPE_DYN) {
                if (!check_returns_list(fn_node->as.function.body)) {
                    sem_error(node->line, node->pos, "Able to reach end of function without 'ret'.");
                }
            }

            function_depth = prev_fn_depth;
            current_return_type = prev_ret_type;
            loop_depth = prev_loop_depth;

            end_scope();
            return make_type(VAR_TYPE_DYN, NULL);
        }

        case NODE_PRIMARY:
            if (node->as.primary.type == PRIMARY_IDENTIFIER) {
                char *name = node->as.primary.val.stringval;
                Symbol *sym = resolve_symbol(name, node->line, node->pos);
                if (sym) {
                    if (sym->is_type_def) {
                        sem_error(node->line, node->pos, "Cannot use type name as a variable.");
                        return make_type(VAR_TYPE_DYN, NULL);
                    }
                    node->symbol_table_index = sym->sym_index;
                    return sym->type;
                }
                return make_type(VAR_TYPE_DYN, NULL);

            } else if (node->as.primary.type == PRIMARY_INT) return make_type(VAR_TYPE_INT, NULL);
            else if (node->as.primary.type == PRIMARY_FLOAT) return make_type(VAR_TYPE_FLOAT, NULL);
            else if (node->as.primary.type == PRIMARY_STRING) return make_type(VAR_TYPE_STRING, NULL);
            else if (node->as.primary.type == PRIMARY_BOOL) return make_type(VAR_TYPE_BOOL, NULL);
            else if (node->as.primary.type == PRIMARY_NULL) return make_type(VAR_TYPE_NULL, NULL);
            return make_type(VAR_TYPE_DYN, NULL);

        case NODE_ASSIGN_OP: {
            if (!is_lvalue(node->as.assign.left)) {
                sem_error(node->line, node->pos, "Left-hand side is not assignable (must be an l-value).");
            }
            TypeInfo left_type = analyze_node(node->as.assign.left);
            TypeInfo right_type = analyze_node(node->as.assign.right);

            if (!is_valid_assignment(left_type, right_type)) {
                sem_error(node->line, node->pos, "Type mismatch in assignment.");
            }
            return left_type;
        }

        case NODE_UNARY_OP: {
            OperatorType op = node->as.unary.op;
            if (op == OP_PLUS_PLUS || op == OP_MINUS_MINUS) {
                if (!is_lvalue(node->as.unary.expr)) {
                    sem_error(node->line, node->pos, "Operand of prefix increment/decrement must be an l-value.");
                }
            }

            TypeInfo expr_type = analyze_node(node->as.unary.expr);

            if (op == OP_BANG) {
                if (expr_type.kind != VAR_TYPE_BOOL && expr_type.kind != VAR_TYPE_DYN) {
                    sem_error(node->line, node->pos, "Operand of '!' must be a boolean.");
                }
                return make_type(VAR_TYPE_BOOL, NULL);
            }

            if (op == OP_TILDE) {
                if (expr_type.kind != VAR_TYPE_INT && expr_type.kind != VAR_TYPE_DYN) {
                    sem_error(node->line, node->pos, "Operand of '~' must be an integer.");
                }
                return expr_type.kind == VAR_TYPE_INT ? expr_type : make_type(VAR_TYPE_DYN, NULL);
            }

            if (op == OP_PLUS || op == OP_MINUS) {
                if (expr_type.kind != VAR_TYPE_INT && expr_type.kind != VAR_TYPE_FLOAT && expr_type.kind != VAR_TYPE_DYN) {
                    sem_error(node->line, node->pos, "Operand of unary '+' or '-' must be an integer or float.");
                }
                return expr_type.kind == VAR_TYPE_DYN ? make_type(VAR_TYPE_DYN, NULL) : expr_type;
            }

            return expr_type;
        }

        case NODE_POSTFIX_OP:
            if (!is_lvalue(node->as.postfix.expr)) {
                sem_error(node->line, node->pos, "Operand of postfix increment/decrement must be an l-value.");
            }
            return analyze_node(node->as.postfix.expr);

        case NODE_BINARY_OP: {
            TypeInfo left = analyze_node(node->as.binary.left);
            TypeInfo right = analyze_node(node->as.binary.right);
            OperatorType op = node->as.binary.op;

            if (op == OP_DOUBLE_AND || op == OP_DOUBLE_PIPE) {
                if (left.kind != VAR_TYPE_BOOL && left.kind != VAR_TYPE_DYN) {
                    sem_error(node->line, node->pos, "Left operand of logical operator must be a boolean.");
                }
                if (right.kind != VAR_TYPE_BOOL && right.kind != VAR_TYPE_DYN) {
                    sem_error(node->line, node->pos, "Right operand of logical operator must be a boolean.");
                }
                return make_type(VAR_TYPE_BOOL, NULL);
            }

            if (op == OP_LESS || op == OP_GREATER || op == OP_LESS_EQ ||
                op == OP_GREATER_EQ || op == OP_BANG_EQ || op == OP_EQ_EQ) {
                if (left.kind != VAR_TYPE_DYN && right.kind != VAR_TYPE_DYN) {
                    if (!types_equal(left, right)) {
                        if (!((left.kind == VAR_TYPE_INT && right.kind == VAR_TYPE_FLOAT) ||
                              (left.kind == VAR_TYPE_FLOAT && right.kind == VAR_TYPE_INT))) {
                            sem_error(node->line, node->pos, "Type mismatch in comparison operator.");
                        }
                    }
                }
                return make_type(VAR_TYPE_BOOL, NULL);
            }

            if (left.kind == VAR_TYPE_STRING || right.kind == VAR_TYPE_STRING) {
                if (op == OP_PLUS) {
                    if (left.kind == VAR_TYPE_STRING && right.kind == VAR_TYPE_STRING) return make_type(VAR_TYPE_STRING, NULL);
                    if (left.kind == VAR_TYPE_DYN || right.kind == VAR_TYPE_DYN) return make_type(VAR_TYPE_DYN, NULL);
                    sem_error(node->line, node->pos, "Invalid concatenation of string with another type.");
                } else {
                    sem_error(node->line, node->pos, "Invalid operation on string.");
                }
                return make_type(VAR_TYPE_DYN, NULL);
            }

            if (left.kind == VAR_TYPE_DYN || right.kind == VAR_TYPE_DYN) return make_type(VAR_TYPE_DYN, NULL);

            if (op == OP_PLUS || op == OP_MINUS || op == OP_STAR || op == OP_SLASH || op == OP_PERC) {
                if (!((left.kind == VAR_TYPE_INT || left.kind == VAR_TYPE_FLOAT) &&
                      (right.kind == VAR_TYPE_INT || right.kind == VAR_TYPE_FLOAT))) {
                    sem_error(node->line, node->pos, "Arithmetic operators only support int or float operands.");
                    return make_type(VAR_TYPE_DYN, NULL);
                }
                if (!types_equal(left, right)) {
                    if ((left.kind == VAR_TYPE_INT && right.kind == VAR_TYPE_FLOAT) ||
                        (left.kind == VAR_TYPE_FLOAT && right.kind == VAR_TYPE_INT)) {
                        return make_type(VAR_TYPE_FLOAT, NULL);
                    }
                }
                return left;
            }

            if (op == OP_AND || op == OP_PIPE || op == OP_XOR ||
                op == OP_SHIFT_LEFT || op == OP_SHIFT_RIGHT) {
                if (left.kind != VAR_TYPE_INT || right.kind != VAR_TYPE_INT) {
                    sem_error(node->line, node->pos, "Bitwise operators only support int operands.");
                    return make_type(VAR_TYPE_DYN, NULL);
                }
                return make_type(VAR_TYPE_INT, NULL);
            }

            if (!types_equal(left, right)) {
                if ((left.kind == VAR_TYPE_INT && right.kind == VAR_TYPE_FLOAT) ||
                    (left.kind == VAR_TYPE_FLOAT && right.kind == VAR_TYPE_INT)) {
                    return make_type(VAR_TYPE_FLOAT, NULL);
                }
                sem_error(node->line, node->pos, "Type mismatch in binary operation.");
                return make_type(VAR_TYPE_DYN, NULL);
            }
            return left;
        }

        case NODE_TERNARY_OP: {
            TypeInfo cond = analyze_node(node->as.ternary.condition);
            if (cond.kind != VAR_TYPE_BOOL && cond.kind != VAR_TYPE_DYN) {
                sem_error(node->line, node->pos, "Condition of ternary operator must be a boolean.");
            }

            TypeInfo t_if = analyze_node(node->as.ternary.ifBranch);
            TypeInfo t_else = analyze_node(node->as.ternary.elseBranch);

            if (t_if.kind == VAR_TYPE_DYN || t_else.kind == VAR_TYPE_DYN) return make_type(VAR_TYPE_DYN, NULL);

            if (!types_equal(t_if, t_else) && t_if.kind != VAR_TYPE_NULL && t_else.kind != VAR_TYPE_NULL) {
                if ((t_if.kind == VAR_TYPE_INT && t_else.kind == VAR_TYPE_FLOAT) ||
                    (t_if.kind == VAR_TYPE_FLOAT && t_else.kind == VAR_TYPE_INT)) {
                    return make_type(VAR_TYPE_FLOAT, NULL);
                }
                sem_error(node->line, node->pos, "Ternary branches must have matching types.");
                return make_type(VAR_TYPE_DYN, NULL);
            }
            return t_if;
        }

        case NODE_EXPR_STMT: analyze_node(node->as.exprStmt.expr); break;
        case NODE_ECHO_STMT: analyze_node(node->as.echoStmt.expr); break;

        case NODE_RETURN: {
            if (function_depth == 0) {
                sem_error(node->line, node->pos, "'ret' statement outside of a function.");
                return make_type(VAR_TYPE_NULL, NULL);
            }
            TypeInfo ret_expr_type = make_type(VAR_TYPE_NULL, NULL);
            if (node->as.returnStmt.expr) {
                ret_expr_type = analyze_node(node->as.returnStmt.expr);
            }

            if (current_return_type.kind != VAR_TYPE_DYN && ret_expr_type.kind != VAR_TYPE_DYN) {
                if (current_return_type.kind == VAR_TYPE_NULL && ret_expr_type.kind != VAR_TYPE_NULL) {
                    sem_error(node->line, node->pos, "Function returns null (void) but a value was returned.");
                } else if (current_return_type.kind != VAR_TYPE_NULL && ret_expr_type.kind == VAR_TYPE_NULL) {
                    sem_error(node->line, node->pos, "Function expects a return value, but an empty 'ret' was used.");
                } else if (!is_valid_assignment(current_return_type, ret_expr_type)) {
                    sem_error(node->line, node->pos, "Return type does not match function signature.");
                }
            }
            return make_type(VAR_TYPE_NULL, NULL);
        }

        case NODE_IF_STMT: {
            TypeInfo cond = analyze_node(node->as.ifStmt.condition);
            if (cond.kind != VAR_TYPE_BOOL && cond.kind != VAR_TYPE_DYN) {
                sem_error(node->line, node->pos, "'if' condition must evaluate to a boolean.");
            }
            analyze_node(node->as.ifStmt.thenBranch);
            if (node->as.ifStmt.elseBranch) analyze_node(node->as.ifStmt.elseBranch);
            break;
        }

        case NODE_WHILE_STMT:
        case NODE_DO_WHILE_STMT: {
            Node *cond_node = (node->type == NODE_WHILE_STMT) ? node->as.whileStmt.condition : node->as.doWhileStmt.condition;
            TypeInfo cond = analyze_node(cond_node);
            if (cond.kind != VAR_TYPE_BOOL && cond.kind != VAR_TYPE_DYN) {
                sem_error(node->line, node->pos, "Loop condition must evaluate to a boolean.");
            }
            loop_depth++;
            analyze_node(node->type == NODE_WHILE_STMT ? node->as.whileStmt.body : node->as.doWhileStmt.body);
            loop_depth--;
            break;
        }

        case NODE_FOR_STMT: {
            begin_scope();
            analyze_node(node->as.forStmt.init);
            if (node->as.forStmt.condition) {
                TypeInfo cond = analyze_node(node->as.forStmt.condition);
                if (cond.kind != VAR_TYPE_BOOL && cond.kind != VAR_TYPE_DYN) {
                    sem_error(node->line, node->pos, "'for' loop condition must evaluate to a boolean.");
                }
            }
            analyze_node(node->as.forStmt.increment);
            loop_depth++;
            analyze_node(node->as.forStmt.body);
            loop_depth--;
            end_scope();
            break;
        }

        case NODE_BREAK:
            if (loop_depth == 0) sem_error(node->line, node->pos, "'break' statement outside of a loop.");
            break;

        case NODE_CONTINUE:
            if (loop_depth == 0) sem_error(node->line, node->pos, "'continue' statement outside of a loop.");
            break;

        case NODE_MEMBER_ACCESS: {
            TypeInfo obj_type = analyze_node(node->as.memberAccess.object);

            if (obj_type.kind == VAR_TYPE_CUSTOM) {
                return get_member_type(obj_type.custom_name, node->as.memberAccess.member, node->line, node->pos);
            } else if (obj_type.kind != VAR_TYPE_DYN) {
                sem_error(node->line, node->pos, "Member access requires a custom type object.");
            }
            return make_type(VAR_TYPE_DYN, NULL);
        }

        case NODE_INDEX_ACCESS: {
            TypeInfo obj_type = analyze_node(node->as.indexAccess.object);
            TypeInfo index_type = analyze_node(node->as.indexAccess.index);

            if (obj_type.kind != VAR_TYPE_LIST && obj_type.kind != VAR_TYPE_DYN) {
                sem_error(node->line, node->pos, "Index access requires a list.");
            }
            if (index_type.kind != VAR_TYPE_INT && index_type.kind != VAR_TYPE_DYN) {
                sem_error(node->line, node->pos, "Index must be an integer.");
            }
            return make_type(VAR_TYPE_DYN, NULL);
        }

        case NODE_CALL: {
            analyze_node(node->as.call.callee);
            analyze_list(node->as.call.arguments);
            return make_type(VAR_TYPE_DYN, NULL);
        }

        default: break;
    }
    return make_type(VAR_TYPE_NULL, NULL);
}

bool analyze_ast(Node *ast_root, bool debug_mode) {
    has_errors = false;
    is_debug_mode = debug_mode;
    sym_count = 0;
    current_depth = 0;
    next_sym_index = 1;
    loop_depth = 0;
    function_depth = 0;
    current_return_type = make_type(VAR_TYPE_NULL, NULL);

    analyze_node(ast_root);

    return !has_errors;
}