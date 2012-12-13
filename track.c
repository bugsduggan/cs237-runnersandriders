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
 * functions declared in data.h
 */

/* read tracks from the file */
Vector* read_tracks(char* filename, Vector* nodes) {
  Vector* lines = read_file(filename);
  Vector* tracks = Vector_new(sizeof(Track*), NULL);
  Track* track_for;
  Track* track_back;
  Node* start_node;
  Node* end_node;
  char* line;
  char* token;
  int node_id;
  int i = 0;

  /*
   * Tracks are being added as a pair, one forward and one backward
   */

  for (i = 0; i < Vector_size(lines); i++) {
    Vector_get(lines, i, &line);
    track_for = malloc(sizeof(Track));
    track_back = malloc(sizeof(Track));

    /* id */
    token = strtok(line, " ");
    track_for->id = atoi(token);
    track_back->id = atoi(token);

    /* nodes */
    token = strtok(NULL, " ");
    node_id = atoi(token);
    start_node = node_from_id(nodes, node_id);

    token = strtok(NULL, " ");
    node_id = atoi(token);
    end_node = node_from_id(nodes, node_id);

    track_for->start = start_node;
    track_for->end = end_node;
    track_back->start = end_node;
    track_back->end = start_node;

    /* safe time */
    token = strtok(NULL, " ");
    track_for->safe_time = atoi(token);
    track_back->safe_time = atoi(token);

    Vector_add(tracks, &track_for);
    Vector_add(tracks, &track_back);
  }

  Vector_dispose(lines);
  return tracks;
}

/* find a track from start node to end node */
Track* track_from_nodes(Vector* tracks, Node* start, Node* end) {
  Track* track;
  int i = 0;

  for (i = 0; i < Vector_size(tracks); i++) {
    Vector_get(tracks, i, &track);
    if (track->start == start && track->end == end) return track;
  }

  return NULL;
}
