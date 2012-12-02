/*
 * main.c
 *
 * The main entry point to the program.
 *
 * Tom Leaman (tom@tomleaman.co.uk)
 */

#include <stdlib.h>
#include <stdio.h>

typedef struct Competitor {
  int id;
  char* name;
  struct Competitor* next;
} Competitor;

typedef struct Event {
  char* title;
  char* date;
  int start_hrs;
  int start_mins;
  Competitor** competitor_list;
} Event;

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

void locate_competitor(Event* event) {
  int input;
  Competitor* competitor;

  printf("Please enter competitor id: ");
  scanf("%d", &input);
  competitor = (*event->competitor_list);
  while (competitor != NULL) {
    if (competitor->id == input) {
      printf("%d: %s STATUS\n", competitor->id, competitor->name); // TODO Plug-in the actual status
      return;
    }
    competitor = competitor->next;
  }
}

int main(int argc, char* argv[]) {
  int input;
  char* filename;
  FILE* fr;
  char line[80];
  Event* event;

  /* Init data structures and whatnot */
  event = malloc(sizeof(Event));
  event->competitor_list = NULL;

  printf("Please enter the name of the file containing event data: ");
  scanf("%79[a-zA-Z0-9]", &filename);
  fr = fopen(filename, "rt");
  fgets(line, 80, fr);
  sscanf(line, "%s", &event->title);
  fgets(line, 80, fr);
  sscanf(line, "%s", &event->date);
  fgets(line, 80, fr);
  // TODO parse the start time
  fclose(fr);

  /* Double-check the info we've got so far */
  printf("%s\n", event->title);
  printf("%s\n", event->date);

  /* The main program loop, shows the menu and prompts for input */
  input = -1;
  while (1) {
    print_menu();
    input = prompt();
    if (input == 1) { // locate competitor
      locate_competitor(event);
    } else if (input == 2) { // show not started

    } else if (input == 3) { // show on course

    } else if (input == 4) { // show finished

    } else if (input == 5) { // supply checkpoints manually

    } else if (input == 6) { // read from file

    } else if (input == 7) { // show results

    } else if (input == 8) { // quit
      return EXIT_SUCCESS;
    } else {
      // invalid input, do nothing
    }
  }

  return EXIT_SUCCESS;
}
