#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static lex_t *lex_new(const char *s) {
    lex_t *l = malloc(sizeof(*l));
    l->s = xstrdup(s);
    l->next = NULL;
    return l;
}

static void lex_push(lex_t **l, const char *s) {
    lex_t *n = lex_new(s);
    if (!*l) *l = n;
    else {
        lex_t *p = *l;
        while (p->next) p = p->next;
        p->next = n;
    }
}

static void emit_token(lex_t **l, const char *buf) {
    if (!buf || !buf[0]) return;

    if (buf[0] == '#')
        return;

    size_t len = strlen(buf);
    if (len >= 2 && buf[0] == '"' && buf[len - 1] == '"') {
        char tmp[1024];
        size_t n = len - 2;
        if (n >= sizeof(tmp)) n = sizeof(tmp) - 1;
        memcpy(tmp, buf + 1, n);
        tmp[n] = 0;
        lex_push(l, tmp);
        return;
    }

    if (buf[0] == '$') {
        const char *v = getenv(buf + 1);
        lex_push(l, v ? v : "");
        return;
    }

    lex_push(l, buf);
}


lex_t *lex_parse(const char *s) {
    lex_t *l = NULL;
    char buf[1024];
    int i = 0;

    int in_quotes = 0;
    int escaped = 0;

    for (; *s; s++) {

        if (escaped) {
            if (i < (int)sizeof(buf)-1)
                buf[i++] = *s;
            escaped = 0;
            continue;
        }

        if (*s == '\\') {
            escaped = 1;
            continue;
        }

        if (*s == '"') {
            in_quotes = !in_quotes;
            continue;
        }

        if (!in_quotes && *s == '#')
            break;

        if (!in_quotes && isspace((unsigned char)*s)) {
            if (i) {
                buf[i] = 0;
                emit_token(&l, buf);
                i = 0;
            }
        }
        else if (!in_quotes && strchr("|&;()<>", *s)) {
            if (i) {
                buf[i] = 0;
                emit_token(&l, buf);
                i = 0;
            }
            char t[3] = { *s, 0, 0 };
            if ((s[0]=='&'&&s[1]=='&') ||
                (s[0]=='|'&&s[1]=='|') ||
                (s[0]=='>'&&s[1]=='>')) {
                t[1] = *++s;
            }
            lex_push(&l, t);
        }
        else {
            if (i < (int)sizeof(buf)-1)
                buf[i++] = *s;
        }
    }

    if (i) {
        buf[i] = 0;
        emit_token(&l, buf);
    }
    return l;
}


void lex_free(lex_t *l) {
    while (l) {
        lex_t *n = l->next;
        free(l->s);
        free(l);
        l = n;
    }
}
