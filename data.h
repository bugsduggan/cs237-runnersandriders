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

/* event data structures */
typedef struct Event {
  char* title;
  char* date;
  int start_hrs;
  int start_mins;
  Node_list* nodes;
  Track_list* tracks;
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

/* event functions */
Event* make_event(char* filename);
void Event_destroy(Event* event);

#endif
