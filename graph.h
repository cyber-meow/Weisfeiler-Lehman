/* graph.h */
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "list.h"
#include "array.h"

/* Graph non-orienté */
typedef struct
{
  int num_vertices, num_edges;
  list **adj_list_arr;
  bool **adj_mat;
} graph;

graph *graph_create(int n);
/* graph_destroy: 
   pour graph_create, random_graph, generate_regular_graph, graph_transform */
void graph_destroy(graph* G);

void add_edge(graph *G, int u, int v);
void delete_edge(graph *G, int u, int v);
void graph_print(const graph *G);
/* le format pour être lu en entrée*/
void graph_print_simple(const graph *G);

graph *random_graph(int n, float p);
graph *generate_regular_graph(int n, int k, int times);
graph *graph_transform(const graph *G, array *pho);
graph *get_graph(FILE *file);

#endif
