/*
 * data
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
 * Node stuff
 */

void node_dispose(void* node) {
  Node* foo = *(Node**) node;
  if (foo) free(foo);
}

Vector* nodes_read(FILE* fp) {
  Vector* lines = read_file(fp);
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

Node* node_from_id(Vector* nodes, int id) {
  int i = 0;
  Node* node;
  for (i = 0; i < Vector_size(nodes); i++) {
    Vector_get(nodes, i, &node);
    if (node->id == id) return node;
  }
  return NULL;
}

/*
 * Track stuff
 */

void track_dispose(void* track) {
  Track* foo = *(Track**) track;
  if (foo) free(foo);
}

Vector* tracks_read(FILE* fp) {
  Vector* lines = read_file(fp);
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

Track* track_from_id(Vector* tracks, int id) {
  int i = 0;
  Track* track;
  for (i = 0; i < Vector_size(tracks); i++) {
    Vector_get(tracks, i, &track);
    if (track->id == id) return track;
  }
  return NULL;
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

Vector* courses_read(FILE* fp, Vector* nodes) {
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
    /* pass NULL for the dispose function, we're relying on nodes_dispose to do the work */
    course->nodes = Vector_new(sizeof(Node*), NULL);
    for (j = 0; j < course->num_nodes; j++) {
      token = strtok(NULL, " ");
      node_id = atoi(token);
      node = node_from_id(nodes, node_id);
      Vector_add(course->nodes, &node);
    }
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
 * Entrant stuff
 */

void entrant_dispose(void* entrant) {
  Entrant* foo = *(Entrant**) entrant;
  if (foo) {
    if (foo->name) free(foo->name);
    free(foo);
  }
}

Vector* entrants_read(FILE* fp) {
  Vector* lines = read_file(fp);
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
    entrant->nodes_visited = 0;
    entrant->last_seen = -1;
    entrant->start_hrs = -1;
    entrant->start_mins = -1;
    entrant->end_hrs = -1;
    entrant->end_mins = -1;
    entrant->duration = 0;
    Vector_add(entrants, &entrant);
  }
  Vector_dispose(lines);
  return entrants;
}

void entrants_dispose(Vector* entrants) {
  Vector_dispose(entrants);
}

Entrant* entrant_from_id(Vector* entrants, int id) {
  int i = 0;
  Entrant* entrant;
  for (i = 0; i < Vector_size(entrants); i++) {
    Vector_get(entrants, i, &entrant);
    if (entrant->id == id) return entrant;
  }
  return NULL;
}

void entrant_update_location(Event* event, Entrant* entrant, int node_id, int hrs, int mins) {
  Course* course;

  /* update his/her location */
  entrant->nodes_visited++;
  entrant->last_seen = node_id;

  /* check if entrant is starting */
  if (entrant->status == NOT_STARTED) {
    entrant->start_hrs = hrs;
    entrant->start_mins = mins;
    entrant->status = STARTED;
  }

  entrant_update_time(entrant, hrs, mins);

  /* check if entrant has finished */
  course = course_from_id(event->courses, entrant->course_id);
  if (entrant->nodes_visited == course_num_checkpoints(course)) {
    entrant->end_hrs = hrs;
    entrant->end_mins = mins;
    entrant->status = FINISHED;
  }
}

void entrant_update_time(Entrant* entrant, int hrs, int mins) {
  if (entrant->status == STARTED) {
    entrant->duration = time_to_duration(hrs, mins) - 
      time_to_duration(entrant->start_hrs, entrant->start_mins);
  }
}

int compare_entrant_not_started(Entrant* a, Entrant* b) {
  if (a->id > b->id) {
    return 1;
  } else if (a->id < b->id) {
    return -1;
  } else {
    return 0;
  }
}

int compare_entrant_started(Entrant* a, Entrant* b) {
  /*
   * this should compare based on which entrant is closest to the expected course
   * duration but for now, I'm just going to use duration
   */
  if (a->duration > b->duration) {
    return -1;
  } else if (a->duration < b->duration) {
    return 1;
  } else {
    return 0;
  }
}

int compare_entrant_finished(Entrant* a, Entrant* b) {
  if (a->duration > b->duration) {
    return 1;
  } else if (a->duration < b->duration) {
    return -1;
  } else {
    return 0;
  }
}

/* a < b = -1; a == b = 0; a > b = 1 */
int compare_entrant(void* v1, void* v2) {
  Entrant* a = *(Entrant**) v1;
  Entrant* b = *(Entrant**) v2;

  /*
   * The plan is to put finished entrants at the top
   * shortest duration first
   *
   * Then started entrants
   * closest to the finish at the top
   *
   * Then not started
   * sorted by id
   */
  if (a->status == NOT_STARTED) {
    if (b->status == NOT_STARTED) {
      return compare_entrant_not_started(a, b);
    } else {
      return 1;
    }
  } else if (a->status == FINISHED) {
    if (b->status == FINISHED) {
      return compare_entrant_finished(a, b);
    } else {
      return -1;
    }
  } else { /* a->status = STARTED */
    if (b->status == STARTED) {
      return compare_entrant_started(a, b);
    } else if (b->status == NOT_STARTED) {
      return -1;
    } else { /* b->status = FINISHED */
      return 1;
    }
  }
}

void entrants_sort(Vector* entrants) {
  Vector_sort(entrants, compare_entrant);
}

/*
 * Event stuff
 */

Event* event_read(FILE* fp) {
  Vector* lines = read_file(fp);
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
