#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "defs.h"
#include <stdbool.h>

bool analyze_ast(Node *ast_root, bool debug_mode);
void print_symbol_table(void);
void print_type_registry(void);

#endif