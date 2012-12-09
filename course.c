/*
 * course
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * private functions
 */

/*
 * functions declared in data.h
 */

Vector* read_courses(char* filename, Vector* nodes, Vector* tracks) {
  Vector* lines = read_file(filename);
  Vector* courses = Vector_new(sizeof(Course*), NULL);
  Course* course;
  Node* node;
  Node* start;
  Node* end;
  Track* track;
  char* line;
  char* token;
  int num_nodes;
  int node_id;
  int i, j = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Vector_get(lines, i, &line);
    course = malloc(sizeof(Course));

    /* id */
    token = strtok(line, " ");
    course->id = token[0]; /* 1 char */

    /* nodes */
    course->nodes = Vector_new(sizeof(Node*), NULL);
    token = strtok(NULL, " ");
    num_nodes = atoi(token);
    for (j = 0; j < num_nodes; j++) {
      token = strtok(NULL, " ");
      node_id = atoi(token);

      node = node_from_id(nodes, node_id);
      Vector_add(course->nodes, &node);
    }

    /* tracks + safe_time */
    course->tracks = Vector_new(sizeof(Track*), NULL);
    course->safe_time = 0;
    for (j = 0; j < num_nodes - 1; j++) { /* num tracks = num_nodes - 1 */
      Vector_get(course->nodes, j, &start);
      Vector_get(course->nodes, j + 1, &end);

      track = track_from_nodes(tracks, start, end);
      Vector_add(course->tracks, &track);

      course->safe_time += track->safe_time;
    }

    Vector_add(courses, &course);
  }

  Vector_dispose(lines);
  return courses;
}

Course* course_from_id(Vector* courses, char id) {
  Course* course;
  int i = 0;

  for (i = 0; i < Vector_size(courses); i++) {
    Vector_get(courses, i, &course);
    if (course->id == id) return course;
  }

  return NULL;
}


Track* next_track(Course* course, Track* current) {
  Track* track;
  Track* next;
  int i = 0;

  /* start one beyond the start of the vector so we don't overrun */
  for (i = 1; i < Vector_size(course->tracks); i++) {
    Vector_get(course->tracks, i - 1, &track);
    if (track == current) {
      Vector_get(course->tracks, i, &next);
      return next;
    }
  }

  return NULL;
}
