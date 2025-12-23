#ifndef TREE_H
#define TREE_H

#include "list.h"

enum { OP_NONE, OP_AND, OP_OR };

typedef struct cmd {
    char **argv;
    char *in, *out;
    int append;
    int bg;
    int op;
    struct cmd *pipe;
    struct cmd *next;
    struct cmd *subshell;
} cmd_t;

cmd_t *build_tree(lex_t *l);
void   tree_free(cmd_t *c);
void cmd_free(cmd_t *c);
void tree_print(cmd_t *t, int indent);



#endif
