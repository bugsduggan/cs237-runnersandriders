#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "util.h"

void test_add_simple(Vector* vector) {
  int i = 0;
  for (i = 1; i < 11; i++) {
    printf("Adding: %d ", i);
    Vector_add(vector, &i);
    printf(" Size: %d Capacity: %d\n", vector->size, vector->capacity);
  }
}

void test_remove_simple(Vector* vector) {
  int ret_val;
  printf("Removing first elem: ");
  Vector_remove(vector, 0, &ret_val);
  printf("%d Size: %d Capacity: %d\n", ret_val, vector->size, vector->capacity);
  printf("Removing last elem: ");
  Vector_remove(vector, vector->size - 1, &ret_val);
  printf("%d Size: %d Capacity: %d\n", ret_val, vector->size, vector->capacity);
}

void test_get_simple(Vector* vector) {
  int ret_val;
  printf("Getting first elem: ");
  Vector_get(vector, 0, &ret_val);
  printf("%d Size: %d Capacity: %d\n", ret_val, vector->size, vector->capacity);
}

void test_out_of_bounds_simple(Vector* vector) {
  int ret_val;
  printf("Trying vector->size: ");
  Vector_remove(vector, vector->size, &ret_val);
  printf("Didn't crash!\n");
  printf("Trying -1: ");
  Vector_remove(vector, vector->size, &ret_val);
  printf("Didn't crash!\n");
}

void test_get_last_simple(Vector* vector) {
  int ret_val;
  printf("Getting last: ");
  Vector_get_last(vector, &ret_val);
  printf("%d\n", ret_val);
}

void test_simple() {
  Vector* vector = Vector_new(sizeof(int), NULL);
  test_add_simple(vector);
  printf("\n");
  test_remove_simple(vector);
  printf("\n");
  test_get_simple(vector);
  printf("\n");
  test_out_of_bounds_simple(vector);
  printf("\n");
  test_get_last_simple(vector);
  Vector_dispose(vector);
}

void dispose_str(void* str) {
  char* foo = *(char**)str;
  if (foo) free(foo);
}

void test_add_complex(Vector* vector) {
  char* str;
  printf("Adding 'Fred': ");
  str = strdup("Fred");
  Vector_add(vector, &str);
  printf("Size: %d Capacity: %d\n", vector->size, vector->capacity);
  printf("Adding 'Wilma': ");
  str = strdup("Wilma");
  Vector_add(vector, &str);
  printf("Size: %d Capacity: %d\n", vector->size, vector->capacity);
  printf("Adding 'Barney': ");
  str = strdup("Barney");
  Vector_add(vector, &str);
  printf("Size: %d Capacity: %d\n", vector->size, vector->capacity);
  printf("Adding 'Pebbles': ");
  str = strdup("Pebbles");
  Vector_add(vector, &str);
  printf("Size: %d Capacity: %d\n", vector->size, vector->capacity);
  printf("Adding 'Bam-Bam': ");
  str = strdup("Bam-Bam");
  Vector_add(vector, &str);
  printf("Size: %d Capacity: %d\n", vector->size, vector->capacity);
}

void test_remove_complex(Vector* vector) {
  char* str;
  printf("Removing first elem: ");
  Vector_remove(vector, 0, &str);
  printf("%s Size: %d Capacity: %d\n", str, vector->size, vector->capacity);
  free(str); /* It's now my responsibility to free this */
}

void test_get_complex(Vector* vector) {
  char* str;
  printf("Getting first elem: ");
  Vector_get(vector, 0, &str);
  printf("%s Size: %d Capacity: %d\n", str, vector->size, vector->capacity);
}

void test_get_last_complex(Vector* vector) {
  char* str;
  printf("Getting last elem: ");
  Vector_get_last(vector, &str);
  printf("%s Size: %d Capacity: %d\n", str, vector->size, vector->capacity);
}

void test_complex() {
  Vector* vector = Vector_new(sizeof(char**), dispose_str);
  test_add_complex(vector);
  printf("\n");
  test_remove_complex(vector);
  printf("\n");
  test_get_complex(vector);
  /* no real need to test out of bounds again */
  printf("\n");
  test_get_last_complex(vector);
  Vector_dispose(vector);
}

int main(int argc, char* argv[]) {
  printf("\n\nTesting simple\n\n");
  test_simple();
  printf("\n\nTesting complex\n\n");
  test_complex();
  return EXIT_SUCCESS;
}
