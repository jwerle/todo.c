
#ifndef __TODO_CONFIG_H__
#define __TODO_CONFIG_H__ 1

#include "todo.h"


/**
 * Gets a configuration value
 * from the current todo scope
 */

char *
todo_config_get (todo_t *todo, char *key);


/**
 * Sets a configuration value
 * in the current todo scope
 */

int
todo_config_set (todo_t *todo, char *key, char *value);


/**
 * Clears the config database
 */

int
todo_config_clear (todo_t *todo);

/**
 * Lists all config variables
 */

void
todo_config_list (todo_t *todo);


/**
 * Removes a specific key from
 * the configuration
 */

int
todo_config_rm (todo_t *todo, char *key);

#endif
