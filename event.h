/*
 * event
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef EVENT_THL5_H
#define EVENT_THL5_H

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

#endif
