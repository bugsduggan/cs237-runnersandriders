/*
 * data
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef DATA_THL5_H
#define DATA_THL5_H

/*
 * event
 */

typedef struct Event {
  char* title;
  char* date;
  int start_hrs;
  int start_mins;
  Vector* nodes;
  Vector* tracks;
  Vector* courses;
  Vector* entrants;
} Event;

Event* event_read(FILE* fp);
void event_dispose(Event* event);

/*
 * node
 */

typedef enum {CP, MC, JN} node_type;

typedef struct Node {
  int id;
  node_type type;
} Node;

Vector* nodes_read(FILE* fp);
void nodes_dispose(Vector* nodes);
Node* node_from_id(Vector* nodes, int id);

/*
 * track
 */

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

/*
 * course
 */

typedef struct Course {
  char id;
  int num_nodes;
  Vector* nodes;
  int safe_time;
} Course;

/* requires nodes and tracks */
Vector* courses_read(FILE* fp, Vector* nodes, Vector* tracks);
void courses_dispose(Vector* courses);
Course* course_from_id(Vector* courses, char id);

int course_num_checkpoints(Course* course);

/*
 * entrant
 */

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
