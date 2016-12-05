/* test.c */
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "weisfeiler-lehman.h"

int *rn()
{
  return NULL;
}

int main()
{
  /*
  list *l = list_create();
  for(int i=0;i<10;++i)
    list_prepend(l,i);
  list_print(l);
  list_destroy(l);
  */

  
  //graph *g = random_graph(50,0.5);
  graph *g = generate_regular_graph(20,5,50);
  graph *g2 = generate_regular_graph(100,50,500);
  graph_print_simple(g);
  //array *pho = random_permutation(1000);
  //print_permutation(pho);
  //graph *g2 = graph_transform(g,pho);
  //graph_print(g2);
  

  //bool b = verify_isomorphism(g,g2,pho);
  //printf("%d\n",b);
  
  /*
  graph *gr = generate_regular_graph(1000,50,100);
  graph_print(gr);
  */
    
  array *iso = isomorphism(g,g2);
  //assert(iso);
  print_permutation(iso);
  //array *iso2 = isomorphism_degree(g,g2);
  //assert(iso2);
  //print_permutation(iso2);
  

  //array *iso3 = isomorphism_naif(g,g2);
  //assert(iso3);
  //print_permutation(iso3);
  
  /*
  partition *P1 = partition_create(10);
  partition *P1_ = partition_create(10);
  partition *P2 = partition_create(10);
  partition *P2_ = partition_create(10);
  assert(stable_partition2(g, g2, P1, P2, P1_, P2_));
  partition_print(P1_);
  partition_print(P2_);
  
  partition_destroy(P1);
  partition_destroy(P1_);
  partition_destroy(P2);
  partition_destroy(P2_);
  */

  //array_destroy(pho);
  if(iso) array_destroy(iso);
  //array_destroy(iso2);
  //array_destroy(iso3);
  
  graph_destroy(g);
  graph_destroy(g2);
  //graph_destroy(gr);
  
  return 0;
}
