/* array.c */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <time.h>
#include "array.h"

array *array_create(int n)
{
  array *arr = (array*)malloc(sizeof(array));
  assert(arr);
  arr -> maxsize = n;
  arr -> length = 0;
  arr -> data = (int*)malloc(n*sizeof(int));
  assert(arr -> data);
  return arr;
}

array *get_array(FILE *file)
{
  int n;
  fscanf(file, "%d", &n);
  array *arr = array_create(n);
  arr -> length = n;
  for(int i=0;i<n;++i)
    fscanf(file, "%d", &(arr -> data[i]));
  return arr;
}

void array_destroy(array *arr)
{
  free(arr -> data);
  free(arr);
}

void array_append(array *arr, int i)
{
  if(arr -> length == arr -> maxsize)
  {
    printf("Error: array already filled\nExiting...\n");
    exit(1);
  }
  arr -> data[arr -> length ++] = i;
}

void array_pop(array *arr)
{
  if(arr -> length == 0)
  {
    printf("Error: cannot pop element from empty array\nExiting...\n");
    exit(1);
  }
  -- arr -> length;
}

void array_fill(array *arr, int i)
{
  int n = arr -> maxsize;
  arr -> length = n;
  for(int t=0;t<n;++t)
    arr -> data[t] = i;
}

bool array_equal(const array *arr1, const array *arr2)
{
  if(arr1 -> length != arr2 -> length)
    return False;
  for(int i=0;i<arr1->length;++i)
  {
    if(arr1 -> data[i] != arr2 -> data[i])
      return False;
  }
  return True;
}

int compare(const void *arg1, const void *arg2) { 
  return (*(int*)arg1 - *(int*)arg2); 
}

void array_sort(array *arr){  
  qsort(arr -> data, arr -> length, sizeof(int), compare); 
}

void array_print(const array *arr)
{
  printf("[");
  for(int i=0;i<arr->length;++i)
  {
    if(i > 0)
      printf(", ");
    printf("%d",arr -> data[i]);
  }
  printf("]\n");
}

void array_print_simple(const array *arr)
{
  printf("%d", arr -> length);
  for(int i=0;i<arr->length;++i)
    printf(" %d", arr -> data[i]);
  printf("\n");
}

void print_permutation(const array *arr)
{
  if(arr)
  {
    printf("These two graphs are isomorphic, with permutation:\n");
    printf("(");
    for(int i=0;i<arr->length;++i)
    {
      if(i > 0)
        printf(", ");
      printf("%d -> %d", i, arr -> data[i]);
    }
    printf(")\n");
  }
  else
    printf("These two graphs are not isomorphic\n");
}

/* générer une permutation aléatoire avec Knuth shuffles */
/* L'implémentation utilisant rand()%(n-i) fait que la probabilité
   n'est pas parfaitement uniforme */
array *random_permutation(int n)
{
  srand((unsigned int)time(NULL));
  array *pho = array_create(n);
  pho -> length = n;
  for(int i=0;i<n;++i)
    pho -> data[i] = i;
  for(int i=0;i<n-1;++i)
  {
    int j = rand()%(n-i);
    int tmp = pho -> data[i];
    pho -> data[i] = pho -> data[i+j];
    pho -> data[i+j] = tmp;
  }
  return pho;
}
