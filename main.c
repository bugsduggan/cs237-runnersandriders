/*
 * main
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "util.h"
#include "data.h"

Event* generate_data() {
  Event* event;
  event = event_read(get_file("Event file: "));
  event->nodes = nodes_read(get_file("Nodes file: "));
  event->tracks = tracks_read(get_file("Tracks file: "));
  event->courses = courses_read(get_file("Courses file: "), event->nodes, event->tracks);
  event->entrants = entrants_read(get_file("Entrants file: "));
  return event;
}

void print_menu() {
  printf("\n");
  printf("Please select from the following options:\n");
  printf("\n");
  printf("\t1. Locate a entrant\n");
  printf("\t2. Show how many entrants have not yet started\n");
  printf("\t3. Show how many entrants are currently on the course\n");
  printf("\t4. Show how many entrants have finished\n");
  printf("\t5. Supply checkpoint times manually\n");
  printf("\t6. Supply checkpoint times from a file\n");
  printf("\t7. Display results list\n");
  printf("\t8. Exit the program\n");
  printf("\n");
}

void display_entrant_stats(Entrant* entrant) {
  printf("\t%02d:  %c  %-50s \n\t\t", entrant->id, entrant->course_id, entrant->name);
  if (entrant->status == NOT_STARTED) {
    printf("NOT STARTED");
  } else if (entrant->status == STARTED) {
    printf("STARTED     Last seen: %02d    Current time: %3d mins",
        entrant->last_seen, entrant->duration);
  } else {
    printf("FINISHED    Total time: %02d mins", entrant->duration);
  }
  printf("\n");
}

void locate_entrant(Event* event) {
  char* line;
  int entrant_id;

  printf("Enter competitor id: ");
  line = readline();
  entrant_id = atoi(line);
  free(line);
  printf("\n");
  display_entrant_stats(entrant_from_id(event->entrants, entrant_id));
}

void count_entrants(Event* event, entrant_status status) {
  int result = 0;
  int i = 0;
  Entrant* entrant;

  for (i = 0; i < Vector_size(event->entrants); i++) {
    Vector_get(event->entrants, i, &entrant);
    if (entrant->status == status) result++;
  }
  printf("\n\t%d\n", result);
}

void update_entrants(Event* event, int hrs, int mins) {
  int i = 0;
  Entrant* entrant;

  for (i = 0; i < Vector_size(event->entrants); i++) {
    Vector_get(event->entrants, i, &entrant);
    entrant_update_time(entrant, hrs, mins);
  }
}

void supply_checkpoint_manual(Event* event) {
  int node_id;
  int entrant_id;
  int hrs;
  int mins;
  char* line;
  char* token;

  printf("\nEnter node id: ");
  line = readline();
  node_id = atoi(line);
  free(line);

  printf("Enter entrant id: ");
  line = readline();
  entrant_id = atoi(line);
  free(line);

  printf("Enter time (hh:mm): ");
  line = readline();
  token = strtok(line, ":");
  hrs = atoi(token);
  token = strtok(NULL, ":");
  mins = atoi(token);
  free(line);

  entrant_update_location(event, entrant_from_id(event->entrants, entrant_id),
      node_id, hrs, mins);
  update_entrants(event, hrs, mins);
}

void supply_checkpoint_file(Event* event) {
  char* line;
  Vector* lines;
  char* token;
  int i = 0;
  int node_id;
  int entrant_id;
  int hrs;
  int mins;

  /* grab filename */
  lines = read_file(get_file("Enter checkpoint data file: "));
  for (i = 0; i < Vector_size(lines); i++) {
    Vector_get(lines, i, &line);
    /* grab checkpoint type */
    token = strtok(line, " ");
    /* do nothing with it (for now) */
    /* grab node id */
    token = strtok(NULL, " ");
    node_id = atoi(token);
    /* grab entrant id */
    token = strtok(NULL, " ");
    entrant_id = atoi(token);
    /* grab time */
    token = strtok(NULL, ":");
    hrs = atoi(token);
    token = strtok(NULL, ":");
    mins = atoi(token);
    entrant_update_location(event, entrant_from_id(event->entrants, entrant_id),
        node_id, hrs, mins);
  }
  update_entrants(event, hrs, mins);
  Vector_dispose(lines);
}

void display_results(Event* event) {
  int i = 0;
  Entrant* entrant;

  entrants_sort(event->entrants);
  printf("\n");
  for (i = 0; i < Vector_size(event->entrants); i++) {
    Vector_get(event->entrants, i, &entrant);
    display_entrant_stats(entrant);
  }
}

int main(int argc, char* argv[]) {
  Event* event = generate_data();
  int input = -1;

  /*
   * Main program loop
   */
  while (1) {
    print_menu();
    input = prompt();
    if (input == 1) {
      locate_entrant(event);
    } else if (input == 2) {
      count_entrants(event, NOT_STARTED);
    } else if (input == 3) {
      count_entrants(event, STARTED);
    } else if (input == 4) {
      count_entrants(event, FINISHED);
    } else if (input == 5) {
      supply_checkpoint_manual(event);
    } else if (input == 6) {
      supply_checkpoint_file(event);
    } else if (input == 7) {
      display_results(event);
    } else if (input == 8) {
      break;
    } else {
      /* invalid input, do nothing */
    }
  }

  event_dispose(event);
  return EXIT_SUCCESS;
}
