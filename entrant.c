/*
 * entrant
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

void entrant_dispose(void* entrant);
int compare_entrant(void* v1, void* v2);
int compare_entrant_not_started(Entrant* a, Entrant* b);
int compare_entrant_started(Entrant* a, Entrant* b);
int compare_entrant_finished(Entrant* a, Entrant* b);

/*
 * functions declared in data.h
 */

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

void entrants_sort(Vector* entrants) {
  Vector_sort(entrants, compare_entrant);
}

/*
 * private functions
 */

void entrant_dispose(void* entrant) {
  Entrant* foo = *(Entrant**) entrant;
  if (foo) {
    if (foo->name) free(foo->name);
    free(foo);
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
