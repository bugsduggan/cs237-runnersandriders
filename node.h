#ifndef NODE_H
#define NODE_H

typedef enum {CP, MC, JN} node_type;

typedef struct Node {
  int id;
  node_type type;
  struct Node* next;
} Node;

typedef struct Node_list {
  Node* head;
} Node_list;

void Node_insert(Node_list* list, Node* new_node);
Node_list* make_nodes(char* filename);
void Node_destroy(Node_list* list);

#endif
