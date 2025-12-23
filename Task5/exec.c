#include "exec.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

int shell_exit = 0;

static int is_builtin(cmd_t *c) {
    return c && c->argv &&
        (!strcmp(c->argv[0],"cd") ||
         !strcmp(c->argv[0],"pwd")||
         !strcmp(c->argv[0],"exit"));
}

static int run_builtin(cmd_t *c) {
    if (!strcmp(c->argv[0],"exit")) {
        shell_exit = 1;
        return 0;
    }
    if (!strcmp(c->argv[0],"cd")) {
        const char *d = c->argv[1] ? c->argv[1] : getenv("HOME");
        if (chdir(d) < 0) perror("cd");
        return 0;
    }
    if (!strcmp(c->argv[0],"pwd")) {
        char b[1024];
        if (getcwd(b,sizeof(b))) puts(b);
        return 0;
    }
    return 0;
}

static pid_t spawn(cmd_t *c, int in, int out, int bg) {
    pid_t p = fork();
    if (p == 0) {
        signal(SIGINT, bg ? SIG_IGN : SIG_DFL);
        if (in)  dup2(in,0);
        if (out!=1) dup2(out,1);

        if (c->in) {
            int f=open(c->in,O_RDONLY);
            if (f<0) _exit(1);
            dup2(f,0);
        }
        if (c->out) {
            int f=open(c->out,
                O_WRONLY|O_CREAT|(c->append?O_APPEND:O_TRUNC),0666);
            if (f<0) _exit(1);
            dup2(f,1);
        }
        if (c->subshell) {
            int r = exec_tree(c->subshell);

            tree_free(c->subshell);
            c->subshell = NULL;

            _exit(r);
        }

        execvp(c->argv[0], c->argv);
        perror(c->argv[0]);
        _exit(1);
    }
    return p;
}

static int exec_pipe(cmd_t *cmd) {
    if (!cmd) return 0;
    if (!cmd->pipe && is_builtin(cmd))
        return run_builtin(cmd);

    int in = 0, fd[2];
    pid_t last = -1;
    int bg = cmd->bg;

    for (cmd_t *c=cmd; c; c=c->pipe) {
        int out = 1;
        if (c->pipe) {
            pipe(fd);
            out = fd[1];
        }
        last = spawn(c, in, out, bg);
        if (c->subshell) {
            tree_free(c->subshell);
            c->subshell = NULL;
        }
        if (in) close(in);
        if (out!=1) close(out);
        if (c->pipe) in = fd[0];
    }

    if (!bg && last > 0) {
        int st;
        waitpid(last, &st, 0);
        return WIFEXITED(st) ? WEXITSTATUS(st) : 1;
    }
    return 0;
}

int exec_tree(cmd_t *t) {
    int r = 0;
    extern volatile sig_atomic_t sigint_received;

    for (; t; t = t->next) {
        if (sigint_received)
            break;

        r = exec_pipe(t);

        if (sigint_received)
            break;

        if (t->op==OP_AND && r) break;
        if (t->op==OP_OR  && !r) break;
    }

    return r;
}
