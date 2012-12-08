/*
 * event
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>

#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * private functions
 */

/*
 * functions declared in data.h
 */

Event* event_init(char* filename) {
  Event* event = malloc(sizeof(Event));

  event->title = NULL;
  event->date = NULL;
  event->start = NULL;
  event->courses = NULL;
  event->entrants = NULL;

  return event;
}
