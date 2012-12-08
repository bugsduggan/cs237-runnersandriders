/*
 * track
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef TRACK_THL5_H
#define TRACK_THL5_H

typedef struct Track {
  int id;
  int start_node;
  int end_node;
  int safe_time;
} Track;

Vector* tracks_read(FILE* fp);
void tracks_dispose(Vector* tracks);
Track* track_from_id(Vector* tracks, int id);

/* finds a track from two nodes (either direction) */
Track* track_from_nodes(Vector* tracks, Node* start, Node* end);

#endif
