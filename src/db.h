
#ifndef __TODO_DB_H__
#define __TODO_DB_H__ 1

#include "core.h"
#include <sophia.h>


typedef struct todo_db {
  void *env;
  void *dbh;
  char *path;
} todo_db_t;


/**
 * Alloocates a new `todo_db_t *` pointer
 */

todo_db_t *
todo_db_new (char *path);


/**
 * Opens a database. Returns `-1`
 * on failure and `0` on success
 */

int
todo_db_open (todo_db_t *db);


/**
 * Sets data to a database
 */

int
todo_db_set (todo_db_t *db, char *key, char *value);


/**
 * Gets data to a database
 */

char *
todo_db_get (todo_db_t *db, char *key);


/**
 * Iterates over each key in database
 */


int
todo_db_foreach (todo_db_t *db, void (*fn)(char *key, char *value, todo_db_t *db));


/**
 * Iterates and returns a reduced value
 */

char *
todo_db_reduce (todo_db_t *db, char *(*fn)(char *curkey, char *curvalue,
                                           char *nextkey, char *nextvalue, todo_db_t *db));


/**
 * Iterates over each key in database
 * and removes the key if a 0 is returned by the
 * callback
 */

int
todo_db_rm_each (todo_db_t *db, int (*fn)(char *key, char *value, todo_db_t *db));


#endif
