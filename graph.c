/* graph.c */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "graph.h"

graph *graph_create(int n)
{
  graph *G = (graph*)malloc(sizeof(graph));
  assert(G);
  G -> num_vertices = n;
  G -> num_edges = 0;
  G -> adj_list_arr = (list**)malloc(n*sizeof(list*));
  assert(G -> adj_list_arr);
  G -> adj_mat = (bool**)malloc(n*sizeof(bool*));
  assert(G -> adj_mat);
  for(int i=0;i<n;++i)
  {
    G -> adj_list_arr[i] = list_create();
    G -> adj_mat[i] = (bool*)calloc(n,sizeof(bool));
    assert(G -> adj_mat[i]);
  }
  return G;
}

void graph_destroy(graph *G)
{
  int n = G -> num_vertices;
  for(int i=0;i<n;++i)
  {
    list_destroy(G -> adj_list_arr[i]);
    free(G -> adj_mat[i]);
  }
  free(G -> adj_list_arr);
  free(G -> adj_mat);
  free(G);
}

void add_edge(graph *G, int u, int v)
{
  assert(!G -> adj_mat[u][v]);
  assert(u != v);
  list_prepend(G -> adj_list_arr[u], v);
  list_prepend(G -> adj_list_arr[v], u);
  G -> adj_mat[u][v] = True;
  G -> adj_mat[v][u] = True;
  ++ G -> num_edges;
}

void delete_edge(graph *G, int u, int v)
{
  assert(G -> adj_mat[u][v]);
  list_delete(G -> adj_list_arr[u], v);
  list_delete(G -> adj_list_arr[v], u);
  G -> adj_mat[u][v] = False;
  G -> adj_mat[v][u] = False;
  -- G -> num_edges;
}

void graph_print(const graph *G)
{
  int n = G -> num_vertices;
  printf("This graph contains %d vertices\n",n);
  for(int i=0;i<n;++i)
  {
    printf("vertex %d: ",i);
    list_print(G -> adj_list_arr[i]);
  }
}

void graph_print_simple(const graph *G)
{
  int n = G -> num_vertices;
  printf("%d %d\n", n, G -> num_edges);
  for(int i=0;i<n;++i)
  {
    list *adj_lis = G -> adj_list_arr[i];
    printf("%d", adj_lis -> length);
    node *curr = adj_lis -> head;
    while(curr)
    {
      printf(" %d", curr -> data);
      curr = curr -> next;
    }
    printf("\n");
  }
}

/* Créer un graphe où chaque arête apparaît avec probabilité p*/
graph *random_graph(int n, float p)
{
  graph *G = graph_create(n);
  srand((unsigned int)time(NULL));
  for(int i=0;i<n;++i)
  {
    for(int j=0;j<i;++j)
    {
      if((double)rand()/(double)RAND_MAX < p)
        add_edge(G,i,j);
    }
  }
  return G;
}

/* Générer un graphe k-régulier, 
   Par contre c'est difficile d'avoir qqch qui est parfaitement aléatoire */
graph *generate_regular_graph(int n, int k, int times)
{
  assert(n*k%2 == 0);
  assert(k < n);
  
  /* d'abort générer un graph d'une forme fixée */
  graph *G = graph_create(n);
  int m = k/2;
  for(int i=0;i<n;++i)
  {
    for(int j=1;j<m+1;++j)
    {
      int v = (i + j)%n;
      add_edge(G, i, v);
    }
  }
  if(k%2)
  {
    for(int i=0;i<(n/2);++i)
      add_edge(G, i, i + n/2);
  }

  /* des modifications */
  srand((unsigned)time(NULL));
  for(int i=0;i<times;++i)
  {
    int u1 = rand()%n;
    int u2 = rand()%n;
    int l1 = G -> adj_list_arr[u1] -> length;
    int l2 = G -> adj_list_arr[u2] -> length;
    if(u1 == u2 || l1 == 0|| l2 == 0) 
      continue;
    int v1_pos = rand()%l1;
    int v2_pos = rand()%l2;
    int v1 = list_at(G -> adj_list_arr[u1], v1_pos);
    int v2 = list_at(G -> adj_list_arr[u2], v2_pos);
    if(u1 == v2 || u2 == v1 || v1 == v2)
      continue;
    if(G -> adj_mat[u1][v2] || G -> adj_mat[u2][v1])
      continue;
    add_edge(G, u1, v2);
    add_edge(G, u2, v1);
    delete_edge(G, u1, v1);
    delete_edge(G, u2, v2);
  }
  
  return G;
}

/* L'image du graphe G après la permutation pho */
graph *graph_transform(const graph *G, array *pho)
{
  int n = G -> num_vertices;
  assert(pho -> length == n);
  int *phod = pho -> data;
  graph *Gn = graph_create(n);
  for(int i=0;i<n;++i)
  {
    void add(int j)
    {
      if(j<i)
        add_edge(Gn,phod[i],phod[j]);
    }
    list_iter(G -> adj_list_arr[i], &add);
  }
  return Gn;
}

graph *get_graph(FILE *file)
{
  int n, m;

  fscanf(file, "%d", &n);
  fscanf(file, "%d", &m);
  graph *G = graph_create(n);

  for(int i = 0;i<n;++i)
  {
    int deg;
    fscanf(file, "%d", &deg);
    for(int j=0;j<deg;++j)
    {
      int v;
      fscanf(file, "%d", &v);
      if(i<v)
        add_edge(G, i, v);
    }
  }

  return G;
}
