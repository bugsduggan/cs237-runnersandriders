/*
 * track
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

void track_dispose(void* track);

/*
 * functions declared in data.h
 */

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

Track* track_from_nodes(Vector* tracks, Node* start, Node* end) {
  Track* track;
  int i = 0;
  for (i = 0; i < Vector_size(tracks); i++) {
    Vector_get(tracks, i, &track);
    if (track->start_node == start->id && track->end_node == end->id)
      return track;
    if (track->start_node == end->id && track->end_node == start->id)
      return track;
  }
  return NULL;
}

/*
 * private functions
 */

void track_dispose(void* track) {
  Track* foo = *(Track**) track;
  if (foo) free(foo);
}
