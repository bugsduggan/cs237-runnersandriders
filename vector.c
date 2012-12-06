/*
 * vector
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#include <stdlib.h>
#include <string.h>
#include "vector.h"

static void Vector_grow(Vector* vector) {
  void* new_base;
  vector->capacity *= 2;
  new_base = malloc(vector->elem_size * vector->capacity);
  memcpy(new_base, vector->base, vector->elem_size * vector->size);
  free(vector->base);
  vector->base = new_base;
}

Vector* Vector_new(int elem_size, void (*dispose_fn)(void*)) {
  Vector* vector = malloc(sizeof(Vector));
  vector->elem_size = elem_size;
  vector->dispose_fn = dispose_fn;
  vector->size = 0;
  vector->capacity = MIN_CAPACITY;
  vector->base = malloc(vector->elem_size * vector->capacity);
  return vector;
}

void Vector_dispose(Vector* vector) {
  int i = 0;
  if (vector) {
    if (vector->dispose_fn) {
      for (i = 0; i < vector->size; i++) {
        vector->dispose_fn((char*)vector->base + vector->elem_size * i);
      }
    }
    if (vector->base) free(vector->base);
    free(vector);
  }
}

void Vector_add(Vector* vector, void* elem) {
  void* target;
  if (vector) {
    if (vector->size == vector->capacity) Vector_grow(vector);
    target = (char*) vector->base + vector->elem_size * vector->size++;
    memcpy(target, elem, vector->elem_size);
  }
}

void Vector_remove(Vector* vector, int index, void* ret_val) {
  if (vector) {
    if (index >= 0 && index < vector->size) {
      memcpy(ret_val, (char*)vector->base + vector->elem_size * index, vector->elem_size);
      if (index + 1 < vector->size) { /* not last elem */
        memmove((char*)vector->base + vector->elem_size * index,
            (char*)vector->base + vector->elem_size * (index + 1),
            vector->elem_size * (vector->size - index - 1));
      }
      vector->size--;
    }
  }
}

void Vector_get(Vector* vector, int index, void* ret_val) {
  if (vector) {
    if (index >= 0 && index < vector->size)
      memcpy(ret_val, (char*)vector->base + vector->elem_size * index, vector->elem_size);
  }
}

void Vector_get_last(Vector* vector, void* ret_val) {
  if (vector) {
    if (vector->size > 0) {
      memcpy(ret_val, (char*)vector->base + vector->elem_size * (vector->size - 1), vector->elem_size);
    }
  }
}

int Vector_size(Vector* vector) {
  return vector->size;
}
