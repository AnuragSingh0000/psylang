#ifndef DEFS_H
#define DEFS_H

#include <stdbool.h>

typedef struct DTNode {
    char *name;
    char *lexeme;
    int num_children; 
    struct DTNode **children;
} DTNode;

#endif