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
  Track* foo = *(Track**) track;
  if (foo) free(foo);
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
  Course* foo = *(Course**) course;
  if (foo) {
    if (foo->nodes) Vector_dispose(foo->nodes);
    free(foo);
  }
}

Vector* courses_read(char* filename) {
  Vector* lines = read_file(filename);
  Vector* courses = Vector_new(sizeof(Course*), course_dispose);
  char* line;
  char* token;
  int i = 0;
  int j = 0;
  int n = 0;

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
    course->nodes = Vector_new(sizeof(int), NULL);
    for (j = 0; j < course->num_nodes; j++) {
      token = strtok(NULL, " ");
      n = atoi(token);
      Vector_add(course->nodes, &n);
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
  Entrant* foo = *(Entrant**) entrant;
  if (foo) {
    if (foo->name) free(foo->name);
    free(foo);
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
    /* init other values */
    entrant->status = NOT_STARTED;
    entrant->last_seen = -1;
    entrant->duration = 0;
    Vector_add(entrants, &entrant);
  }
  Vector_dispose(lines);
  return entrants;
}

void entrants_dispose(Vector* entrants) {
  Vector_dispose(entrants);
}

void entrant_update_location(Event* event, int entrant_id, int node_id, int hrs, int mins) {
  Entrant* entrant;
  Course* course;
  Node* node; /* the last node of the entrant's course */
  int i = 0;

  for (i = 0; i < Vector_size(event->entrants); i++) {
    Vector_get(event->entrants, i, &entrant);
    if (entrant->id == entrant_id) break;
  }

  if (entrant->status == NOT_STARTED) entrant->status = STARTED;
  entrant->last_seen = node_id;
  entrant->duration = time_to_duration(hrs, mins)
    - time_to_duration(event->start_hrs, event->start_mins);

  /* check if entrant has finished */
  for (i = 0; i < Vector_size(event->courses); i++) {
    Vector_get(event->courses, i, &course);
    if (entrant->course_id == course->id) {
      Vector_get_last(course->nodes, &node);
      if (node->id == node_id) entrant->status = FINISHED;
    }
  }
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
