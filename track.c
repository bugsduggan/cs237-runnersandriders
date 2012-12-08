/*
 * track.c
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

Vector* read_tracks(char* filename, Vector* nodes) {
  Vector* lines = read_file(filename);
  Vector* tracks = Vector_new(sizeof(Track*), NULL);
  Track* track;
  char* line;
  char* token;
  int node_id;
  int i = 0;

  /*
   * It's worth noting that at this point, the start and end nodes
   * are set fairly arbitrarily. Once the track becomes part of a
   * course, the nodes should be set in the correct, logical order
   */

  for (i = 0; i < Vector_size(lines); i++) {
    Vector_get(lines, i, &line);
    track = malloc(sizeof(Track));

    /* id */
    token = strtok(line, " ");
    track->id = atoi(token);

    /* start node */
    token = strtok(NULL, " ");
    node_id = atoi(token);
    track->start = node_from_id(nodes, node_id);

    /* end node */
    token = strtok(NULL, " ");
    node_id = atoi(token);
    track->end = node_from_id(nodes, node_id);

    /* safe time */
    token = strtok(NULL, " ");
    track->safe_time = atoi(token);

    Vector_add(tracks, &track);
  }

  Vector_dispose(lines);
  return tracks;
}
