/*
 * course
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>

#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * private functions
 */

/*
 * functions declared in data.h
 */

Course* course_init(char* line) {
  Course* course = malloc(sizeof(Course));

  course->id = '\0';
  course->nodes = NULL;
  course->tracks = NULL;
  int safe_time = -1;

  return course;
}
