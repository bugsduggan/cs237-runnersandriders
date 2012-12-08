/*
 * track.c
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

Track* track_init(char* line) {
  Track* track = malloc(sizeof(Track*));

  track->id = 0;
  track->start = NULL;
  track->end = NULL;
  track->safe_time = 0;

  return track;
}
