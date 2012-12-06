/*
 * util
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef UTIL_THL5_H
#define UTIL_THL5_H

#define MAX_LINE_LENGTH 80

char* strdup(const char* str); /* returns a pointer to a newly malloc'd string */
Vector* read_file(char* filename); /* reads a whole file into a vector */

#endif
