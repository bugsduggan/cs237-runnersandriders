/*
 * course
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * function prototypes
 */

void course_dispose(void* course);
void course_safe_time(Vector* tracks, Course* course);

/*
 * functions declared in data.h
 */

Vector* courses_read(FILE* fp, Vector* nodes, Vector* tracks) {
  Vector* lines = read_file(fp);
  Vector* courses = Vector_new(sizeof(Course*), course_dispose);
  char* line;
  char* token;
  Node* node;
  int i = 0;
  int j = 0;
  int node_id;

  for (i = 0; i < Vector_size(lines); i++) {
    Course* course = malloc(sizeof(Course));
    Vector_get(lines, i, &line);
    /* read course id */
    token = strtok(line, " ");
    course->id = token[0]; /* should be one char */
    /* read num nodes */
    token = strtok(NULL, " ");
    course->num_nodes = atoi(token);
    /* read nodes */
    /* we're relying on nodes_dispose to do the free-ing */
    course->nodes = Vector_new(sizeof(Node*), NULL);
    for (j = 0; j < course->num_nodes; j++) {
      token = strtok(NULL, " ");
      node_id = atoi(token);
      node = node_from_id(nodes, node_id);
      Vector_add(course->nodes, &node);
    }
    /* calculate the safe completion time */
    course_safe_time(tracks, course);
    Vector_add(courses, &course);
  }
  Vector_dispose(lines);
  return courses;
}

void courses_dispose(Vector* courses) {
  Vector_dispose(courses);
}

Course* course_from_id(Vector* courses, char id) {
  int i = 0;
  Course* course;
  for (i = 0; i < Vector_size(courses); i++) {
    Vector_get(courses, i, &course);
    if (course->id == id) return course;
  }
  return NULL;
}

int course_num_checkpoints(Course* course) {
  int result = 0;
  int i = 0;
  Node* node;

  for (i = 0; i < Vector_size(course->nodes); i++) {
    Vector_get(course->nodes, i, &node);
    if (node->type == CP) result++;
  }
  return result;
}

/*
 * private functions
 */

void course_dispose(void* course) {
  Course* foo = *(Course**) course;
  if (foo) {
    if (foo->nodes) Vector_dispose(foo->nodes);
    free(foo);
  }
}

void course_safe_time(Vector* tracks, Course* course) {
  Node* start;
  Node* end;
  Track* track;
  int i = 0;
  course->safe_time = 0;

  for (i = 0; i < Vector_size(course->nodes) - 1; i++) {
    Vector_get(course->nodes, i, &start);
    Vector_get(course->nodes, i + 1, &end);
    track = track_from_nodes(tracks, start, end);
    course->safe_time += track->safe_time;
  }
}
