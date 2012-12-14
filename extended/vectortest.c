/* 
 * File:   vectortest.c
 * Author: thl5
 *
 * Created on 14 December 2012, 10:10
 */

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

void test_get_simple(Vector* vector) {
  int ret_val;
  printf("Getting first elem: ");
  Vector_get(vector, 0, &ret_val);
  printf("%d Size: %d Capacity: %d\n", ret_val, vector->size, vector->capacity);
}

void test_simple() {
  Vector* vector = Vector_new(sizeof(int), NULL);
  test_add_simple(vector);
  printf("\n");
  test_get_simple(vector);
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

void test_get_complex(Vector* vector) {
  char* str;
  printf("Getting first elem: ");
  Vector_get(vector, 0, &str);
  printf("%s Size: %d Capacity: %d\n", str, vector->size, vector->capacity);
}

void test_complex() {
  Vector* vector = Vector_new(sizeof(char**), dispose_str);
  test_add_complex(vector);
  printf("\n");
  test_get_complex(vector);
  /* no real need to test out of bounds again */
  Vector_dispose(vector);
}

/*
 *
 * Uncomment this and comment main to run
int main(int argc, char** argv) {
  printf("\n\nTesting simple\n\n");
  test_simple();
  printf("\n\nTesting complex\n\n");
  test_complex();
  return (EXIT_SUCCESS);
}
*/
