
#include "../src/todo.h"
#include <assert.h>

int
main (int argc, char *argv[]) {
  todo_db_t *db = todo_db_new("./test-db");

  if (-1 == todo_db_open(db)) {
    todo_error("Failed to open database");
  }

  assert(0 == todo_db_set(db, "foo", "bar"));
  assert(0 == strcmp("bar", todo_db_get(db, "foo")));

  return 0;
}
