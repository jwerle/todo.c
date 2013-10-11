
#ifndef __TODO_CORE_H__
#define __TODO_CORE_H__ 1


/**
 * todo - "core.h"
 *
 * copyright (c) joseph werle
 * MIT license
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>


/**
 * Returns the int representation
 * of todo's version
 */

int
todo_version ();


/**
 * Returns a string representation
 * of todo's version
 */

char *
todo_version_str ();


/**
 * Flattens a char array into a string
 * from a given start index and end index
 */

char *
todo_flatten (char *array[], int start, int end);

/**
 * Version constants
 */

#define TODO_MAJOR_VERSION 0
#define TODO_MINOR_VERSION 0
#define TODO_PATCH_VERSION 1
#define TODO_VERSION todo_version()
#define TODO_VERSION_STR todo_version_str()


/**
 * Prints a string to stderr
 */

#define todo_error(s) fprintf(stderr, "todo: error: %s\n", s);


/**
 * Prints a formatted string to stderr
 */

#define todo_ferror(fmt, ...) { \
  char t[256]; \
  sprintf(t, "error: %s\n", fmt); \
  fprintf(stderr, t, ##__VA_ARGS__); \
}


/**
 * Prints a formatted string to
 * stdout
 */

#define todo_printf(fmt, ...) { \
  char t[256]; \
  sprintf(t, "todo: %s", fmt); \
  printf(t, ##__VA_ARGS__); \
}






#endif
