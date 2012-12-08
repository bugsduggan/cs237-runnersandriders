/*
 * entrant
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

Entrant* entrant_init(char* line) {
  Entrant* entrant = malloc(sizeof(Entrant));

  entrant->id = 0;
  entrant->name = NULL;
  entrant->course = NULL;
  entrant->start_time = NULL;
  entrant->last_node = NULL;
  entrant->last_time = NULL;
  entrant->curr_track = NULL;
  entrant->status = NOT_STARTED;

  return entrant;
}
