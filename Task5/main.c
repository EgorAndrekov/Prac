#include "list.h"
#include "tree.h"
#include "exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define HISTORY_SIZE 10

static char *history[HISTORY_SIZE] = {0};
static int history_count = 0;

volatile sig_atomic_t sigint_received = 0;

static void free_history(void) {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (history[i]) {
            free(history[i]);
            history[i] = NULL;
        }
    }
    history_count = 0;
}


static void add_to_history(const char *line) {
    if (!line || line[0] == '\0') {
        return;
    }
    
    if (history_count == HISTORY_SIZE) {
        free(history[0]);
        
        for (int i = 0; i < HISTORY_SIZE - 1; i++) {
            history[i] = history[i + 1];
        }
        
        history[HISTORY_SIZE - 1] = NULL;
        
    } else {
        history_count++;
    }
    
    
    history[history_count - 1] = strdup(line);
    
}

char **get_history(void) {
    return history;
}

int get_history_count(void) {
    return history_count;
}

static void sigint_handler(int signo) {
    (void)signo;
    sigint_received = 1;
    write(STDOUT_FILENO, "\n", 1);
}

static char *read_line(void) {
    size_t cap = 128, len = 0;
    char *s = malloc(cap);
    int c;

    if (!s) return NULL;

    while (1) {
        c = getchar();

        if (c == EOF) {
            if (sigint_received) {
                sigint_received = 0;
                free(s);
                return strdup("");
            }
            free(s);
            return NULL;
        }

        if (c == '\n')
            break;

        if (len + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(s, cap);
            if (!tmp) {
                free(s);
                return NULL;
            }
            s = tmp;
        }

        s[len++] = (char)c;
    }

    s[len] = 0;
    return s;
}

void inv() {     
    printf("%s", "\033[22;36m");
    char s[100]; 
    gethostname(s, 100);     
    printf("%s@%s", getenv("USER"), s);       
    printf("%s", "\x1B[37m");   
    getcwd(s, 100);     
    printf(":%s$ ", s); 
} 

int main(void) {
    char *line = NULL;
    lex_t *lex = NULL;
    cmd_t *tree = NULL;

    signal(SIGINT, sigint_handler);

    while (!shell_exit) {
        inv();
        fflush(stdout);

        line = read_line();
        if (!line) {
            printf("\n");
            break;
        }

        if (sigint_received) {
            sigint_received = 0;
            free(line);
            continue;
        }

        if (*line == 0) {
            free(line);
            continue;
        }

        add_to_history(line);

        lex = lex_parse(line);
        tree = build_tree(lex);

        // tree_print(tree, 0);

        sigint_received = 0;

        if (tree)
            exec_tree(tree);

        tree_free(tree); tree = NULL;
        lex_free(lex);   lex  = NULL;
        free(line);      line = NULL;
    }

    free_history();
    
    tree_free(tree);
    lex_free(lex);
    free(line);
    return 0;
}