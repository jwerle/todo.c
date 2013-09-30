
#ifndef __TODO_H__
#define __TODO_H__ 1

#include "core.h"
#include <sophia.h>


typedef struct todo_db {
  void *env;
} todo_db_t;


/**
 * Alloocates a new `todo_db_t *` pointer
 */

todo_db_t *
todo_db_new (char path);


#endif
