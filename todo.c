
#include <stdio.h>
#include "src/todo.h"
#include "deps/path.h"
#include "deps/commander.h"


struct options {
  char *path;
  int verbose;
} opts_t;

static struct options opts;

static void
help (command_t *program);



// flag callbacks
static void
path_opt (command_t *self);

static void
verbose_opt (command_t *self);


char *
man_exec () {
  char **paths = calloc(1, sizeof(char));
  char *pathv = strdup(getenv("PATH"));
  int pathc = path_split(pathv, paths, ":");
  int i = 0, rc;

  for (; i < pathc; ++i) {
    char *cmd = malloc(sizeof(char) * 256);
    sprintf(cmd, "%s/man", paths[i]);
    rc = access(cmd, X_OK);
    if (0 == rc) {
      free(pathv);
      return cmd;
    } else {
      free(cmd);
    }
  }

  free(pathv);
  return NULL;
}

int
main (int argc, char *argv[]) {
  char tmp[256];
  char *cmd = argv[1];
  char **paths = calloc(1, sizeof(char));
  char *mancmd = man_exec();
  char *pathv = strdup(getenv("PATH"));
  int pathc = path_split(pathv, paths, ":");
  int i = 0, rc;

  // check if we can make calls
  // to the command processor
  if (0 == system(NULL)) {
    todo_error("systems command processor is not available.");
    exit(1);
  }

  if (NULL == cmd) {
    cmd = "help";
  }

  if (argc > 1 && 0 != strcmp(cmd, "help") && 0 != strncmp(cmd, "-", 1)) {
    for (; i < pathc; ++i) {
      char *tpath = malloc(sizeof(char) * 256);
      char tcmd[4096];

      sprintf(tpath, "%s/todo-%s", paths[i], cmd);
      rc = access(tpath, X_OK);

      if (0 == rc) {
        char *args = todo_flatten(argv, 2, argc);
        sprintf(tcmd, "%s %s", tpath, args);
        rc = system(tcmd);
        free(tpath);
        exit(rc);
      }

      free(tpath);
    }

   todo_ferror("'todo %s' is not a todo command found in your path.", cmd);
  }

  command_t program;
  command_init(&program, "todo", TODO_VERSION_STR);

  program.usage = "<command> [<args>] [--version] [--path <path>] [--verbose]";

  // opts
  command_option(&program, "-p", "--path <path>", "set the scope path", path_opt);
  command_option(&program, "-v", "--verbose", "enable verbose output", verbose_opt);

  // parse cli
  command_parse(&program, argc, argv);

  // allow path scope overide
  if (NULL == opts.path) {
    opts.path = getcwd(tmp, sizeof(tmp));
  }

  if (1 == opts.verbose) {
    todo_printf("path set to '%s'\n", opts.path);
  }

  if (0 == strcmp("help", cmd)) {
    if (argc > 2) {
      if (NULL == mancmd) {
        todo_error("The program 'man' was not found in your path");
        exit(1);
      }

      char helpcmd[256];
      sprintf(helpcmd, "%s todo-%s", mancmd, argv[2]);
      rc = system(helpcmd);
      exit(rc);
    } else {
      help(&program);
    }
  } else {
    help(&program);
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

static void
help (command_t *program) {
  command_help(program);
  printf("See 'todo help <command>' for more help on a certain command.\n");
  exit(1);
}
