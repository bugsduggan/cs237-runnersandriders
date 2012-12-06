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
