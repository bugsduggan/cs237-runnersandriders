/*
 * vector
 *
 * Tom Leaman (thl5@aber.ac.uk)
 */

#ifndef VECTOR_THL_H
#define VECTOR_THL_H

#define MIN_CAPACITY 4

typedef struct Vector {
  size_t elem_size;
  void (*dispose_fn)(void*);
  int size;
  int capacity;
  void* base;
} Vector;

Vector* Vector_new(int elem_size, void (*dispose_fn)(void*));
void Vector_dispose(Vector* vector);

void Vector_add(Vector* vector, void* elem);
void Vector_get(Vector* vector, int index, void* ret_val);
void Vector_sort(Vector* vector, int (*comp_fn)(void*, void*));
int Vector_size(Vector* vector);

#endif
