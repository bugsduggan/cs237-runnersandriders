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
  Time* start;
  Vector* courses;
  Vector* entrants;
} Event;

Event* event_init(char* filename);

/*
 * node
 */

typedef enum {
  CP,
  MC,
  JN
} node_type;

typedef struct Node {
  int id;
  node_type type;
} Node;

Node* node_init(char* line);

/*
 * track
 */

typedef struct Track {
  int id;
  Node* start;
  Node* end;
  int safe_time;
} Track;

Track* track_init(char* line);

/*
 * course
 */

typedef struct Course {
  char id;
  Vector* nodes;
  Vector* tracks;
  int safe_time;
} Course;

Course* course_init(char* line);

/*
 * entrant
 */

typedef enum {
  NOT_STARTED,
  STARTED,
  STOPPED,
  FINISHED
} entrant_status;

typedef struct Entrant {
  int id;
  char* name;
  Course* course;
  Time* start_time;
  Node* last_node;
  Time* last_time;
  Track* curr_track;
  entrant_status status;
} Entrant;

Entrant* entrant_init(char* line);

#endif
