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
  Vector* nodes;
  Vector* entrants;
  Time* time;
} Event;

Event* read_event(char* filename);
/* will not update entrant_id, that's handled by entrant_update_location */
void update_time(Event* event, Time* time, int entrant_id);

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
Track* next_track(Course* course, Track* current);
/* grabs the next track in the course and accounts for entrants arriving so
 * early that they confuse the track predictions from entrant_update_time */
Track* next_track_from_node(Course* course, Track* current, Node* node);

/*
 * entrant
 */

typedef enum {
  NOT_STARTED,    /* waiting to start */
  STARTED,        /* running */
  STOPPED,        /* at a medical checkpoint */
  DISQUAL_SAFETY, /* disqualified for safety */
  DISQUAL_INCORR, /* disqualified for getting lost */
  FINISHED        /* finished */
} entrant_status;

typedef struct Entrant {
  int id;
  Course* course;
  char* name;

  int duration; /* the run time */
  entrant_status status;
  Time* start_time;
  /* the last timed point */
  Node* last_cp_node;
  Time* last_cp_time;
  /* the assumed location */
  Node* last_node;
  Time* last_time;
  /* the current time and what track we think they're on */
  Time* curr_time;
  Track* curr_track;
} Entrant;

Vector* read_entrants(char* filename, Vector* courses);
Entrant* entrant_from_id(Vector* entrants, int id);
void entrant_stats(Entrant* entrant, Time* curr_time);
/* updates the entrant at a timed or medical checkpoint */
void entrant_update_location(Event* event, char type, int entrant_id, int node_id);
/* updates the running time and predicts which track they're on */
void entrant_update_time(Event* event, Entrant* entrant);
void entrants_sort(Event* event);

#endif
