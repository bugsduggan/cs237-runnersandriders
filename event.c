/*
 * event
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

Event* read_event(char* filename) {
  Vector* lines = read_file(filename);
  Event* event = malloc(sizeof(Event));
  char* line;
  char* token;

  Vector_get(lines, 0, &line);
  token = strtok(line, "\n");
  event->title = strdup(token);

  Vector_get(lines, 1, &line);
  token = strtok(line, "\n");
  event->date = strdup(token);

  Vector_get(lines, 2, &line);
  event->start = str_to_time(line);

  event->time = timecpy(event->start);

  Vector_dispose(lines);
  return event;
}

void update_time(Event* event, Time* time) {
  Entrant* entrant;
  int i = 0;

  free(event->time);
  event->time = timecpy(time);
  for (i = 0; i < Vector_size(event->entrants); i++) {
    Vector_get(event->entrants, i, &entrant);
    entrant_update_time(event, entrant);
  }
}
