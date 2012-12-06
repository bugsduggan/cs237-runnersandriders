/*
 * data
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef DATA_THL5_H
#define DATA_THL5_H

/*
 * Data structures
 */

/* Node stuff */
typedef enum {CP, MC, JN} node_type;

typedef struct Node {
  int id;
  node_type type;
} Node;

/*
 * Functions
 */

/* Node stuff */
Vector* nodes_read(char* filename); /* populates vector with pointers to nodes */
void nodes_dispose(Vector* nodes); /* disposes of a vector of nodes */

#endif
