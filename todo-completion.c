
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_COMPLETION_FILE "/etc/todo-completions"

void
output (char *file);

void
usage ();

int
main (int argc, char *argv[]) {
  char *COMPLETION_FILE = getenv("TODO_COMPLETION_FILE");

  if (NULL == COMPLETION_FILE) {
    COMPLETION_FILE = DEFAULT_COMPLETION_FILE;
  }

  if (1 == argc) {
    output(COMPLETION_FILE);
  } else {
    printf("Unknown option: %s\n", argv[1]);
    usage();
  }

  return 0;
}

void
output (char *file) {
  FILE *fd = fopen(file, "r");
  long size;
  char *buffer;
  size_t result_size;

  if (NULL == fd) {
    fprintf(stderr, "error: Failed to open usage file '%s'\n", file);
    fprintf(stderr, "error: Set 'TODO_COMPLETION_FILE' environment variable\n");
    exit(1);
  }


  // seeek to end to get size
  fseek(fd, 0, SEEK_END);
  // store size from seek
  size = ftell(fd);
  // rewind to beginning of file
  rewind(fd);
  // malloc memory for buffer the size of `char` and file
  buffer = (char*) malloc(sizeof(char) * size);

  // if malloc failed then exit
  if (NULL == buffer) {
    fprintf(stderr, "error: Memory allocation error\n");
    exit(2);
  }

  // read into buffer and return result size
  result_size = fread(buffer, 1, size, fd);

  // if result size is not equal to told size
  // then exit with error
  if (size != result_size) {
    fprintf(stderr, "error: Error reading file '%s'\n", file);
    exit(3);
  }

  // output buffer to stdout
  printf("%s\n", buffer);

  // close the file descriptor and free the buffer
  fclose(fd);
  free(buffer);
  // exit with success
  exit(0);

}

void
usage () {
  printf("usage: todo-completion\n");
  exit(1);
}
