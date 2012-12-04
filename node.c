/*
 * node
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {CP, MC, JN} node_type;

typedef struct Node {
  int id;
  node_type type;
  struct Node* next;
} Node;

typedef struct Node_list {
  Node* head;
} Node_list;

void Node_insert_r(Node* head, Node* new_node) {
  if (head->next == NULL) {
    head->next = new_node;
  } else {
    Node_insert_r(head->next, new_node);
  }
}

void Node_insert(Node_list* list, Node* new_node) {
  if (list->head == NULL) {
    list->head = new_node;
  } else {
    Node_insert_r(list->head, new_node);
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
  }
}

/* The following function was taken from
 * http://stackoverflow.com/questions/314401/how-to-read-a-line-from-the-console-in-c
 * It's probably overkill for this assignment but it works! */
char* getline_foo() {
  char* line = malloc(100), * linep = line;
  size_t lenmax = 100, len = lenmax;
  int c;

  if(line == NULL)
    return NULL;

  for(;;) {
    c = fgetc(stdin);
    if(c == EOF)
      break;

    if(--len == 0) {
      len = lenmax;
      char * linen = realloc(linep, lenmax *= 2);

      if(linen == NULL) {
        free(linep);
        return NULL;
      }
      line = linen + (line - linep);
      linep = linen;
    }

    if((*line++ = c) == '\n')
      break;
  }
  *line = '\0';
  return linep;
}

int main(int argc, char* argv[]) {
  char* filename;
  char line[80];
  char* token;
  FILE* fp;
  Node_list* list = malloc(sizeof(Node_list));
  list->head = NULL;

  printf("Input node filename: ");
  filename = getline_foo();
  strtok(filename, "\n");

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
    Node_insert(list, new_node);
  }

  fclose(fp);
  Node_print(list);
  return EXIT_SUCCESS;
}
