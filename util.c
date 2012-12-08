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

/*
 * function prototypes
 */

void string_dispose(void* string);

/*
 * functions declared in util.h
 */

/*
 * This function was taken from
 * http://cboard.cprogramming.com/c-programming/95462-compiler-error-warning-implicit-declaration-function-strdup.html
 * as it's not part of the c89 standard 
 */
char* strdup(const char* str) {
  int n = strlen(str) + 1;
  char* dup = malloc(n);
  if (dup) {
    strcpy(dup, str);
  }
  return dup;
}

/*
 * The following function was taken from
 * http://stackoverflow.com/questions/314401/how-to-read-a-line-from-the-console-in-c
 * I just couldn't get scanf to play nicely and this seems to be more robust code
 */
char* readline() {
  char* line = malloc(100), *linep = line;
  size_t lenmax = 100, len = lenmax;
  int c;

  if(line == NULL)
    return NULL;

  for(;;) {
    c = fgetc(stdin);
    if(c == EOF)
      break;

    if(--len == 0) {
      len = lenmax;
      char * linen = realloc(linep, lenmax *= 2);

      if(linen == NULL) {
        free(linep);
        return NULL;
      }
      line = linen + (line - linep);
      linep = linen;
    }

    if((*line++ = c) == '\n')
      break;
  }
  *line = '\0';
  strtok(linep, "\n"); /* this line is mine, just to strip the newline */
  return linep;
}

Vector* read_file(char* filename) {
  char line[MAX_LINE_LENGTH];
  char* str;
  FILE* fp
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

/*
 * private functions
 */

void string_dispose(void* string) {
  char* foo = *(char**) string;
  if (foo) free(foo);
}
