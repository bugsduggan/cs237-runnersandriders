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
  char* filename;
  Event* event;

  /* read in data files */
  printf("Event file: ");
  filename = readline();
  event = event_read(filename);
  free(filename);
  /* nodes */
  printf("Nodes file: ");
  filename = readline();
  event->nodes = nodes_read(filename);
  free(filename);
  /* tracks */
  printf("Tracks file: ");
  filename = readline();
  event->tracks = tracks_read(filename);
  free(filename);
  /* courses */
  printf("Courses file: ");
  filename = readline();
  event->courses = courses_read(filename, event->nodes);
  free(filename);
  /* entrants */
  printf("Entrants file: ");
  filename = readline();
  event->entrants = entrants_read(filename);
  free(filename);

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

int prompt() {
  char* line;
  int result;
  printf(">>  ");
  line = readline();
  result = atoi(line);
  return result;
}

void display_entrant_stats(Event* event, int entrant_id) {
  int i = 0;
  Entrant* entrant;

  for (i = 0; i < Vector_size(event->entrants); i++) {
    Vector_get(event->entrants, i, &entrant);
    if (entrant->id == entrant_id) {
      printf("\t%d: %s - ", entrant->id, entrant->name);
      if (entrant->status == NOT_STARTED) {
        printf("NOT STARTED");
      } else if (entrant->status == STARTED) {
        printf("STARTED - Last seen: %d Current time: %d",
            entrant->last_seen, entrant->duration);
      } else {
        printf("FINISHED - Total time: %d", entrant->duration);
      }
      printf("\n");
      return; /* this is just so we don't print the error message */
    }
  }
  printf("Invalid entrant id\n");
}

void locate_entrant(Event* event) {
  char* line;
  int entrant_id;

  printf("Enter competitor id: ");
  line = readline();
  entrant_id = atoi(line);
  free(line);
  printf("\n");
  display_entrant_stats(event, entrant_id);
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

  entrant_update_location(event, entrant_id, node_id, hrs, mins);
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
  printf("Enter checkpoint data file: ");
  line = readline();
  lines = read_file(line);
  free(line);
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
    entrant_update_location(event, entrant_id, node_id, hrs, mins);
  }
  Vector_dispose(lines);
}

void display_results(Event* event) {
  int i = 0;

  printf("\n");
  for (i = 0; i < Vector_size(event->entrants); i++) {
    display_entrant_stats(event, i + 1);
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
