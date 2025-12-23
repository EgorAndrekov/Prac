#ifndef EXEC_H
#define EXEC_H
#include "tree.h"

#include <signal.h>

extern volatile sig_atomic_t sigint_received;


extern int shell_exit;
int exec_tree(cmd_t *t);

#endif
