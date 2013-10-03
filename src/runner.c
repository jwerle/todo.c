
#include "todo.h"
#include <stdio.h>
#include <string.h>

void
todo_runner_usage (char *cmd, char *opts);

int
todo_runner_has_cmd (char *cmd);

void
todo_runner_unknown (char *type, char *arg);


void
todo_runner_usage (char *cmd, char *opts) {
  printf("usage: %s %s\n", cmd, opts);
}
