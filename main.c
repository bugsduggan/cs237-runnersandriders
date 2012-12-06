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
  event->courses = courses_read(filename);
  free(filename);
  /* entrants */
  printf("Entrants file: ");
  filename = readline();
  event->entrants = entrants_read(filename);
  free(filename);

  return event;
}

int main(int argc, char* argv[]) {
  Event* event = generate_data();

  event_dispose(event);
  return EXIT_SUCCESS;
}
