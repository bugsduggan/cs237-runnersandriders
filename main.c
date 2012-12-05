/*
 * main.c
 *
 * The main entry point to the program.
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data.h"

/* The following function was taken from
 * http://stackoverflow.com/questions/314401/how-to-read-a-line-from-the-console-in-c
 * It's probably overkill for this assignment but it works! */
char* getline_foo() {
  char* line = malloc(100), * linep = line;
  size_t lenmax = 100, len = lenmax;
  int c;

  if(line == NULL)
    return NULL;

  for(;;) {
    c = fgetc(stdin);
    if(c == EOF)
      break;

    if(--len == 0) {
      len = lenmax;
      char * linen = realloc(linep, lenmax *= 2);

      if(linen == NULL) {
        free(linep);
        return NULL;
      }
      line = linen + (line - linep);
      linep = linen;
    }

    if((*line++ = c) == '\n')
      break;
  }
  *line = '\0';
  return linep;
}

void print_menu() {
  printf("\n");
  printf("Please select from the following options:\n");
  printf("\n");
  printf("\t1. Locate a competitor\n");
  printf("\t2. Show how many competitors have not yet started\n");
  printf("\t3. Show how many competitors are currently on the course\n");
  printf("\t4. Show how many competitors have finished\n");
  printf("\t5. Supply checkpoint times manually\n");
  printf("\t6. Supply checkpoint times from a file\n");
  printf("\t7. Display results list\n");
  printf("\t8. Exit the program\n");
  printf("\n");
}

int prompt() {
  int result = -1;
  printf(">>  ");
  scanf("%d", &result);
  return result;
}

int main(int argc, char* argv[]) {
  int input;
  char* filename;
  Event* event;

  /* Init data structures and whatnot */

  printf("Please enter the name of the file containing event data: ");
  filename = getline_foo();
  strtok(filename, "\n");
  event = make_event(filename);
  free(filename);

  printf("Please enter the name of the file containing node data: ");
  filename = getline_foo();
  strtok(filename, "\n");
  event->nodes = make_nodes(filename);
  free(filename);

  printf("Please enter the name of the file containing track data: ");
  filename = getline_foo();
  strtok(filename, "\n");
  event->tracks = make_tracks(filename);
  free(filename);

  /* The main program loop, shows the menu and prompts for input */
  input = -1;
  while (1) {
    print_menu();
    input = prompt();
    if (input == 1) { /* locate competitor */

    } else if (input == 2) { /* show not started */

    } else if (input == 3) { /* show on course */

    } else if (input == 4) { /* show finished */

    } else if (input == 5) { /* supply checkpoints manually */

    } else if (input == 6) { /* read from file */

    } else if (input == 7) { /* show results */

    } else if (input == 8) { /* quit */
      Event_destroy(event);
      return EXIT_SUCCESS;
    } else {
      /* invalid input, do nothing */
    }
  }

  return EXIT_SUCCESS;
}
