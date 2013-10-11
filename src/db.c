
#include "db.h"

// current ket for getter
static const char * __cur_key = NULL;

static char *
handle_get_cb (char *ckey, char *cvalue, char *nkey, char *nvalue, todo_db_t *db) {
  char *value = NULL;
  char *tmp_ckey = strdup(ckey);
  char *tmp_nkey = strdup(nkey);
  int size = (int) strlen(__cur_key);
  int cksize = (int) strlen(ckey);
  int nksize = (int) strlen(nkey);

  if (0 != strcmp("", tmp_nkey) && size != nksize) {
    nksize--;
  }

  if (0 != strcmp("", tmp_ckey) && size != cksize) {
    cksize--;
  }

  if (NULL == nvalue) return value;

  // printf("%s = %s -- (%d) [%d]\n", ckey, cvalue, (int) strlen(ckey), strcmp(__cur_key, ckey));
  // printf("%s = %s -- (%d) [%d]\n", nkey, nvalue, (int) strlen(nkey), strcmp(__cur_key, nkey));

  // printf("%d %d\n", size, cksize);
  // printf("%d %d\n", size, nksize);

  if (size == cksize) {
    if (0 == strcmp(__cur_key, tmp_ckey) || 0 == nksize) {
      value = strdup(cvalue);
    }
  }

  if (size == nksize) {
    if (0 == strcmp(__cur_key, tmp_nkey)) {
      value = strdup(nvalue);
    }
  }

  free(tmp_ckey);
  free(tmp_nkey);

  return value;
}

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

  // turn on garbage collection
  sp_ctl(env, SPGC, 1);

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

  int rc = sp_set(db->dbh, strdup(key), strlen(key), strdup(value), strlen(value));

  if (-1 == rc) {
    todo_ferror("Error setting '%s'. sophia: '%s'\n", key, sp_error(db->dbh));
    return rc;
  }

  return 0;
}


char *
todo_db_get (todo_db_t *db, char *key) {
  void *value;

  __cur_key = key;
  value = todo_db_reduce(db, handle_get_cb);

  // reset
  __cur_key = NULL;
  return value;
}


int
todo_db_foreach (todo_db_t *db, void (*fn)(char *key, char *value, todo_db_t *db)) {
  void *c = sp_cursor(db->dbh, SPGT, NULL, 0);

  if (NULL == c) {
    return -1;
  }

  while (sp_fetch(c)) {
    fn(strdup(sp_key(c)), strdup(sp_value(c)), db);
  }

  sp_destroy(c);
  return 0;
}


int
todo_db_rm_each (todo_db_t *db, int (*fn)(char *key, char *value, todo_db_t *db)) {
  int rc = -1;
  int i = 0, l = 0;
  void *c = sp_cursor(db->dbh, SPGT, NULL, 0);
  const char *ckey;
  const char *cvalue;
  char *keys[512];
  size_t sizes[512] = { };
  size_t size;

  while (sp_fetch(c)) {
    ckey = sp_key(c);
    cvalue = sp_value(c);
    size = sp_keysize(c);
    rc = fn(strdup((char *) ckey), strdup((char *)cvalue), db);
    if (0 == rc) {
      keys[i++] = (char *)ckey;
      sizes[i] = size;
      if (-1 == rc) {
        return rc;
      }
    }
  }


  sp_destroy(c);

  l = i;
  i = 0;
  for (; i < l; ++i) {
    rc = sp_delete(db->dbh, (const char *)keys[i], strlen((const char*) keys[i]));
    if (-1 == rc) {
      return rc;
    }
  }

  return rc;
}


char *
todo_db_reduce (todo_db_t *db, char *(*fn)(char *curkey, char *curvalue, char *nextkey, char *nextvalue, todo_db_t *db)) {
  void *c = sp_cursor(db->dbh, SPGT, NULL, 0);
  char *reduced = NULL;
  char *tmp = "";
  int i = 0;
  char *ckey, *cvalue;
  char *nkey, *nvalue;

  if (NULL == c) {
    return NULL;
  }

  while (sp_fetch(c)) {
    // next value
    if (i++ % 2) {
      nkey = strndup(sp_key(c), sp_keysize(c));
      nvalue = strndup(sp_value(c), sp_valuesize(c));
      reduced = fn(ckey, cvalue, nkey, nvalue, db);
      free(nkey);
      free(nvalue);
    } else {
      ckey =  strdup(sp_key(c));
      cvalue = strdup(sp_value(c));
    }

    if (NULL != reduced) break;
  }

  if (NULL == reduced) {
    reduced = fn(ckey, cvalue, tmp, tmp, db);
  }


  sp_destroy(c);

  return reduced;
}
