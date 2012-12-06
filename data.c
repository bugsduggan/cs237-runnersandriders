/*
 * data
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * Node stuff
 */

void node_dispose(void* node) {
  Node* foo = *(Node**) node;
  if (foo) free(foo);
}

Vector* nodes_read(char* filename) {
  Vector* lines = read_file(filename);
  Vector* nodes = Vector_new(sizeof(Node*), node_dispose);
  char* line;
  char* token;
  int i = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Node* node = malloc(sizeof(Node));
    Vector_get(lines, i, &line);
    /* read node id */
    token = strtok(line, " ");
    node->id = atoi(token);
    /* read node type */
    token = strtok(NULL, " ");
    if (strcmp(token, "CP") == 0) {
      node->type = CP;
    } else if (strcmp(token, "MC") == 0) {
      node->type = MC;
    } else {
      node->type = JN;
    }
    Vector_add(nodes, &node);
  }
  Vector_dispose(lines);
  return nodes;
}

void nodes_dispose(Vector* nodes) {
  Vector_dispose(nodes);
}

/*
 * Track stuff
 */

void track_dispose(void* track) {
  Track* track = *(Track**) track;
  if (track) free(track);
}

Vector* tracks_read(char* filename) {
  Vector* lines = read_file(filename);
  Vector* tracks = Vector_new(sizeof(Track*), track_dispose);
  char* line;
  char* token;
  int i = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Track* track = malloc(sizeof(Track));
    Vector_get(lines, i, &line);
    /* read track id */
    token = strtok(line, " ");
    track->id = atoi(token);
    /* read start node */
    token = strtok(NULL, " ");
    track->start_node = atoi(token);
    /* read end node */
    token = strtok(NULL, " ");
    track->end_node = atoi(token);
    /* read safe time */
    token = strtok(NULL, " ");
    track->safe_time = atoi(token);
    Vector_add(tracks, &track);
  }
  Vector_dispose(lines);
  return tracks;
}

void tracks_dispose(Vector* tracks) {
  Vector_dispose(tracks);
}

/*
 * Course stuff
 */

void course_dispose(void* course) {
  Course* course = *(Course**) course;
  if (course) {
    if (course->nodes) free(course->nodes);
    free(course);
  }
}

Vector* courses_read(char* filename) {
  Vector* lines = read_file(filename);
  Vector* courses = Vector_new(sizeof(Course*), course_dispose);
  char* line;
  char* token;
  int i = 0;
  int j = 0;

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
    course->nodes = int[course->num_nodes];
    for (j = 0; j < course->num_nodes; j++) {
      token = strtok(NULL, " ");
      course->nodes[j] = atoi(token);
    }
    Vector_add(courses, &course);
  }
  Vector_dispose(lines);
  return courses;
}

void courses_dispose(Vector* courses) {
  Vector_dispose(courses);
}

/*
 * Entrant stuff
 */

void entrant_dispose(void* entrant) {
  Entrant* entrant = *(Entrant**) entrant;
  if (entrant) {
    if (entrant->name) free(entrant->name);
    free(entrant);
  }
}

Vector* entrants_read(char* filename) {
  Vector* lines = read_file(filename);
  Vector* entrants = Vector_new(sizeof(Entrant*), entrant_dispose);
  char* line;
  char* token;
  int i = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Entrant* entrant = malloc(sizeof(Entrant));
    Vector_get(lines, i, &line);
    /* read id */
    token = strtok(line, " ");
    entrant->id = atoi(token);
    /* read course id */
    token = strtok(NULL, " ");
    entrant->course_id = token[0]; /* should be one char */
    /* read name */
    token = strtok(NULL, "\n"); /* read to the end of the line */
    entrant->name = strdup(token);
    Vector_add(entrants, &entrant);
  }
  Vector_dispose(lines);
  return entrants;
}

void entrants_dispose(Vector* entrants) {
  Vector_dispose(entrants);
}

/*
 * Event stuff
 */

Event* event_read(char* filename) {
  Vector* lines = read_file(filename);
  Event* event = malloc(sizeof(Event));
  char* line;
  char* token;

  /* read event title */
  Vector_get(lines, 0, &line);
  event->title = strdup(line);
  /* read event date */
  Vector_get(lines, 1, &line);
  event->date = strdup(line);
  /* read event time */
  Vector_get(lines, 2, &line);
  token = strtok(line, ":");
  event->start_hrs = atoi(token);
  token = strtok(NULL, ":");
  event->start_mins = atoi(token);
  /* initialise other data */
  event->nodes = NULL;
  event->tracks = NULL;
  event->courses = NULL;
  event->entrants = NULL;

  Vector_dispose(lines);
  return event;
}

void event_dispose(Event* event) {
  if (event) {
    if (event->title) free(event->title);
    if (event->date) free(event->date);
    if (event->nodes) nodes_dispose(event->nodes);
    if (event->tracks) tracks_dispose(event->tracks);
    if (event->courses) courses_dispose(event->courses);
    if (event->entrants) entrants_dispose(event->entrants);
    free(event);
  }
}
