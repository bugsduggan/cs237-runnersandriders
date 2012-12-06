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
  int nodes[];
} Course;

/* Entrant stuff */
typedef struct Entrant {
  int id;
  char course_id;
  char* name;
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
}

/*
 * Functions
 */

/* Node stuff */
Vector* nodes_read(char* filename); /* populates vector with pointers to nodes */
void nodes_dispose(Vector* nodes); /* disposes of a vector of nodes */

/* Track stuff */
Vector* tracks_read(char* filename);
void tracks_dispose(Vector* tracks);

/* Course stuff */
Vector* courses_read(char* filename);
void courses_dispose(Vector* courses);

/* Entrant stuff */
Vector* entrants_read(char* filename);
void entrants_dispose(Vector* entrants);

/* Event stuff */
Event* event_read(char* filename);
void event_dispose(Event* event);

#endif
