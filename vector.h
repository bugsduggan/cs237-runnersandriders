/* 
 * File:   vector.h
 * Author: thl5
 *
 * Created on 14 December 2012, 10:04
 */

#ifndef VECTOR_H
#define	VECTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MIN_CAPACITY 4

/*
 * This was designed based on principles from Stanford University's CS107 lectures
 * available on YouTube. In particular the lecture where they describe implementing
 * a generic stack in C.
 *
 * This code (and that in vector.c) is, however all my own work
 */

typedef struct Vector {
  size_t elem_size;          /* the size of the elements being stored */
  void (*dispose_fn)(void*); /* a function which knows how to dispose of the elements
                                being stored */
  int size;                  /* the logical size of the vector */
  int capacity;              /* the capacity of the vector */
  void* base;                /* a pointer to the start of the data */
} Vector;

Vector* Vector_new(int elem_size, void (*dispose_fn)(void*));
void Vector_dispose(Vector* vector);

/* add an element to the end */
void Vector_add(Vector* vector, void* elem);
/* get the value of an element at index */
void Vector_get(Vector* vector, int index, void* ret_val);
/* sorts the data in the vector according to the comparison function */
void Vector_sort(Vector* vector, int (*comp_fn)(void*, void*));
/* the size of the vector */
int Vector_size(Vector* vector);

#ifdef	__cplusplus
}
#endif

#endif	/* VECTOR_H */

