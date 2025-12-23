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


volatile sig_atomic_t sigint_received = 0;


static void sigint_handler(int signo) {
    (void)signo;
    sigint_received = 1;
    write(STDOUT_FILENO, "\n", 1);
}


/* C99-safe getline */
static char *read_line(void)
{
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

        lex = lex_parse(line);
        tree = build_tree(lex);

        //tree_print(tree, 0);

        sigint_received = 0;

        if (tree)
            exec_tree(tree);


        tree_free(tree); tree = NULL;
        lex_free(lex);   lex  = NULL;
        free(line);      line = NULL;
    }

    tree_free(tree);
    lex_free(lex);
    free(line);
    return 0;
}
