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

typedef struct Event {
  char* title;
  char* date;
  int start_hrs;
  int start_mins;
  Node_list* nodes;
} Event;

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

/* This function was taken from
 * http://cboard.cprogramming.com/c-programming/95462-compiler-error-warning-implicit-declaration-function-strdup.html
 * as it's not part of the c89 standard */
char *strdup(const char *str)
{
  int n = strlen(str) + 1;
  char *dup = malloc(n);
  if(dup) {
    strcpy(dup, str);
  }
  return dup;
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

void read_event(Event* event, char* filename) {
  char line[80];
  char* token;
  char* delim = ":";
  FILE* fp = fopen(filename, "r");

  fgets(line, 80, fp);
  event->title = strdup(line);
  fgets(line, 80, fp);
  event->date = strdup(line);
  fgets(line, 80, fp);
  token = strtok(line, delim);
  event->start_hrs = atoi(token);
  token = strtok(NULL, delim);
  event->start_mins = atoi(token);

  fclose(fp);
}

int main(int argc, char* argv[]) {
  int input;
  char* filename;
  Event* event;

  /* Init data structures and whatnot */
  event = malloc(sizeof(Event));
  event->nodes = NULL;

  printf("Please enter the name of the file containing event data: ");
  filename = getline_foo();
  strtok(filename, "\n");
  read_event(event, filename);
  free(filename);

  printf("Please enter the name of the file containing node data: ");
  filename = getline_foo();
  strtok(filename, "\n");
  event->nodes = make_nodes(filename);
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
      Node_destroy(event->nodes);
      free(event->title);
      free(event->date);
      free(event); /* And probably a whole lot more besides */
      return EXIT_SUCCESS;
    } else {
      /* invalid input, do nothing */
    }
  }

  return EXIT_SUCCESS;
}
