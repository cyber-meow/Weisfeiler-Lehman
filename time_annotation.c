/* time_annotation.c */
/* Ce fichier sert uniquement à annoter le temps de calcul */

/* Pour une fonction <func>, notons n la taille de son entrée
   (une notion assez vague, à traiter cas par cas), la valeur renvoyé
   par <func>_bound ne depend que de n, et si on le note f(n), ceci
   implique que <func> s'exécute en temps O(f(n)).
   Ex: si <func>_bound renvoie tout le temps 1, <func> s'exécute en
   en temps constant.*/

#include <stdio.h>
#include <math.h>

#include "weisfeiler-lehman.h"


/* list.c */

int list_create_bound(){
  return 1;
}

int list_destroy_bound(list *lis){
  return lis -> length;
}

int list_prepend_bound(list *lis, int i){
  return 1;
}

int list_delete_bound(list *lis, int i){
  return lis -> length;
}

int list_print_bound(const list* lis){
  return lis -> length;
}

/* Comme on ne connaît pas forcément la compléxité de la fonction action
   (qui peut à priori être n'importe quoi), on compte seulement le nombre
   d'opérations effectuées en dehors de action.
   Pour avoir quelque chose de plus précise, il faut définir une structure
   particulier qui contient à la fois la fonction et l'information sur
   sa complexité. */
int list_iter_bound(const list *lis, void (*action)(int)){
  return lis -> length;
}

int list_at_bound(const list *lis, int pos){
  return lis -> length;
}


/* array.c */

int array_create_bound(int n){
  return 1;
}

int get_array_bound(FILE *file)
{
  int n;
  fscanf(file, "%d", &n);
  return n;
}

int array_destroy_bound(array *arr){
  return 1;
}

int array_append_bound(array *arr, int i){
  return 1;
}

int array_pop_bound(array *arr){
  return 1;
}

int array_fill_bound(array *arr){
  return arr -> maxsize;
}

int array_equal_bound(const array *arr1, const array *arr2)
{
  int n1 = arr1 -> length;
  int n2 = arr2 -> length;
  return n1<n2?n1:n2;
}

int compare_bound(const void *arg1, const void *arg2){
  return 1;
}

int array_sort_bound(array *arr)
{
  int n = arr -> length;
  return (int) n*log(n);
}

int array_print_bound(const array *arr){
  return arr -> length;
}

int array_print_simple_bound(const array *arr){
  return arr -> length;
}

/* Il arrive qu'on passe un pointeur null à cette fonction*/
int print_permutation_bound(const array *arr){
  return arr?1:(arr -> length);
}

int random_permutation_bound(int n){
  return n;
}


/* graph.c */

int graph_create_bound(int n){
  return n;
}

int graph_destroy_bound(graph *G){
  return G -> num_vertices + G -> num_edges;
}

int add_edge_bound(graph *G, int u, int v){
  return 1;
}

int delete_edge_bound(graph *G, int u, int v){
  return G -> num_vertices;
}

int graph_print_bound(const graph *G){
  return G -> num_vertices + G -> num_edges;
}

int graph_print_simple_bound(const graph *G){
  return G -> num_vertices + G -> num_edges;
}

int random_graph_bound(int n, float p){
  return n*n;
}

int generate_regular_graph_bound(int n, int k, int times){
  return n*k + n*times;
}

int graph_transform_bound(const graph *G, array *pho){
  return G -> num_vertices + G -> num_edges;
}

int get_graph_bound(FILE *file)
{
  int n, m;
  fscanf(file, "%d", &m);
  fscanf(file, "%d", &n);
  return n + m;
}


/* partition.c */

int partition_create_bound(int n){
  return n;
}

int partition_destroy_bound(partition *P){
  return 1;
}

int partition_move_bound(partition *P, int v, int to){
  return 1;
}

int set_copy_bound(const set *S){
  return S -> size;
}

int set_destroy_bound(set *S){
  return S -> size;
}

int set_print_bound(const set *S){
  return S -> size;
}

/* la remarque pour list_iter s'applique ici aussi */
int set_iter_bound(const set *S, bool (*f)(int v)){
  return S -> size;
}

int partition_copy_bound(const partition *P){
  return P -> num_elements;
}

int get_partition_bound(FILE *file)
{
  int n;
  fscanf(file, "%d", &n);
  return n;
}

int partition_print_bound(const partition *P){
  return P -> num_elements;
}

int compatible_bound(const partition *P1, const partition *P2)
{
  int n1 = P1 -> num_elements;
  int n2 = P2 -> num_elements;
  return n1<n2?n1:n2;
}


/* Avant de continuer, quelques fonctions utiles */

int smaller_num_vertices(const graph *G1, const graph *G2)
{
  int n1 = G1 -> num_vertices;
  int n2 = G2 -> num_vertices;
  return n1<n2?n1:n2;
}

int smaller_num_edges(const graph *G1, const graph *G2)
{
  int n1 = G1 -> num_edges;
  int n2 = G2 -> num_edges;
  return n1<n2?n1:n2;
} 

int factorial(unsigned int n){
  return n?factorial(n-1):1;
}


/* weisfeiler-lehman.c */

int verify_isomorphism_bound
  (const graph *G1, const graph *G2, const array *pho)
{
  int n = smaller_num_vertices(G1,G2);
  return n*n;
}

int isomorphism_naif_bound(const graph *G1, const graph *G2)
{
  int n = smaller_num_vertices(G1,G2);
  return factorial(n)*n*n;
}

int isomorphism_degree_bound(const graph *G1, const graph *G2)
{
  int n = smaller_num_vertices(G1,G2);
  return factorial(n)*n*n;
}

int configuration_bound(const list* lis, const partition *P)
{
  int n = lis -> length;
  return (int) n*log(n);
}

/* Une borne beaucoup plus simple à obenir c'est n^4, le résultat ici
   c'est moins évident */
int stable_partition_bound(const graph *G, const partition *P)
{
  int n = G -> num_vertices;
  int m = G -> num_edges;
  return (int) n*(n+m*log(n));
}

int stable_partition2_bound(const graph *G1, const graph *G2,
  partition *P1, partition *P2, partition *P1_, partition *P2_)
{
  int n = smaller_num_vertices(G1,G2);
  int m = smaller_num_edges(G1,G2);
  return (int) n*(n+m*log(n));
}

/* Avec le heuristique, l'algorithme marche sûrement beaucoup mieux dans
   la plupart des cas, mais dû à la difficulté d'une analyse complète et
   correcte, je me contente de donner seulement la complexité dans le pire
   des cas ici. On remarque quand même qu'on a n!*(n+m*log(n)) au lieu de
   n!*n*(n+m*log(n)) */
int Weisfeirler_Lehman_bound(const graph *G1, const graph *G2,
    const partition *P1, const partition *P2, array *pho)
{
  int n = smaller_num_vertices(G1,G2);
  int m = smaller_num_edges(G1,G2);
  return (int) factorial(n)*(n+m*log(n));
}

int isomorphism_bound(const graph *G1, const graph *G2)
{
  int n = smaller_num_vertices(G1,G2);
  int m = smaller_num_edges(G1,G2);
  return (int) factorial(n)*(n+m*log(n));
}

int main(){ return 0;}
