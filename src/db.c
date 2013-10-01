
#include "db.h"


todo_db_t *
todo_db_new (char *path) {
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
  db->path = path;

  return db;
}


int
todo_db_open (todo_db_t *db) {
  db->dbh = sp_open(db->env);

  if (NULL == db->dbh) {
    todo_ferror("%s\n", sp_error(db->env));
    sp_destroy(db->env);
    return -1;
  }

  return 0;
}



int
todo_db_set (todo_db_t *db, char *key, char *value) {
  if (NULL == db->dbh) {
    todo_ferror("Attempting set on unopened database at '%s'\n", db->path);
    return -1;
  }

  int rc = sp_set(db->dbh, key, sizeof(key), value, sizeof(value));

  if (-1 == rc) {
    todo_ferror("Error setting '%s'. sophia: '%s'\n", key, sp_error(db->dbh));
    return rc;
  }

  return 0;
}


char *
todo_db_get (todo_db_t *db, char *key) {
  void *value;
  size_t size;
  int rc;

  rc = sp_get(db->dbh, key, sizeof(key), &value, &size);

  printf("%d\n", rc);

  if (-1 == rc) {
    todo_ferror("Error getting '%s'. sophia: '%s'\n", key, sp_error(db->dbh));
    return NULL;
  } else if (1 == rc) {
    return value;
  }

  return NULL;
}
