
#include "db.h"


todo_db_t *
todo_db_new (char path) {
  int rc;
  void *env;
  todo_db_t *db = malloc(sizeof(todo_db_t));

  // bail if failed to allocate memoery
  if (NULL == db) return NULL;

  env = sp_env();

  // error in memoery allocation
  if (NULL == env) return NULL;

  // create if not exists in read-write mode
  rc = sp_ctl(env, SPDIR, SPO_CREAT|SPO_RDWR, path);

  if (-1 == rc) {
    todo_ferror("%s\n", sp_error(env));
    sp_destroy(env);
    return NULL;
  }

  db->env = env;

  return db;
}
