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

/*
 * returns a valid filename 
 */
char* get_filename(char* prompt) {
  char* filename;

  printf("%s", prompt);
  filename = readline();
  while (!valid_filename(filename)) {
    printf("Could not open %s\n", filename);
    printf("%s", prompt);
    free(filename);
    filename = readline();
  }
  return filename;
}

/*
 * reads in data from all relevant files
 * returns a pointer to an event object
 */
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

  event->nodes = nodes;
  event->entrants = entrants;
  return event;
}

/*
 * print out the name and details of the event
 */
void display_event_header(Event* event) {
  printf("\n\n");
  printf("\t%s\n", event->title);
  printf("\t%s\n", event->date);
  printf("\t%d:%d\n", event->start->hours, event->start->minutes);
  printf("\n");
}

/*
 * display the menu and grab an int from the user
 * to specify what action to take
 */
int display_menu(Event* event) {
  char* line;
  char* token;
  int input;
  printf("\n");
  printf("Please select from the following options:\n");
  printf("\n");
  printf("\t 1. Locate a entrant\n");
  printf("\t 2. Show how many entrants have not yet started\n");
  printf("\t 3. Show how many entrants are currently on the course\n");
  printf("\t 4. Show how many entrants have finished\n");
  printf("\t 5. List entrants excluded for safety\n");
  printf("\t 6. List entrants excluded for incorrect route\n");
  printf("\t 7. Supply checkpoint times manually\n");
  printf("\t 8. Supply checkpoint times from a file\n");
  printf("\t 9. Display results list\n");
  printf("\t10. Exit the program\n");
  printf("\n");

  printf("%02d:%02d  >>  ", event->time->hours, event->time->minutes);
  line = readline();
  token = strtok(line, "\n");
  input = atoi(token);
  free(line);

  return input;
}

/*
 * locate a particular entrant from id
 */
void locate_entrant(Event* event) {
  Entrant* entrant;
  int entrant_id;
  char* line;

  printf("Enter entrant id: ");
  line = readline();
  strtok(line, "\n");
  entrant_id = atoi(line);

  entrant = entrant_from_id(event->entrants, entrant_id);
  entrant_stats(entrant, event->time);
  free(line);
}

/*
 * returns how many entrants have a particular status
 */
int count_by_status(Event* event, entrant_status status) {
  Entrant* entrant;
  int ret_val = 0;
  int i = 0;

  for (i = 0; i < Vector_size(event->entrants); i++) {
    Vector_get(event->entrants, i, &entrant);
    if (entrant->status == status) ret_val++;
  }

  return ret_val;
}

/*
 * list entrants excluded for safety reasons
 */
void list_excluded_safety(Event* event) {
  Entrant* entrant;
  int i = 0;

  /* make sure there's at least one */
  if (count_by_status(event, DISQUAL_SAFETY) == 0) {
    printf("\tNo entrants disqualified for safety reasons\n");
  } else {
    for (i = 0; i < Vector_size(event->entrants); i++) {
      Vector_get(event->entrants, i, &entrant);
      if (entrant->status == DISQUAL_SAFETY) {
        printf("\t%2d: %-50s\n", entrant->id, entrant->name);
        printf("\t\tCourse: %c Last node: %2d\n", entrant->course->id,
            entrant->last_cp_node->id);
      }
    }
  }
}

/*
 * list entrants excluded for getting lost
 */
void list_excluded_incorrect(Event* event) {
  Entrant* entrant;
  int i = 0;

  /* make sure there's at least one */
  if (count_by_status(event, DISQUAL_INCORR) == 0) {
    printf("\tNo entrants disqualified for incorrect route\n");
  } else {
    for (i = 0; i < Vector_size(event->entrants); i++) {
      Vector_get(event->entrants, i, &entrant);
      if (entrant->status == DISQUAL_INCORR) {
        printf("\t%2d: %-50s\n", entrant->id, entrant->name);
        printf("\t\tCourse: %c Last node: %2d\n", entrant->course->id,
            entrant->last_cp_node->id);
      }
    }
  }
}

/*
 * display all the entrants based on status/duration
 */
void display_results(Event* event) {
  Entrant* entrant;
  int i = 0;

  entrants_sort(event);
  /* since the entrants are now sorted, we know that all entrants with a particular
   * status will be grouped together */

  /* display finished */
  if (count_by_status(event, FINISHED) > 0) {
    printf("\n\tFinished:\n");
    for (; i < Vector_size(event->entrants); i++) {
      Vector_get(event->entrants, i, &entrant);
      if (entrant->status != FINISHED) break; /* if entrant hasn't finished, skip to
                                                 the next block of entrants */
      printf("\t\t%3d: %-50s\n", entrant->id, entrant->name);
      printf("\t\t\tCourse: %c Total time: %3d mins\n", entrant->course->id, entrant->duration);
    }
  }
  /* display started and stopped */
  if (count_by_status(event, STARTED) + count_by_status(event, STOPPED) > 0) {
    printf("\n\tRunning:\n");
    for (; i < Vector_size(event->entrants); i++) {
      Vector_get(event->entrants, i, &entrant);
      if (entrant->status != STARTED && entrant->status != STOPPED) break;

      printf("\t\t%3d: %-50s\n", entrant->id, entrant->name);
      printf("\t\t\tCourse: %c Track: %2d Run time: %3d mins\n", entrant->course->id,
          entrant->curr_track->id, entrant->duration);
    }
  }
  /* display disqualified */
  if (count_by_status(event, DISQUAL_SAFETY) + count_by_status(event, DISQUAL_INCORR) > 0) {
    printf("\n\tDisqualified:\n");
    for (; i < Vector_size(event->entrants); i++) {
      Vector_get(event->entrants, i, &entrant);
      if (entrant->status != DISQUAL_SAFETY && entrant->status != DISQUAL_INCORR) break;

      printf("\t\t%3d: %-50s\n", entrant->id, entrant->name);
      printf("\t\t\tCourse: %c ", entrant->course->id);
      if (entrant->status == DISQUAL_SAFETY) printf("Disqualified for safety\n");
      else printf("Disqualified for incorrect route\n");
    }
  }
  /* display waiting to start */
  if (count_by_status(event, NOT_STARTED) > 0) {
    printf("\n\tWaiting to start:\n");
    for (; i < Vector_size(event->entrants); i++) {
      Vector_get(event->entrants, i, &entrant);

      printf("\t\t%3d: %-50s\n", entrant->id, entrant->name);
      printf("\t\t\tCourse: %c\n", entrant->course->id);
    }
  }
}

/*
 * update an entrant manually
 */
void update_manual(Event* event) {
  char* line;
  char type;
  int node_id;
  int entrant_id;
  Time* time;

  /* type */
  printf("Enter update type (T/I/A/D/E): ");
  line = readline();
  type = line[0];
  while (type != 't' && type != 'T' &&
      type != 'i' && type != 'I' &&
      type != 'a' && type != 'A' &&
      type != 'd' && type != 'D' &&
      type != 'e' && type != 'E') {
    printf("Invalid type. Please enter one of T/I/A/D/E: ");
    free(line);
    line = readline();
    type = line[0];
  }
  free(line);

  /* node id */
  printf("Enter node id: ");
  line = readline();
  node_id = atoi(line);
  free(line);

  /* entrant id */
  printf("Enter entrant id: ");
  line = readline();
  entrant_id = atoi(line);
  free(line);

  /* time */
  printf("Enter time (hh:mm): ");
  line = readline();
  time = str_to_time(line);
  free(line);

  update_time(event, time, entrant_id); /* entrant_id refers to an entrant that will
                                           NOT be updated by this call */
  entrant_update_location(event, type, entrant_id, node_id); /* they get updated here */

  /* print out the entrant's stats */
  entrant_stats(entrant_from_id(event->entrants, entrant_id), event->time);
  free(time);
}

/*
 * update entrants from a file
 */
void update_file(Event* event) {
  char* filename = get_filename("Enter checkpoint file: ");
  Vector* lines = read_file(filename);
  char* line;
  char* token;
  char type;
  int node_id;
  int entrant_id;
  Time* time;
  int i = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Vector_get(lines, i, &line);

    /* type */
    token = strtok(line, " ");
    type = token[0];

    /* node id */
    token = strtok(NULL, " ");
    node_id = atoi(token);

    /* entrant id */
    token = strtok(NULL, " ");
    entrant_id = atoi(token);

    /* time */
    token = strtok(NULL, "\n");
    time = str_to_time(token);

    update_time(event, time, entrant_id);
    entrant_update_location(event, type, entrant_id, node_id);
  }

  display_results(event);
  Vector_dispose(lines);
}

/*
 * the main method (including program loop)
 */
int main(int argc, char* argv[]) {
  Event* event = read_data();
  int running = 1;
  int input;

  display_event_header(event);
  while (running) {
    input = display_menu(event);
    switch (input) {
      case 1:
        locate_entrant(event);
        break;
      case 2:
        printf("\t%d\n", count_by_status(event, NOT_STARTED));
        break;
      case 3:
        printf("\t%d\n", count_by_status(event, STARTED) + count_by_status(event, STOPPED));
        break;
      case 4:
        printf("\t%d\n", count_by_status(event, FINISHED));
        break;
      case 5:
        list_excluded_safety(event);
        break;
      case 6:
        list_excluded_incorrect(event);
        break;
      case 7:
        update_manual(event);
        break;
      case 8:
        update_file(event);
        break;
      case 9:
        display_results(event);
        break;
      case 10:
        running = 0;
        break;
      default:
        /* invalid input, do nothing */
        break;
    }
  }

  return EXIT_SUCCESS;
}
