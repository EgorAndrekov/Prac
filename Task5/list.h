#ifndef LIST_H
#define LIST_H

typedef struct lex {
    char *s;
    struct lex *next;
} lex_t;

lex_t *lex_parse(const char *s);
void   lex_free(lex_t *l);

#endif
