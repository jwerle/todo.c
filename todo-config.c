
#include <stdio.h>
#include "deps/commander.h"
#include "src/todo.h"

struct options {
  char *path;
  int verbose;
  char *key;
  char *value;
} opts_t;

static struct options opts;

// flags
static void
path_opt (command_t *self);

static void
verbose_opt (command_t *self);

static void
key_opt (command_t *self);

static void
value_opt (command_t *self);



static void
help (command_t *program);

int
main (int argc, char *argv[]) {
  char tmp[256], *cmd = argv[1];
  int rc;
  todo_t *todo;
  command_t program;

  if (NULL == cmd) {
    cmd = "";
  }

  command_init(&program, "todo-config", "0.0.1");

  // usage
  program.usage = "<comannd> [<args>] [--key <key>] [--verbose] [--path <path] <command> [<args>]";

  // opts
  command_option(&program, "-p", "--path <path>", "set the scope path", path_opt);
  command_option(&program, "-V", "--verbose", "enable verbose output", verbose_opt);
  command_option(&program, "-k", "--key <name>", "config key", key_opt);
  command_option(&program, "-v", "--value <value>", "config key", value_opt);

  // parse cli
  command_parse(&program, argc, argv);

  if (NULL == opts.path) {
    opts.path = getcwd(tmp, sizeof(tmp));
  }

  if (1 == opts.verbose) {
    todo_printf("path set to '%s'\n", opts.path);
  }

  if (0 == strcmp("get", cmd)) {
    if (NULL == opts.key) {
      todo_error("missing the '--key <name>' option");
      exit(1);
    }

    todo = todo_init(opts.path, 0);
    if (1 == opts.verbose) printf("initialized..\n");
    char * value = todo_config_get(todo, opts.key);
    if (NULL == value) {
      if (opts.verbose) {
        todo_printf("no value found for key '%s'\n", opts.key);
      }

      exit(1);
    } else {
      printf("%s\n", value);
    }
  } else if (0 == strcmp("set", cmd)) {
    if (NULL == opts.key) {
      todo_error("missing the '--key <name>' option");
      exit(1);
    } else if (NULL == opts.value) {
      todo_error("missing the '--value <value>' option");
    }

    todo = todo_init(opts.path, 0);
    rc = todo_config_set(todo, opts.key, opts.value);
    if (-1 == rc) {
      if (1 == opts.verbose) {
        todo_ferror("error setting key '%s' (%s)", opts.key, sp_error(todo->db->env));
      }
      exit(1);
    } else {
      exit(0);
    }
  } else if (0 == strcmp("list", cmd) || 0 == strcmp("ls", cmd)) {
    todo = todo_init(opts.path, 0);
    todo_config_list(todo);
  } else if (0 == strcmp("clear", cmd)) {
    todo = todo_init(opts.path, 0);
    todo_config_clear(todo);
  } else if (0 == strcmp("rm", cmd) || 0 == strcmp("remove", cmd)) {
    todo = todo_init(opts.path, 0);
    rc = todo_config_rm(todo, opts.key);
    printf("%d\n", rc);
    if (-1 == rc) {
      if (1 == opts.verbose) {
        todo_ferror("error removing key '%s' %s", opts.key, sp_error(todo->db->env));
      }
      exit(1);
    }
  } else {
    help(&program);
  }

  command_free(&program);
  return 0;
}


static void
key_opt (command_t *self) {
  opts.key = (char *) self->arg;
}


static void
value_opt (command_t *self) {
  opts.value = (char *) self->arg;
}


static void
path_opt (command_t *self) {
  opts.path = (char *) self->arg;
}


static void
verbose_opt (command_t *self) {
  opts.verbose = 1;
}


static void
help (command_t *program) {
  command_help(program);
  printf("  get <name>                   get value\n");
  printf("  set <name> <value>           set value\n");
}
