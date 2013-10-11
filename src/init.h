
#ifndef __TODO_INIT_H__
#define __TODO_INIT_H__ 1


/**
 * todo - "init.h"
 *
 * copyright (c) joseph werle
 * MIT license
 */


#include <stdlib.h>
#include "db.h"


typedef struct todo {
  todo_db_t *db;
  char *path;
  char *config_path;
  int is_fresh;
} todo_t;


todo_t *
todo_init (char *path);


#endif
