/*
 * node
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef NODE_THL5_H
#define NODE_THL5_H

typedef enum {CP, MC, JN} node_type;

typedef struct Node {
  int id;
  node_type type;
} Node;

Vector* nodes_read(FILE* fp);
void nodes_dispose(Vector* nodes);
Node* node_from_id(Vector* nodes, int id);

#endif
