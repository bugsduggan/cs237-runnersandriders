/*
 * entrant
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * private functions
 */

char* status_to_str(entrant_status status) {
  if (status == NOT_STARTED) {
    return "Waiting to start";
  } else if (status == STARTED) {
    return "Started";
  } else if (status == STOPPED) {
    return "At medical checkpoint";
  } else {
    return "Finished";
  }
}

/*
 * functions declared in data.h
 */

Vector* read_entrants(char* filename, Vector* courses) {
  Vector* lines = read_file(filename);
  Vector* entrants = Vector_new(sizeof(Entrant*), NULL);
  Entrant* entrant;
  char* line;
  char* token;
  int i = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Vector_get(lines, i, &line);
    entrant = malloc(sizeof(Entrant));

    /* id */
    token = strtok(line, " ");
    entrant->id = atoi(token);

    /* course */
    token = strtok(NULL, " ");
    entrant->course = course_from_id(courses, token[0]); /* 1 char */

    /* name */
    token = strtok(NULL, "\n"); /* read to end of line */
    entrant->name = strdup(token);

    /* other data */
    entrant->status = NOT_STARTED;
    entrant->start_time = NULL;
    entrant->last_node = NULL;
    entrant->last_time = NULL;
    entrant->curr_track = NULL;

    Vector_add(entrants, &entrant);
  }

  Vector_dispose(lines);
  return entrants;
}

Entrant* entrant_from_id(Vector* entrants, int id) {
  Entrant* entrant;
  int i = 0;

  for (i = 0; i < Vector_size(entrants); i++) {
    Vector_get(entrants, i, &entrant);
    if (entrant->id == id) return entrant;
  }

  return NULL;
}

int entrant_duration(Entrant* entrant) {
  return 0;
}

void entrant_stats(Entrant* entrant) {
  printf("\n");
  printf("\t%02d: %-50s\n", entrant->id, entrant->name);
  printf("\t\tCourse: %c\n", entrant->course->id);
  printf("\t\tStatus: %s\n", status_to_str(entrant->status));
  if (entrant->status == STARTED) {
    printf("\n\t\tLast seen at node: %2d @ %02d:%02d\n", entrant->last_node->id,
        entrant->last_time->hours, entrant->last_time->minutes);
    printf("\t\tEstimated location: track %2d\n", entrant->curr_track->id);
  } else if (entrant->status == STOPPED) {
    printf("\n\t\tAt medical checkpoint: %2d since %02d:%02d\n", entrant->last_node->id,
        entrant->last_time->hours, entrant->last_time->minutes);
  } else if (entrant->status == FINISHED) {
    printf("\n\t\tFinished. Run time: %3d\n", entrant_duration(entrant));
  }
}

void entrant_update(Event* event, int entrant_id, int node_id) {

}
