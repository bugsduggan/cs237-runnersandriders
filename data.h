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
  Vector* entrants;
} Event;

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

Vector* read_nodes(char* filename);
Node* node_from_id(Vector* nodes, int id);
node_type str_to_type(char* str);

/*
 * track
 */

typedef struct Track {
  int id;
  Node* start;
  Node* end;
  int safe_time;
} Track;

Vector* read_tracks(char* filename, Vector* nodes);
Track* track_from_nodes(Vector* tracks, Node* start, Node* end);

/*
 * course
 */

typedef struct Course {
  char id;
  Vector* nodes;
  Vector* tracks;
  int safe_time;
} Course;

Vector* read_courses(char* filename, Vector* nodes, Vector* tracks);
Course* course_from_id(Vector* courses, char id);

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
  Course* course;
  char* name;
  entrant_status status;
  Time* start_time;
  Node* last_node;
  Time* last_time;
  Track* curr_track;
} Entrant;

Vector* read_entrants(char* filename, Vector* courses);

#endif
