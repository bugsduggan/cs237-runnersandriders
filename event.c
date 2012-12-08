/*
 * event
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "event.h"
#include "vector.h"
#include "util.h"
#include "node.h"
#include "track.h"
#include "course.h"
#include "entrant.h"

/*
 * function prototypes
 */

/*
 * functions declared in event.h
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

/*
 * private functions
 */
