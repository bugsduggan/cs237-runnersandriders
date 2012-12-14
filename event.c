
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * functions declared in data.h
 */

/* read an event from file
 * doesn't initialise everything! */
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

/* update the time in event and the entrants
 * does not update the entrant identified by entrant_id,
 * this will be done by a subsequent call to entrant_update_location */
void update_time(Event* event, Time* time, int entrant_id) {
  Entrant* entrant;
  int i = 0;

  free(event->time);
  event->time = timecpy(time);

  /* now update the entrants */
  for (i = 0; i < Vector_size(event->entrants); i++) {
    Vector_get(event->entrants, i, &entrant);
    if (entrant->id != entrant_id)
      entrant_update_time(event, entrant);
  }
}