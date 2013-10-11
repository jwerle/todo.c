
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <commander.h>
#include <fs.h>
#include "init.h"


todo_t *
todo_init (char *path, int warn) {
  fs_stats *stats;
  todo_t *todo = malloc(sizeof(todo_t *));

  if (NULL == todo) return NULL;

  strcat(path, "/.todo");

  todo->is_fresh = 1;

  if ((stats = fs_stat(path)) && 1 == warn) {
    todo->is_fresh = 0;
    todo_ferror("todo is already initialized in '%s'", path);
  }

  char *db_path = strdup(path);
  strcat(db_path, "/db");

  fs_mkdir(path, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);

  todo->path = strdup(path);
  todo->db = todo_db_new(db_path);

  // fatal
  if (-1 == todo_db_open(todo->db)) {
    todo_error("Failed to open todo database");
    exit(1);
  }

  free(stats);

  return todo;
}
