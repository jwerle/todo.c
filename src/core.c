
#include "core.h"

int
todo_version () {
  return TODO_MAJOR_VERSION << 0x10 | TODO_MINOR_VERSION << 0x8 | TODO_PATCH_VERSION;
}

char *
todo_version_str () {
  char *str = malloc(sizeof(char) * 256);
  sprintf(str, "%d.%d.%d"
      , TODO_MAJOR_VERSION
      , TODO_MINOR_VERSION
      , TODO_PATCH_VERSION
  );

  return str;
}


char *
todo_flatten (char *array[], int start, int end) {
  char *str = malloc(sizeof(char) * (start + end));
  int i = start;
  strcat(str, "");
  for (; i < end; ++i) {
    strcat(str, array[i]);
    strcat(str, " ");
    if (NULL == array[i]) {
      break;
    }
  }


  return str;
}
