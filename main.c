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

typedef enum {
  NOT_STARTED,
  RUNNING,
  FINISHED
} status_t; 

typedef enum {
  CP,
  MC,
  JN
} node_t;

typedef struct Node {
  int id;
  node_t type;
  struct Node* next;
} Node;

typedef struct Competitor {
  int id;
  char* name;
  status_t status;
  struct Competitor* next;
} Competitor;

typedef struct Event {
  char* title;
  char* date;
  int start_hrs;
  int start_mins;
  Node** node_list;
  Competitor** competitor_list;
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

  fgets(line, 80, fp); // Now this is borked somehow
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

void read_nodes(Event* event, char* filename) {
  char line[80];
  char* token;
  char* delim = " ";
  FILE* fp = fopen(filename, "r");
  Node* node;
  Node* prev;

  fgets(line, 80, fp);
  node = malloc(sizeof(Node));
  token = strtok(line, delim);
  node->id = atoi(token);
  token = strtok(NULL, delim);
  if (strncmp(token, "CP", 2) == 0) {
    node->type = CP;
  } else if (strncmp(token, "MC", 2) == 0) {
    node->type = MC;
  } else {
    node->type = JN;
  }
  node->next = NULL;
  event->node_list = &node;
  prev = node;
  while (fgets(line, 80, fp) != NULL) {
    node = NULL;
    node = malloc(sizeof(Node));
    token = strtok(line, delim);
    node->id = atoi(token);
    token = strtok(NULL, delim);
    if (strncmp(token, "CP", 2) == 0) {
      node->type = CP;
    } else if (strncmp(token, "MC", 2) == 0) {
      node->type = MC;
    } else {
      node->type = JN;
    }
    node->next = NULL;
    prev->next = node;
  }

  fclose(fp);
}

void locate_competitor(Event* event) {
  int input;
  Competitor* competitor;

  printf("Please enter competitor id: ");
  scanf("%d", &input);
  competitor = (*event->competitor_list);
  while (competitor != NULL) {
    if (competitor->id == input) {
      printf("%d: %s ", competitor->id, competitor->name);
      // TODO if the competitor is running, it should show where he/she is on the course
      if (competitor->status == NOT_STARTED) {
        printf("Not started\n");
      } else if (competitor->status == RUNNING) {
        printf("Running\n");
      } else {
        printf("Finished\n");
      }
      return;
    }
    competitor = competitor->next;
  }
}

int main(int argc, char* argv[]) {
  int input;
  char* filename;
  Event* event;

  /* Init data structures and whatnot */
  event = malloc(sizeof(Event));
  event->node_list = NULL;
  event->competitor_list = NULL;

  printf("Please enter the name of the file containing event data: ");
  filename = getline_foo();
  read_event(event, filename);

  printf("Please enter the name of the file containing node data: ");
  filename = getline_foo();
  read_nodes(event, filename);

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
      free(event); // And probably a whole lot more besides
      return EXIT_SUCCESS;
    } else {
      // invalid input, do nothing
    }
  }

  return EXIT_SUCCESS;
}
