/*
 * main
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
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

int main(int argc, char* argv[]) {
  read_data();

  return EXIT_SUCCESS;
}
