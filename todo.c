
#include "src/todo.h"
#include "deps/path.h"

char *cmds[] = {

  "ls", "new", "completion"

};


int cmdc = sizeof(cmds) / sizeof(cmds[0]);


void
usage ();


int
main (int argc, char *argv[]) {
  int i = 0, has_cmd;
  char *paths[256];

  path_split(getenv("PATH"), paths, ":");

  if (1 == argc) {
    usage();
    return 1;
  } else if (0 == strcmp("commands", argv[1]) ||
             0 == strcmp("help", argv[1])) {

    usage();
    printf("\nwhere <command> is one of:\n");
    for (; i < cmdc; ++i) {
      printf("    %s\n", cmds[i]);
    }

    return 0;

  } else {
    if (0 == strncmp("-", argv[1], 1)) {
      fprintf(stderr, "error: Unknown option '%s'\n", argv[1]);
    } else {
      fprintf(stderr, "error: Unknown command '%s'\n", argv[1]);
    }

    usage();
    return 1;
  }

  return 0;
}


void
usage () {
  printf("usage: todo <command> [<args>]\n");
}
