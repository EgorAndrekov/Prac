#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *xstrdup(const char *s) {
    size_t n = strlen(s)+1;
    char *p = malloc(n);
    if (p) memcpy(p,s,n);
    return p;
}

static cmd_t *new_cmd(void) {
    return calloc(1, sizeof(cmd_t));
}

static void argv_add(cmd_t *c, const char *s) {
    int n = 0;
    if (c->argv) while (c->argv[n]) n++;
    c->argv = realloc(c->argv, sizeof(char*) * (n + 2));
    c->argv[n] = xstrdup(s);
    c->argv[n + 1] = NULL;
}

static cmd_t *parse_simple(lex_t **l) {
    cmd_t *c = new_cmd();
    int has_argv = 0;

    while (*l) {
        if (!strcmp((*l)->s,"|") ||
            !strcmp((*l)->s,";") ||
            !strcmp((*l)->s,"&&")||
            !strcmp((*l)->s,"||")||
            !strcmp((*l)->s,"&")||
            !strcmp((*l)->s,")"))
            break;
        
        /* --- subshell: ( ... ) --- */
        if (!strcmp((*l)->s, "(")) {
            *l = (*l)->next;

            c->subshell = build_tree(*l);

            int level = 1;
            while (*l && level) {
                if (!strcmp((*l)->s, "(")) level++;
                else if (!strcmp((*l)->s, ")")) level--;
                *l = (*l)->next;
            }

            has_argv = 1;
            continue;
        }


        if (!strcmp((*l)->s,"<")) {
            *l = (*l)->next;
            if (*l) c->in = xstrdup((*l)->s);
        } else if (!strcmp((*l)->s,">") ||
                   !strcmp((*l)->s,">>")) {
            c->append = ((*l)->s[1] == '>');
            *l = (*l)->next;
            if (*l) c->out = xstrdup((*l)->s);
        } else {
            argv_add(c, (*l)->s);
            has_argv = 1;
        }
        *l = (*l)->next;
    }

    if (!has_argv && !c->in && !c->out) {
        tree_free(c);
        return NULL;
    }
    return c;
}

cmd_t *build_tree(lex_t *l) {
    cmd_t *head = NULL, **cur = &head;

    while (l) {
        cmd_t *c = parse_simple(&l);
        if (!c) break;

        *cur = c;
        cur = &c->next;

        if (!l) break;

        if (!strcmp(l->s,"|")) {
            l = l->next;
            c->pipe = build_tree(l);
            break;
        }
        if (!strcmp(l->s,"&&")) c->op = OP_AND;
        else if (!strcmp(l->s,"||")) c->op = OP_OR;
        else if (!strcmp(l->s,"&"))  c->bg = 1;

        l = l->next;
    }
    return head;
}

void tree_free(cmd_t *c) {
    if (!c) return;
    tree_free(c->subshell);
    tree_free(c->pipe);
    tree_free(c->next);
    if (c->argv) {
        for (int i=0; c->argv[i]; i++)
            free(c->argv[i]);
        free(c->argv);
    }
    free(c->in);
    free(c->out);
    free(c);
}

static void print_indent(int n) {
    for (int i = 0; i < n; i++)
        printf("  ");
}

static const char *op_str(int op) {
    switch (op) {
        case OP_AND: return "&&";
        case OP_OR:  return "||";
        default:     return ";";
    }
}

void tree_print(cmd_t *t, int indent) {
    for (; t; t = t->next) {
        print_indent(indent);
        printf("CMD");

        if (t->bg)
            printf(" &");

        if (t->argv && t->argv[0]) {
            printf(" argv:");
            for (int i = 0; t->argv[i]; i++)
                printf(" [%s]", t->argv[i]);
        }

        if (t->in)
            printf(" < %s", t->in);
        if (t->out)
            printf(" %s %s", t->append ? ">>" : ">", t->out);

        printf("\n");

        if (t->pipe) {
            print_indent(indent);
            printf("PIPE |\n");
            tree_print(t->pipe, indent + 1);
        }

        if (t->subshell) {
            print_indent(indent);
            printf("SUBSHELL (\n");
            tree_print(t->subshell, indent + 1);
            print_indent(indent);
            printf(")\n");
        }

        if (t->op) {
            print_indent(indent);
            printf("OP %s\n", op_str(t->op));
        }
    }
}
