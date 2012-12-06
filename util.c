/*
 * util
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "util.h"

char* strdup(const char* str) {
  int n = strlen(str) + 1;
  char* dup = malloc(n);
  if (dup) {
    strcpy(dup, str);
  }
  return dup;
}

char* readline() {
  return NULL;
}

void string_dispose(void* string) {
  char* foo = *(char**) string;
  if (foo) free(foo);
}

Vector* read_file(char* filename) {
  char line[MAX_LINE_LENGTH];
  char* str;
  FILE* fp;
  Vector* lines = Vector_new(sizeof(char**), string_dispose);

  fp = fopen(filename, "r");
  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    strtok(line, "\n"); /* strip newline */
    str = strdup(line); /* make a new pointer */
    Vector_add(lines, &str); /* pass it to the vector */
  }
  fclose(fp);

  return lines;
}
