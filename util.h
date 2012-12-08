/*
 * util
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef UTIL_THL5_H
#define UTIL_THL5_H

#define MAX_LINE_LENGTH 80

typedef struct Time {
  int hours;
  int minutes;
} Time;

char* strdup(const char* str); /* returns a pointer to a newly malloc'd string */
char* readline(); /* reads a line from stdin */
Vector* read_file(char* filename); /* reads a whole file into a vector */

#endif
