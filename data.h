#ifndef DATA_H
#define DATA_H

/* node data structures */
typedef enum {CP, MC, JN} node_type;

typedef struct Node {
  int id;
  node_type type;
  struct Node* next;
} Node;

typedef struct Node_list {
  Node* head;
} Node_list;

/* track data structures */
typedef struct Track {
  int id;
  int start_node;
  int end_node;
  int safe_time;
  struct Track* next;
} Track;

typedef struct Track_list {
  Track* head;
} Track_list;

/* course data structures */
typedef struct Course {
  char id;
  int n_nodes;
  int* nodes; /* an array of node IDs which make up this course */
  struct Course* next;
} Course;

typedef struct Course_list {
  Course* head;
} Course_list;

/* entrant data structures */
typedef enum {NOT_STARTED, STARTED, FINISHED} entrant_status;

typedef struct Entrant {
  int id;
  char course_id;
  char* name;
  entrant_status status;
  int total_time;
  int last_checkpoint;
  struct Entrant* next;
} Entrant;

typedef struct Entrant_list {
  Entrant* head;
} Entrant_list;

/* event data structures */
typedef struct Event {
  char* title;
  char* date;
  int start_hrs;
  int start_mins;
  Node_list* nodes;
  Track_list* tracks;
  Course_list* courses;
  Entrant_list* entrants;
} Event;

/*
 * function prototypes
 */

/* node functions */
Node_list* make_nodes(char* filename);
void Node_destroy(Node_list* list);

/* track functions */
Track_list* make_tracks(char* filename);
void Track_destroy(Track_list* list);

/* course functions */
Course_list* make_courses(char* filename);
void Course_destroy(Course_list* list);

/* entrant functions */
Entrant_list* make_entrants(char* filename);
void Entrant_destroy(Entrant_list* list);
Entrant* find_entrant(Entrant_list* list, int id);

/* event functions */
Event* make_event(char* filename);
void Event_destroy(Event* event);

#endif
