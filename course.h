/*
 * course
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef COURSE_THL5_H
#define COURSE_THL5_H

typedef struct Course {
  char id;
  int num_nodes;
  Vector* nodes;
  int safe_time;
} Course;

/* requires nodes and tracks */
Vector* courses_read(FILE* fp, Vector* nodes, Vector* tracks);
void courses_dispose(Vector* courses);
Course* course_from_id(Vector* courses, char id);

int course_num_checkpoints(Course* course);

#endif
