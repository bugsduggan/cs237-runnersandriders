/*
 * data
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "util.h"
#include "data.h"

#include <stdio.h>

void node_dispose(void* node) {
  Node* foo = *(Node**)node;
  if (foo) free(foo);
}

Vector* nodes_read(char* filename) {
  Vector* lines = read_file(filename);
  Vector* nodes = Vector_new(sizeof(Node*), node_dispose);
  char* line;
  char* token;
  int i = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Node* node = malloc(sizeof(Node));
    Vector_get(lines, i, &line);
    /* read node id */
    token = strtok(line, " ");
    node->id = atoi(token);
    /* read node type */
    token = strtok(NULL, " ");
    if (strcmp(token, "CP") == 0) {
      node->type = CP;
    } else if (strcmp(token, "MC") == 0) {
      node->type = MC;
    } else {
      node->type = JN;
    }
    Vector_add(nodes, &node);
  }
  Vector_dispose(lines);
  return nodes;
}

void nodes_dispose(Vector* nodes) {
  Vector_dispose(nodes);
}

int main(int argc, char* argv[]) {
  Vector* nodes = nodes_read("main_data/nodes.txt");
  int i = 0;
  Node* node;

  for (i = 0; i < Vector_size(nodes); i++) {
    Vector_get(nodes, i, &node);
    printf("%d: ", node->id);
    if (node->type == CP) {
      printf("CP");
    } else if (node->type == MC) {
      printf("MC");
    } else {
      printf("JN");
    }
    printf("\n");
  }
  nodes_dispose(nodes);
  return EXIT_SUCCESS;
}
