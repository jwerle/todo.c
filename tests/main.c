
#include "../src/todo.h"

int
main (int argc, char *argv[]) {
  todo_db_t *db = todo_db_new("./db");

  if (-1 == todo_db_open(db)) {
    todo_error("Failed to open database");
  }

  todo_db_set(db, "foo", "bar");
  printf("value: %s\n", todo_db_get(db, "foo"));

  return 0;
}
