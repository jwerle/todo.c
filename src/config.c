
#include "config.h"

static char *
format_key (char *key) {
  char tmp[512];
  char *ckey = NULL;
  sprintf(tmp, "config:%s", key);
  ckey = tmp;
  return ckey;
}


static int
filter_config_keys (char *key, char *value, todo_db_t *db) {
  char *prefix = "config:";
  int len = strlen(prefix);

  if (0 == strncmp(key, prefix, len)) {
    return 0;
  }

  return -1;
}


static void
handle_list (char *key, char *value, todo_db_t *db) {
  char *prefix = "config:";
  char *tmp_key = strdup(key);
  int i = 0;
  int t = 0;
  int len = strlen(prefix);
  if (0 == strncmp(tmp_key, prefix, len)) {
    for (; i < len; ++i) {
      t = *tmp_key++;
    }

    printf("%s = %s\n", tmp_key, value);
  }
}

char *
todo_config_get (todo_t *todo, char *key) {
  char *ckey = format_key(key);
  char *value = NULL;
  value = todo_db_get(todo->db, ckey);
  return value;
}


int
todo_config_set (todo_t *todo, char *key, char *value) {
  char *ckey = format_key(key);
  return todo_db_set(todo->db, ckey, value);
}


int
todo_config_clear (todo_t *todo) {
  return todo_db_rm_each(todo->db, filter_config_keys);
}


void
todo_config_list (todo_t *todo) {
  todo_db_foreach(todo->db, handle_list);
}


int
todo_config_rm (todo_t *todo, char *key) {
  char *prefix = "config:";
  char *tmp_key = malloc(strlen(prefix) + strlen(key) + 1);
  char *ckey;
  int size;
  int rc = 0;
  int ksize;
  void *c = sp_cursor(todo->db->dbh, SPGT, NULL, 0);

  sprintf(tmp_key, "%s%s", prefix, key);
  ksize = (int) strlen(tmp_key);

  while (sp_fetch(c)) {
    ckey = (char *)sp_key(c);
    size = (int) strlen(ckey);
    if (ksize == size -1 && '\0' == tmp_key[size] && 0 == strncmp(ckey, tmp_key, size -1)) {
      size = sp_keysize(c);
      break;
    }

  }

  sp_destroy(c);
  rc = sp_delete(todo->db->dbh, (const char *)ckey, size);
  // free(ckey);
  free(tmp_key);
  return rc;
}


