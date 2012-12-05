#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

void test_add(Vector* vector) {
  int i = 0;
  for (i = 1; i < 11; i++) {
    printf("Adding: %d ", i);
    Vector_add(vector, &i);
    printf(" Size: %d Capacity: %d\n", vector->size, vector->capacity);
  }
}

void test_remove(Vector* vector) {
  int ret_val;
  printf("Removing first elem: ");
  Vector_remove(vector, 0, &ret_val);
  printf("%d Size: %d Capacity: %d\n", ret_val, vector->size, vector->capacity);
  printf("Removing last elem: ");
  Vector_remove(vector, vector->size - 1, &ret_val);
  printf("%d Size: %d Capacity: %d\n", ret_val, vector->size, vector->capacity);
}

void test_get(Vector* vector) {
  int ret_val;
  printf("Getting first elem: ");
  Vector_get(vector, 0, &ret_val);
  printf("%d Size: %d Capacity: %d\n", ret_val, vector->size, vector->capacity);
}

void test_out_of_bounds(Vector* vector) {
  int ret_val;
  printf("Trying vector->size: ");
  Vector_remove(vector, vector->size, &ret_val);
  printf("\n");
  printf("Trying -1: ");
  Vector_remove(vector, vector->size, &ret_val);
  printf("\n");
}

int main(int argc, char* argv[]) {
  Vector* vector = Vector_new(sizeof(int), NULL);
  test_add(vector);
  printf("\n");
  test_remove(vector);
  printf("\n");
  test_get(vector);
  printf("\n");
  test_out_of_bounds(vector);
  Vector_dispose(vector);
  return EXIT_SUCCESS;
}
