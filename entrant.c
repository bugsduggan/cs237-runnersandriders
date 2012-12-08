/*
 * entrant
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
