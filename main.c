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

int main(int argc, char* argv[]) {
  char* filename;
  Event* event;

  /* read in data files */
  printf("Event file: ");
  filename = readline();
  event = event_read(filename);

  event_dispose(event);
  return EXIT_SUCCESS;
}
