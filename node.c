
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node.h"

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

void Node_print(Node_list* list) {
  Node* curr = list->head;
  while (curr != NULL) {
    printf("%d: ", curr->id);
    if (curr->type == CP) {
      printf("CP");
    } else if (curr->type == MC) {
      printf("MC");
    } else {
      printf("JN");
    }
    printf("\n");
    curr = curr->next;
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
