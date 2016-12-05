/* partition.h */

/* On va implémenter une partition de n éléments en k<=n parties
   disjointss, k peut être modifié alors que n est fixé une fois 
   que la partition est créee (ensemble sous-jacent)
   Cette implémentation permet de:
   1. accéder à la partie correspondant à un élément en temps constant
   2. bouge un élément à une autre partie en temps constante
   3. parcourir chaque partie en temps linéaire de sa taille */

#ifndef _PARTITION_H_
#define _PARTITION_H_

#include "array.h"  /* bool */

typedef struct _box
{
  int set_id, ele_id;
  struct _box *last ,*next;
} box;

/* data: 1er élément de la partie, par convention data -> last = NULL */
typedef struct
{
  int size;
  box *data;
} set;

typedef struct
{
  int num_elements, num_sets;
  box *ele_to_par;      /* à chaque élément associe le numéro de sa partie */
  set *sets;            /* les parties */
} partition;

partition *partition_create(int n);
/* partition_destroy: 
   pour partition_create, partition_copy, get_partition, stable_partition */
void partition_destroy(partition *P);
void partition_move(partition *P, int v, int to);

set *set_copy(const set *S);
/* set_destroy: pour set_copy */
void set_destroy(set *S);
void set_print(const set *S);
bool set_iter(const set *S, bool (*f)(int v));

partition *partition_copy(const partition *P);
partition *get_partition(FILE *file);
void partition_print(const partition *P);
bool compatible(const partition *P1, const partition *P2);


#endif
