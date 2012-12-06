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

/*
 * Vector* vector = Vector_new(sizeof(int), NULL);
 * Vector* vector = Vector_new(sizeof(char**), dispose_fn);
 */
Vector* Vector_new(int elem_size, void (*dispose_fn)(void*));
void Vector_dispose(Vector* vector);

/*
 * Vector_add(vector, &foo);
 */
void Vector_add(Vector* vector, void* elem);
/*
 * Vector_remove(vector, ind, &ret_val);
 */
void Vector_remove(Vector* vector, int index, void* ret_val);
void Vector_get(Vector* vector, int index, void* ret_val);
void Vector_get_last(Vector* vector, void* ret_val);

int Vector_size(Vector* vector);

#endif
