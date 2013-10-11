
#include <stdio.h>
#include "deps/commander.h"
#include "src/todo.h"

struct options {
  char *path;
  int verbose;
} opts_t;

static struct options opts;

// flags
static void
path_opt (command_t *self);

static void
verbose_opt (command_t *self);

int
main (int argc, char *argv[]) {
  char tmp[256], *cmd = argv[1];
  command_t program;

  command_init(&program, "todo-init", "0.0.1");

  if (NULL == cmd) {
    cmd = "";
  }

  program.usage = "[--version] [--verbose] [--path <path>]";

  // opts
  command_option(&program, "-v", "--verbose", "enable verbose output", verbose_opt);
  command_option(&program, "-p", "--path <path>", "set the path to initialize", path_opt);

  // parse
  command_parse(&program, argc, argv);

  if (NULL == opts.path) {
    opts.path = getcwd(tmp, sizeof(tmp));
  }

  if (1 == opts.verbose) {
    todo_printf("path set to '%s'\n", opts.path);
  }

  if (0 == strcmp("help", cmd)) {
    command_help(&program);
    exit(1);
  } else {
    todo_t *todo = todo_init(opts.path, 1);
    if (1 == todo->is_fresh) {
      todo_printf("Initialized a new todo project at %s\n", todo->path);
    }
  }

  command_free(&program);
  return 0;
}


static void
path_opt (command_t *self) {
  opts.path = (char *)self->arg;
}


static void
verbose_opt (command_t *self) {
  opts.verbose = 1;
}
