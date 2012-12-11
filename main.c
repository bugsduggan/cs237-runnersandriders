/*
 * main
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vector.h"
#include "util.h"
#include "data.h"

char* get_filename(char* prompt) {
  char* filename;

  printf(prompt);
  filename = readline();
  while (!valid_filename(filename)) {
    printf("Could not open %s\n", filename);
    printf(prompt);
    free(filename);
    filename = readline();
  }
  return filename;
}

Event* read_data() {
  char* filename;
  Event* event;
  Vector* nodes;
  Vector* tracks;
  Vector* courses;
  Vector* entrants;

  filename = get_filename("Please enter name file: ");
  event = read_event(filename);

  filename = get_filename("Please enter nodes file: ");
  nodes = read_nodes(filename);

  filename = get_filename("Please enter tracks file: ");
  tracks = read_tracks(filename, nodes);

  filename = get_filename("Please enter courses file: ");
  courses = read_courses(filename, nodes, tracks);

  filename = get_filename("Please enter entrants file: ");
  entrants = read_entrants(filename, courses);

  event->entrants = entrants;
  return event;
}

void display_event_header(Event* event) {
  printf("\n\n");
  printf("\t%s\n", event->title);
  printf("\t%s\n", event->date);
  printf("\t%d:%d\n", event->start->hours, event->start->minutes);
  printf("\n");
}

int display_menu(Event* event) {
  char* line;
  char* token;
  int input;
  printf("\n");
  printf("Please select from the following options:\n");
  printf("\n");
  printf("\t 1. Locate a entrant\n");
  printf("\t 2. Show how many entrants have not yet started\n");
  printf("\t 3. Show how many entrants are currently on the course\n");
  printf("\t 4. Show how many entrants have finished\n");
  printf("\t 5. Supply checkpoint times manually\n");
  printf("\t 6. Supply checkpoint times from a file\n");
  printf("\t 7. Display results list\n");
  printf("\t 8. Exit the program\n");
  printf("\n");

  printf("%02d:%02d  >>  ", event->time->hours, event->time->minutes);
  line = readline();
  token = strtok(line, "\n");
  input = atoi(token);
  free(line);

  return input;
}

void locate_entrant(Event* event) {
  Entrant* entrant;
  int entrant_id;
  char* line;

  printf("Enter entrant id: ");
  line = readline();
  strtok(line, "\n");
  entrant_id = atoi(line);

  entrant = entrant_from_id(event->entrants, entrant_id);
  if (entrant)
    entrant_stats(entrant, event->time);
  free(line);
}

int count_by_status(Event* event, entrant_status status) {
  Entrant* entrant;
  int ret_val = 0;
  int i = 0;

  for (i = 0; i < Vector_size(event->entrants); i++) {
    Vector_get(event->entrants, i, &entrant);
    if (entrant->status == status) ret_val++;
  }

  return ret_val;
}

void display_results(Event* event) {
  Entrant* entrant;
  int i = 0;

  entrants_sort(event);
  if (count_by_status(event, FINISHED) > 0) {
    printf("\n\tFinished:\n");
    for (; i < Vector_size(event->entrants); i++) {
      Vector_get(event->entrants, i, &entrant);
      if (entrant->status != FINISHED) break;
      printf("\t\t%2d: %-50s\n", entrant->id, entrant->name);
      printf("\t\t\tCourse: %c Total time: %3d mins\n", entrant->course->id, entrant->duration);
    }
  }
  if (count_by_status(event, STARTED) > 0) {
    printf("\n\tRunning:\n");
    for (; i < Vector_size(event->entrants); i++) {
      Vector_get(event->entrants, i, &entrant);
      if (entrant->status != STARTED) break;
      printf("\t\t%2d: %-50s\n", entrant->id, entrant->name);
      printf("\t\t\tCourse: %c Track: %2d Run time: %3d mins\n", entrant->course->id,
          entrant->curr_track->id, entrant->duration);
    }
  }
  if (count_by_status(event, NOT_STARTED) > 0) {
    printf("\n\tWaiting to start:\n");
    for (; i < Vector_size(event->entrants); i++) {
      Vector_get(event->entrants, i, &entrant);
      printf("\t\t%2d: %-50s\n", entrant->id, entrant->name);
      printf("\t\t\tCourse: %c\n", entrant->course->id);
    }
  }
}

void update_manual(Event* event) {
  char* line;
  int node_id;
  int entrant_id;
  Time* time;

  /* type */

  /* node id */
  printf("Enter node id: ");
  line = readline();
  node_id = atoi(line);
  free(line);

  /* entrant id */
  printf("Enter entrant id: ");
  line = readline();
  entrant_id = atoi(line);
  free(line);

  /* time */
  printf("Enter time (hh:mm): ");
  line = readline();
  time = str_to_time(line);
  free(line);

  update_time(event, time);
  entrant_update_location(event, entrant_id, node_id);
  entrant_stats(entrant_from_id(event->entrants, entrant_id), event->time);
  free(time);
}

void update_file(Event* event) {
  char* filename = get_filename("Enter checkpoint file: ");
  Vector* lines = read_file(filename);
  char* line;
  char* token;
  int node_id;
  int entrant_id;
  Time* time;
  int i = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Vector_get(lines, i, &line);

    /* type */
    token = strtok(line, " ");

    /* node id */
    token = strtok(NULL, " ");
    node_id = atoi(token);

    /* entrant id */
    token = strtok(NULL, " ");
    entrant_id = atoi(token);

    /* time */
    token = strtok(NULL, "\n");
    time = str_to_time(token);

    update_time(event, time);
    entrant_update_location(event, entrant_id, node_id);
  }

  display_results(event);
  Vector_dispose(lines);
}

int main(int argc, char* argv[]) {
  Event* event = read_data();
  int running = 1;
  int input;

  display_event_header(event);
  while (running) {
    input = display_menu(event);
    switch (input) {
      case 1:
        locate_entrant(event);
        break;
      case 2:
        printf("\t%d\n", count_by_status(event, NOT_STARTED));
        break;
      case 3:
        printf("\t%d\n", count_by_status(event, STARTED));
        break;
      case 4:
        printf("\t%d\n", count_by_status(event, FINISHED));
        break;
      case 5:
        update_manual(event);
        break;
      case 6:
        update_file(event);
        break;
      case 7:
        display_results(event);
        break;
      case 8:
        running = 0;
        break;
      default:
        /* invalid input, do nothing */
        break;
    }
  }

  return EXIT_SUCCESS;
}
