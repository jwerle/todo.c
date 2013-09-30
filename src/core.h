
#ifndef __TODO_COMMON_H__
#define __TODO_COMMON_H__ 1


/**
 * todo
 *
 * copyright (c) joseph werle
 * BSD license
 */


#define TODO_MAJOR_VERSION 0
#define TODO_MINOR_VERSION 0
#define TODO_PATCH_VERSION 1

#include <unistd.h> // getlogin(), getcwd(), getuid()
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>


/**
 * Prints a string to stderr
 */

#define todo_error(s) fprintf(stderr, "todo: error: %s\n", s);


/**
 * Prints a formatted string to stderr
 */

#define todo_ferror(fmt, ...)                                   \
  char t[256];                                                  \
  sprintf(t, "todo: error: %s\n", fmt);                         \
  fprintf(stderr, t, ##__VA_ARGS__);



#define repo_printf(fmt, ...)                                   \
  char t[256];                                                  \
  sprintf(t, "todo: %s", fmt);                                  \
  printf(t, ##__VA_ARGS__);


#endif