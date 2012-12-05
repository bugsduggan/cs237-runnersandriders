
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data.h"

/*
 * general helper funcs
 */

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

/*
 * node functions
 */

void Node_insert_r(Node* head, Node* new_node) {
  if (head->next) {
    Node_insert_r(head->next, new_node);
  } else {
    head->next = new_node;
  }
}

void Node_insert(Node_list* list, Node* new_node) {
  if (list->head) {
    Node_insert_r(list->head, new_node);
  } else {
    list->head = new_node;
  }
}

Node_list* make_nodes(char* filename) {
  char line[80];
  char* token;
  FILE* fp;
  Node_list* list = malloc(sizeof(Node_list));
  list->head = NULL;

  fp = fopen(filename, "r");
  while (fgets(line, 80, fp) != NULL) {
    Node* new_node = malloc(sizeof(Node));
    token = strtok(line, " ");
    new_node->id = atoi(token);
    token = strtok(NULL, " ");
    strtok(token, "\n");
    if (strcmp(token, "CP") == 0) {
      new_node->type = CP;
    } else if (strcmp(token, "MC") == 0) {
      new_node->type = MC;
    } else {
      new_node->type = JN;
    }
    new_node->next = NULL;
    Node_insert(list, new_node);
  }

  fclose(fp);
  return list;
}

void Node_destroy_r(Node* head) {
  if (head) {
    Node_destroy_r(head->next);
    free(head);
  }
}

void Node_destroy(Node_list* list) {
  if (list) {
    if (list->head) {
      Node_destroy_r(list->head);
    }
    free(list);
  }
}

/*
 * track functions
 */

void Track_insert_r(Track* head, Track* new_track) {
  if (head->next) {
    Track_insert_r(head->next, new_track);
  } else {
    head->next = new_track;
  }
}

void Track_insert(Track_list* list, Track* new_track) {
  if (list->head) {
    Track_insert_r(list->head, new_track);
  } else {
    list->head = new_track;
  }
}

Track_list* make_tracks(char* filename) {
  char line[80];
  char* token;
  FILE* fp;
  Track_list* list = malloc(sizeof(Track_list));
  list->head = NULL;

  fp = fopen(filename, "r");
  while (fgets(line, 80, fp) != NULL) {
    Track* new_track = malloc(sizeof(Track));
    token = strtok(line, " ");
    new_track->id = atoi(token);
    token = strtok(line, " ");
    new_track->start_node = atoi(token);
    token = strtok(line, " ");
    new_track->end_node = atoi(token);
    token = strtok(line, " ");
    new_track->safe_time = atoi(token);
    new_track->next = NULL;
    Track_insert(list, new_track);
  }

  fclose(fp);
  return list;
}

void Track_destroy_r(Track* head) {
  if (head) {
    Track_destroy_r(head->next);
    free(head);
  }
}

void Track_destroy(Track_list* list) {
  if (list) {
    if (list->head) {
      Track_destroy_r(list->head);
    }
    free(list);
  }
}

/*
 * event functions
 */

Event* make_event(char* filename) {
  char line[80];
  char* token;
  FILE* fp;
  Event* event = malloc(sizeof(Event));
  event->nodes = NULL;
  event->tracks = NULL;

  fp = fopen(filename, "r");
  /* read title */
  fgets(line, 80, fp);
  event->title = strdup(line);
  /* read date */
  fgets(line, 80, fp);
  event->date = strdup(line);
  /* read start time (hrs:mins) */
  fgets(line, 80, fp);
  token = strtok(line, ":");
  event->start_hrs = atoi(token);
  token = strtok(NULL, ":");
  event->start_mins = atoi(token);

  fclose(fp);
  return event;
}

void Event_destroy(Event* event) {
  if (event) {
    if (event->title) free(event->title);
    if (event->date) free(event->date);
    Node_destroy(event->nodes);
    Track_destroy(event->tracks);
    free(event);
  }
}
