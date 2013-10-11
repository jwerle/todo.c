
#include <stdio.h>
#include <sys/stat.h>
#include <commander.h>
#include <fs.h>
#include "todo.h"


todo_t *
todo_init (char *path) {
  todo_t *todo = malloc(sizeof(todo_t));
  strcat(path, "/.todo");

  todo->is_fresh = 1;

  // fatal
  if (fs_stat(path)) {
    todo->is_fresh = 0;
    todo_ferror("todo is already initialized in '%s'", path);
    exit(1);
  }

  char *db_path = strdup(path);
  char *config_path = strdup(path);
  strcat(db_path, "/db");
  strcat(config_path, "/config");

  // fatal
  if (-1 == fs_mkdir(path, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)) {
    todo_ferror("Error creating '.todo/' directory in '%s'", path);
    exit(1);
  }

  FILE *config = fs_open(config_path, "w");

  // fatal
  if (NULL == config) {
    fs_error("todo-init");
    todo_error("Error creating './todo/config' file (open)");
    exit(1);
  }

  // fatal
  if (-1 == fs_close(config)) {
    todo_error("Error creating './todo/config' file (close)");
    exit(1);
  }

  todo->config_path = config_path;
  todo->path = path;
  todo->db = todo_db_new(db_path);
  todo->config_path = config_path;

  // fatal
  if (-1 == todo_db_open(todo->db)) {
    todo_error("Failed to create todo database");
    exit(1);
  }

  return todo;
}
