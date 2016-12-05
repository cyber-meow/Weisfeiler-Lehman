/* partition.c */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "partition.h"

partition *partition_create(int n)
{
  partition *P = (partition*)malloc(sizeof(partition));
  assert(P);
  P -> num_elements = n;
  P -> num_sets = 1;
  
  P -> ele_to_par = (box*)calloc(n,sizeof(box));
  assert(P -> ele_to_par);
  P -> sets = (set*)calloc(n+1,sizeof(set));
  assert(P -> sets);
  
  for(int i=0;i<n;++i)
  {
    box *boxi = &(P -> ele_to_par[i]);
    boxi -> set_id = 0;
    boxi -> ele_id = i;
    if(i>0)
      boxi -> last = &(P -> ele_to_par[i-1]);
    if(i<n-1)
      boxi -> next = &(P -> ele_to_par[i+1]);
  }
  P -> sets[0].size = n;
  P -> sets[0].data = &(P -> ele_to_par[0]);
  return P;
}

void partition_destroy(partition *P)
{
  free(P -> ele_to_par);
  free(P -> sets);
  free(P);
}

void partition_move(partition *P, int v, int to)
{
  box *boxv = &(P -> ele_to_par[v]);
  if(boxv -> last)
  {
    if(boxv -> next)
    {
      boxv -> last -> next = boxv -> next;
      boxv -> next -> last = boxv -> last;
    }
    else
      boxv -> last -> next = NULL;
  }
  else
  {
    if(boxv -> next)
    {
      boxv -> next -> last = NULL;
      P -> sets[boxv -> set_id].data = boxv -> next;
    }
    else
      P -> sets[boxv -> set_id].data = NULL;
  }
  -- P -> sets[boxv -> set_id].size;
  if(P -> sets[to].data)
    P -> sets[to].data -> last = boxv;
  boxv -> next = P -> sets[to].data;
  boxv -> last = NULL;
  boxv -> set_id = to;
  P -> sets[to].data = boxv;
  ++ P -> sets[to].size;
}

set *set_copy(const set *S)
{
  set *S_cp = (set*)calloc(1,sizeof(set));
  assert(S_cp);
  S_cp -> size = S -> size;
  if(S -> data)
  {
    box *current = S -> data;
    assert(current -> last == NULL);
    box *curr_cp = (box*)malloc(sizeof(box));
    assert(curr_cp);
    S_cp -> data = curr_cp;
    curr_cp -> last = NULL;
    while(current -> next)
    {
      curr_cp -> set_id = current -> set_id;
      curr_cp -> ele_id = current -> ele_id;
      curr_cp -> next = (box*)malloc(sizeof(box));
      assert(curr_cp -> next);
      curr_cp -> next -> last = curr_cp;
      curr_cp = curr_cp -> next;
      current = current -> next;
    }
    curr_cp -> set_id = current -> set_id;
    curr_cp -> ele_id = current -> ele_id;
    curr_cp -> next = NULL;
  }
  return S_cp;
}

void set_destroy(set *S)
{
  if(S -> data)
  {
    box *b = S -> data;
    assert(b -> last == NULL);
    while(b)
    {
      box *current = b;
      b = b -> next;
      free(current);
    }
  }
  free(S);
}

void set_print(const set *S)
{
  if(S -> data)
  {
    box* b = S -> data;
    assert(b -> last == NULL);
    printf("{");
    while(b -> next)
    {
      printf("%d, ", b -> ele_id);
      b = b -> next;
    }
    printf("%d}", b -> ele_id);
  }
  else
    printf("{}");
}

/* C'est une fonction fait particulier pour le backtracking. On parcourt 
   l'ensemble et si on trouve a une solution on le dit immédiatement */
bool set_iter(const set *S, bool (*f)(int v))
{
  if(S -> data)
  {
    box *b = S -> data;
    assert(b -> last == NULL);
    while(b)
    {
      bool is_iso = f(b -> ele_id);
      if(is_iso)
        return True;
      b = b -> next;
    }
  }
  return False;
}

partition *partition_copy(const partition *P)
{
  int n = P -> num_elements;
  partition *P_ = partition_create(n);
  P_ -> num_sets = P -> num_sets;
  for(int i=0;i<n;++i)
  {
    int set_of_i = P ->  ele_to_par[i].set_id;
    partition_move(P_, i, set_of_i);
  }
  return P_;
}

partition *get_partition(FILE *file)
{
  int n, k;
  fscanf(file, "%d", &n);
  fscanf(file, "%d", &k);
  partition *P = partition_create(n);
  
  for(int i=0;i<k;++i)
  {
    int size;
    fscanf(file, "%d", &size);
    for(int j=0;j<size;++j)
    {
      int e;
      fscanf(file, "%d", &e);
      partition_move(P, e, i);
    }
  }

  return P;
}

void partition_print(const partition *P)
{
  int n = P -> num_elements;
  printf("A partition of %d elements into %d sets\n", n, P -> num_sets);
  for(int i=0;i<P->num_sets;++i)
  {
    if(P -> sets[i].data)
    {
      printf("set %d: ",i);
      set_print(&(P -> sets[i]));
      printf("\n");
    }
  }
}

bool compatible(const partition *P1, const partition *P2)
{
  int n = P1 -> num_elements;
  if(P2 -> num_elements != P1 -> num_elements)
    return False;
  for(int i=0;i<n+1;++i)
  {
    if(P1 -> sets[i].size != P2 -> sets[i].size)
      return False;
  }
  return True;
}
