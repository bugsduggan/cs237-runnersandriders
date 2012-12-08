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

int display_menu() {
  char* line;
  char* token;
  int input;
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

  printf(">>  ");
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
    entrant_stats(entrant);
  free(line);
}

int main(int argc, char* argv[]) {
  Event* event = read_data();
  int running = 1;
  int input;

  display_event_header(event);
  while (running) {
    input = display_menu();
    switch (input) {
      case 1:
        locate_entrant(event);
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
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
