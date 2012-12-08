/*
 * node
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>

#include "vector.h"
#include "util.h"
#include "data.h"

/*
 * private functions
 */

/*
 * functions declared in data.h
 */

Node* node_init(char* line) {
  Node* node = malloc(sizeof(Node));

  node->id = 0;
  node->type = CP;

  return node;
}
