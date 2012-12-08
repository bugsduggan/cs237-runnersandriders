/*
 * entrant
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef ENTRANT_THL5_H
#define ENTRANT_THL5_H

typedef enum {
  NOT_STARTED,
  STARTED,
  FINISHED
} entrant_status;

typedef struct Entrant {
  int id;
  char course_id;
  char* name;
  entrant_status status;
  int nodes_visited;
  int last_seen;
  int start_hrs;
  int start_mins;
  int end_hrs;
  int end_mins;
  int duration;
} Entrant;

Vector* entrants_read(FILE* fp);
void entrants_dispose(Vector* entrants);
Entrant* entrant_from_id(Vector* entrants, int id);

void entrant_update_location(Event* event, Entrant* entrant,
    int node_id, int hrs, int mins);
void entrant_update_time(Entrant* entrant, int hrs, int mins);
void entrants_sort(Vector* entrants);

#endif
