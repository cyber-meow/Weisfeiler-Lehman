/* array.h */

#ifndef _ARRAY_H_
#define _ARRAY_H_

typedef enum {False, True} bool;

typedef struct
{
  int maxsize, length;
  int *data;
} array;

array *array_create(int n);
array *get_array(FILE *file);
/* array_destroy doit être appeler pour la valeur renvoyé par
   array_create, random_permutation, get_array et
   isomorphisme(_...), configuration de wh.h */
void array_destroy(array *arr);

void array_append(array *arr, int i);
void array_pop(array *arr);
void array_fill(array *arr, int i); 

bool array_equal(const array *arr1, const array *arr2);
void array_sort(array *arr);

void array_print(const array *arr);
void array_print_simple(const array *arr);
void print_permutation(const array *arr);
array *random_permutation(int n);

#endif
