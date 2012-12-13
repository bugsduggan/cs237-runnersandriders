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

/* 'borrowed' functions */
char* strdup(const char* str);      /* returns a pointer to a newly malloc'd string */
char* readline();                   /* reads a line from stdin */

int valid_filename(char* filename); /* checks if a filename is valid */
Vector* read_file(char* filename);  /* reads a whole file into a vector */
Time* str_to_time(char* str);       /* produces a time from a string (hh:mm) */
Time* timecpy(Time* time);          /* deep copy a time struct */
int time_to_duration(Time* time);   /* turn hh:mm into mm */

#endif
