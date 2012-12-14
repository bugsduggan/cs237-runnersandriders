/*
 * vector
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <string.h>

#include "vector.h"

/*
 * Note: we can't do pointer arithmetic on void* pointers, the compiler doesn't
 * know how big the thing it's pointing to is!
 * So we use a cast to a char* (which is one byte) to multiply the pointer
 * arithmetic by 1 and point to (hopefully) the correct location.
 */

/*
 * private functions
 */

static void Vector_grow(Vector* vector) {
  void* new_base;

  /* use a doubling strategy when the vector is full
   * this means the vector will only resize every 2^n inserts */
  vector->capacity *= 2;
  /* allocate new space */
  new_base = malloc(vector->elem_size * vector->capacity);
  /* copy the old data over */
  memcpy(new_base, vector->base, vector->elem_size * vector->size);
  /* free the old space */
  free(vector->base);
  /* update the local pointer */
  vector->base = new_base;
}

/*
 * functions declared in vector.h
 */

/* create a new vector object */
Vector* Vector_new(int elem_size, void (*dispose_fn)(void*)) {
  Vector* vector = malloc(sizeof(Vector));

  vector->elem_size = elem_size;
  vector->dispose_fn = dispose_fn;
  vector->size = 0;
  vector->capacity = MIN_CAPACITY;
  vector->base = malloc(vector->elem_size * vector->capacity);

  return vector;
}

/* dispose of the vector */
void Vector_dispose(Vector* vector) {
  int i = 0;

  if (vector) {
    if (vector->dispose_fn) {
      /* this will only be used if a disposal function has been passed in */
      for (i = 0; i < vector->size; i++) {
        vector->dispose_fn((char*)vector->base + vector->elem_size * i);
      }
    }

    if (vector->base) free(vector->base);
    free(vector);
  }
}

/* add an element to the vector */
void Vector_add(Vector* vector, void* elem) {
  void* target;

  if (vector) {
    if (vector->size == vector->capacity) Vector_grow(vector);

    /* target will point to the next empty space in the vector */
    target = (char*) vector->base + vector->elem_size * vector->size++;
    /* copy the data in */
    memcpy(target, elem, vector->elem_size);
  }
}

/* copy the value at index into ret_val */
void Vector_get(Vector* vector, int index, void* ret_val) {
  if (vector) {
    if (index >= 0 && index < vector->size)
      memcpy(ret_val, (char*)vector->base + vector->elem_size * index, vector->elem_size);
  }
}

/* this implements a generic bubble sort */
void Vector_sort(Vector* vector, int (*comp_fn)(void*, void*)) {
  int i = 0;
  int j = 0;
  void* temp = malloc(vector->elem_size);

  if (vector) {
    for (i = 0; i < vector->size; i++) {
      for (j = 1; j < vector->size; j++) {
        /* a = j-1, b = j */
        if (comp_fn((char*)vector->base + vector->elem_size * (j-1),
              (char*)vector->base + vector->elem_size * j) == 1) { /* if a > b */
          /* temp = a */
          memcpy(temp, (char*)vector->base + vector->elem_size * (j-1), vector->elem_size);
          /* a = b */
          memcpy((char*)vector->base + vector->elem_size * (j-1),
              (char*)vector->base + vector->elem_size * j,  vector->elem_size);
          /* b = temp */
          memcpy((char*)vector->base + vector->elem_size * j, temp, vector->elem_size);
        }
      }
    }
    free(temp);
  }
}

/* just return the size */
int Vector_size(Vector* vector) {
  /* I could let the client use vector->size directly, but I think it's more
   * consistent to get them to call a function */
  return vector->size;
}
