#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * functions declared in data.h
 */

/* read nodes from the file */
Vector* read_nodes(char* filename) {
  Vector* lines = read_file(filename);
  Vector* nodes = Vector_new(sizeof(Node*), NULL);
  Node* node;
  char* line;
  char* token;
  int i = 0;

  for (i = 0; i < Vector_size(lines); i++) {
    Vector_get(lines, i, &line);
    node = malloc(sizeof(Node));

    /* id */
    token = strtok(line, " ");
    node->id = atoi(token);

    /* type */
    token = strtok(NULL, " ");
    node->type = str_to_type(token);

    Vector_add(nodes, &node);
  }

  Vector_dispose(lines);
  return nodes;
}

/* find a node from id */
Node* node_from_id(Vector* nodes, int id) {
  Node* node;
  int i = 0;

  for (i = 0; i < Vector_size(nodes); i++) {
    Vector_get(nodes, i, &node);
    if (node->id == id) return node;
  }

  return NULL;
}

/* get a node type from a string */
node_type str_to_type(char* str) {
  if (strcmp(str, "CP") == 0) return CP;
  else if (strcmp(str, "MC") == 0) return MC;
  else return JN;
}
