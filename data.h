/*
 * data
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef DATA_THL5_H
#define DATA_THL5_H

/*
 * Data structures
 */

/* Node stuff */
typedef enum {CP, MC, JN} node_type;

typedef struct Node {
  int id;
  node_type type;
} Node;

/* Track stuff */
typedef struct Track {
  int id;
  int start_node;
  int end_node;
  int safe_time;
} Track;

/* Course stuff */
typedef struct Course {
  char id;
  int num_nodes;
  Vector* nodes;
} Course;

/* Entrant stuff */
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
  int last_seen; /* last node visited */
  int duration;
} Entrant;

/* Event stuff */
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

/*
 * Functions
 */

/* Node stuff */
Vector* nodes_read(FILE* fp); /* populates vector with pointers to nodes */
void nodes_dispose(Vector* nodes); /* disposes of a vector of nodes */
Node* node_from_id(Vector* nodes, int id); /* returns the node with the given id (NULL if not found) */

/* Track stuff */
Vector* tracks_read(FILE* fp);
void tracks_dispose(Vector* tracks);
Track* track_from_id(Vector* tracks, int id);

/* Course stuff */
Vector* courses_read(FILE* fp, Vector* nodes); /* requires nodes to link */
void courses_dispose(Vector* courses);
Course* course_from_id(Vector* courses, char id);

/* Entrant stuff */
Vector* entrants_read(FILE* fp);
void entrants_dispose(Vector* entrants);
Entrant* entrant_from_id(Vector* entrants, int id);
void entrant_update_location(Event* event, int entrant_id, int node_id, int hrs, int mins);
void entrants_sort(Vector* entrants);

/* Event stuff */
Event* event_read(FILE* fp);
void event_dispose(Event* event);

#endif
