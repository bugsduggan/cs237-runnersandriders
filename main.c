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

void read_data() {
  char* filename;
  Vector* nodes;
  Vector* tracks;
  Vector* courses;

  filename = get_filename("Please enter nodes file: ");
  nodes = read_nodes(filename);

  filename = get_filename("Please enter tracks file: ");
  tracks = read_tracks(filename, nodes);

  filename = get_filename("Please enter courses file: ");
  courses = read_courses(filename, nodes, tracks);

  filename = get_filename("Please enter entrants file: ");
  courses = read_entrants(filename, courses);
}

int main(int argc, char* argv[]) {
  read_data();

  return EXIT_SUCCESS;
}
