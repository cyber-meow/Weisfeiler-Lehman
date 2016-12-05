/* weisfeiler-lehman.c */
/* le fichier qui contient les fonctions principales */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph.h"
#include "partition.h"


/* Vérifie si pho est un isomorphisme de G1 vers G2 */
bool verify_isomorphism(const graph *G1, const graph *G2, const array *pho)
{
  int n = G1 -> num_vertices;
  if((G2 -> num_vertices != n)||(pho -> length != n))
    return False;
  int *phod = pho -> data;
  for(int i=0;i<n;++i)
  {
    for(int j=0;j<i;++j)
    {
      if(G1 -> adj_mat[i][j] != G2 -> adj_mat[phod[i]][phod[j]])
        return False;
    }
  }
  return True;
}


/* backtracking naif, NULL si pas de solution */
array *isomorphism_naif(const graph *G1, const graph *G2)
{
  int n = G1 -> num_vertices;
  if(G2 -> num_vertices != n)
    return NULL;
  /* a dans la partie 0 si a n'est pas encore utilisé */
  partition *markers = partition_create(n);
  markers -> num_sets = 2;
  array *pho = array_create(n);
  
  bool iso_partial()
  {
    int u = pho -> length;
    if(u == n)
      return True;
    set *not_used = set_copy(&(markers -> sets[0]));
    bool choosev(int v)
    {
      int i;
      for(i=0;i<u;++i)
      {
        if(G1 -> adj_mat[i][u] != G2 -> adj_mat[pho -> data[i]][v])
          break;
      }
      if(i == u)
      {
        array_append(pho,v);
        partition_move(markers,v,1);
        bool is_iso = iso_partial();
        if(is_iso)
          return True;
        partition_move(markers,v,0);
        array_pop(pho);
      }
      return False;
    }
    bool is_iso = set_iter(not_used, &choosev);
    set_destroy(not_used);
    return is_iso;
  }

  bool is_iso = iso_partial();
  partition_destroy(markers);
  if(is_iso)
    return pho;
  array_destroy(pho);
  return NULL;
}


/* backtracking avec partitionnement sur les degrés */
array *isomorphism_degree(const graph *G1, const graph *G2)
{
  int n = G1 -> num_vertices;
  if(G2 -> num_vertices != n)
    return NULL;
  
  /* construire les deux partitions */
  partition *g1_degree_P = partition_create(n);
  g1_degree_P -> num_sets = n + 1;
  partition *g2_degree_P = partition_create(n);
  g2_degree_P -> num_sets = n + 1;
  for(int i=0;i<n;++i)
  {
    int g1_i_deg = G1 -> adj_list_arr[i] -> length;
    int g2_i_deg = G2 -> adj_list_arr[i] -> length;
    partition_move(g1_degree_P, i, g1_i_deg);
    partition_move(g2_degree_P, i, g2_i_deg);
  }
  if(!compatible(g1_degree_P, g2_degree_P))
    return NULL;
  
  array *pho = array_create(n);
  bool iso_partial()
  {
    int u = pho -> length;
    if(u == n)
      return True;
    int g1_u_deg = G1 -> adj_list_arr[u] -> length;
    set *possible_choices = set_copy(&(g2_degree_P -> sets[g1_u_deg]));
    bool choosev(int v)
    {
      int i;
      for(i=0;i<u;++i)
      {
        if(G1 -> adj_mat[i][u] != G2 -> adj_mat[pho -> data[i]][v])
          break;
      }
      if(i == u)
      {
        array_append(pho,v);
        /* convetion: partie n pour les éléments déjà utilisé */
        partition_move(g2_degree_P, v, n);
        bool is_iso = iso_partial();
        if(is_iso)
          return True;
        partition_move(g2_degree_P, v, g1_u_deg);
        array_pop(pho);
      }
      return False;
    }
    bool is_iso = set_iter(possible_choices, &choosev);
    set_destroy(possible_choices);
    return is_iso;
  }

  bool is_iso = iso_partial();
  partition_destroy(g1_degree_P);
  partition_destroy(g2_degree_P);
  if(is_iso)
    return pho;
  array_destroy(pho);
  return NULL;
}



/* étant donné les voisins d'un sommet et la partition du graph,
   on renvoie la configuration de ce sommet 
   (i.e. les numéros des parties correspondantes à ses voisins, triés) */
array *configuration(const list* lis, const partition* P)
{
  int n = lis -> length;
  array *conf = array_create(n);
  node *current = lis -> head;
  while(current)
  {
    int v = current -> data;
    array_append(conf, P -> ele_to_par[v].set_id);
    current = current -> next;
  }
  array_sort(conf);
  return conf;
}


/* Trouver une partition stable à partir d'une partition intitiale 
   P -> num_sets joue un rôle ici 
   La partition en entrée n'est pas modifié */
partition *stable_partition(const graph *G, const partition *P)
{
  int n = G -> num_vertices;
  assert(P -> num_elements == n);
  partition *P_ref = partition_copy(P);
  partition *P_ = partition_create(n);
  P_ -> num_sets = 0;

  /* obetenir une nouvelle partition en fonction de P_ref */
  while(True)
  {
    int offset = 0;
    for(int i=0;i<P_ref->num_sets;++i)
    {
      /* à chaque configuration associer un numéro */
      array *partition_num[P_ref -> sets[i].size];
      int pn_len = 0;
      box *b = P_ref -> sets[i].data;
      while(b)
      {
        int u = b -> ele_id;
        /* regardons les voisins de u */
        array *conf = configuration(G -> adj_list_arr[u], P_ref);
        int j, p = -1;
        for(j=0;j<pn_len;++j)
        {
          if(array_equal(partition_num[j], conf))
          {
            p = j + offset;
            array_destroy(conf);
            break;
          }
        }
        if(j == pn_len)
        {
          p = P_ -> num_sets;
          partition_num[pn_len] = conf;
          ++ P_ -> num_sets;
          assert(P_ -> num_sets <= n);
          ++ pn_len;
        }
        assert(p >= 0);
        partition_move(P_, u, p);
        b = b -> next;
      }
      for(int j=0;j<pn_len;++j)
        array_destroy(partition_num[j]);
      offset += pn_len;
      assert(offset == P_ -> num_sets);
    }

    /* Si la partition est déjà stable */
    if(P_ -> num_sets == P_ref -> num_sets)
      break;
    partition *tmp = P_ref;
    P_ref = P_;
    P_ = tmp;
    P_ -> num_sets = 0;
  }
  partition_destroy(P_ref);
  return P_;
}



/* P1 et P2 respectivement une partition de G1 et G2
   Supposons qu'il sonts en correspondance, on raffine au fur et à mesure
   les deux partitions et on vérifie en même temps qu'ils soient toujours 
   compatbile
   P1 et P2 peuvent être modifiés au cours de l'appel (faire un copie!)
   P1_ et P2_ sont utilisé pour faire les nouvelles partitions
   A la fin, si on renvoie True, les contenus de P1, P2, P1_, P2_
   sont tous des partitions stables */

bool stable_partition2 (const graph *G1, const graph *G2, 
  partition *P1, partition *P2, partition *P1_, partition *P2_)
{
  assert(compatible(P1,P2));
  int n = G1 -> num_vertices;
  assert(P1 -> num_elements == n);
  assert(G2 -> num_vertices == n);
  
  /* Initialiser P1_ et P2_*/
  P1_ -> num_sets = 0;
  P2_ -> num_sets = 0;
  
  /* On travaille partie par partie */
  int offset = 0;
  for(int i=0;i<P1->num_sets;++i)
  {
    array *partition_num[P1 -> sets[i].size];
    int pn_len = 0;
    
    /* raffiner la partition pour G1 */
    box *b1 = P1 -> sets[i].data;
    while(b1)
    {
      int u = b1 -> ele_id;
      array *conf = configuration(G1 -> adj_list_arr[u], P1);
      int j, p =-1;
      for(j=0;j<pn_len;++j)
      {
        if(array_equal(partition_num[j],conf))
        {
          p = j + offset;
          array_destroy(conf);
          break;
        }
      }
      if(j == pn_len)
      {
        p = P1_ -> num_sets;
        partition_num[pn_len] = conf;
        ++ P1_ -> num_sets;
        ++ pn_len;
      }
      partition_move(P1_, u, p);
      b1 = b1 -> next;
    }

    /* raffiner la partition pour G2 */
    P2_ -> num_sets += pn_len;
    box *b2 = P2 -> sets[i].data;
    while(b2)
    {
      int u = b2 -> ele_id;
      array *conf = configuration(G2 -> adj_list_arr[u], P2);
      int j, p = -1;
      for(j=0;j<pn_len;++j)
      {
        if(array_equal(partition_num[j],conf))
        {
          p = j + offset;
          array_destroy(conf);
          break;
        }
      }
      if(j == pn_len)
      {
        array_destroy(conf);
        for(int j=0;j<pn_len;++j)
          array_destroy(partition_num[j]);
        return False;
      }
      partition_move(P2_, u, p);
      b2 = b2 -> next;
    }
    
    for(int j=0;j<pn_len;++j)
      array_destroy(partition_num[j]);
    offset += pn_len;
  }

  if(!compatible(P1_, P2_))
    return False;
  /* Si les partitions sont stables, renvoie True */
  if(P1_ -> num_sets == P1 -> num_sets)
    return True;
  return(stable_partition2(G1, G2, P1_, P2_, P1, P2));
}


/* P1 et P2 sont passé en paramètres car on raffine chaque fois
   les partitions, l'isomorphisme en train d'être construit pho
   est modifié en place */
bool Weisfeiler_Lehman(const graph *G1, const graph *G2, 
    const partition *P1, const partition *P2, array *pho)
{
  assert(compatible(P1,P2));
  int n = G1 -> num_vertices;
  assert(P1 -> num_elements == n);
  assert(G2 -> num_vertices == n);

  partition *P1_ref = partition_copy(P1);
  partition *P2_ref = partition_copy(P2);
  partition *P1_ = partition_create(n);
  partition *P2_ = partition_create(n);
  
  /* construite les partition stables */
  bool is_compatible = stable_partition2(G1, G2, P1_ref, P2_ref, P1_, P2_);
  partition_destroy(P1_ref);
  partition_destroy(P2_ref);
  
  /* si deux partitions non compatibles, pas de solution */
  if(!is_compatible)
  {
    partition_destroy(P1_);
    partition_destroy(P2_);
    return False;
  }
  assert(compatible(P1_, P2_));

  /* on précise l'image d'un élément quand c'est possible, et 
     on chercher la partie contenant le moins d'éléments parmi ceux 
     qui contiennent au moins 2 éléments */
  int min_ind = -1;
  int min_size = n + 1;
  for(int i=0;i<P1_->num_sets;++i)
  {
    if(P1_ -> sets[i].size == 1)
    {
      int u = P1_ -> sets[i].data -> ele_id;
      int v = P2_ -> sets[i].data -> ele_id;
      pho -> data[u] = v;
    }
    else
    {
      int seti_size = P1_ -> sets[i].size;
      assert(seti_size > 1);
      if(seti_size < min_size)
      {
        min_size = seti_size;
        min_ind = i;
      }
    }
  }

  /* si pho est entièrement définie, on renvoie True */
  if(min_ind == -1)
  {
    partition_destroy(P1_);
    partition_destroy(P2_);
    return True;
  }

  /* sinon on teste avec P1_/P2_ -> sets[min_id] */
  int u = P1_ -> sets[min_ind].data -> ele_id;
  partition_move(P1_, u, P1_ -> num_sets);
  ++ P1_ -> num_sets;
  ++ P2_ -> num_sets;
  set *possible_choices = set_copy(&(P2_ -> sets[min_ind]));

  bool choosev(int v)
  {
    partition_move(P2_, v, P2_ -> num_sets -1);
    if(Weisfeiler_Lehman(G1, G2, P1_, P2_, pho))
      return True;
    partition_move(P2_, v, min_ind);
    return False;
  }

  bool is_iso = set_iter(possible_choices, &choosev);
  set_destroy(possible_choices);
  partition_destroy(P1_);
  partition_destroy(P2_);
  return is_iso;
}


/* la version finale */
array *isomorphism(const graph *G1, const graph *G2)
{
  int n = G1 -> num_vertices;
  if(G2 -> num_vertices != n)
    return False;
  partition *P1 = partition_create(n);
  partition *P2 = partition_create(n);
  array *pho = array_create(n);
  array_fill(pho,0);
 
  bool is_iso = Weisfeiler_Lehman(G1, G2, P1, P2, pho);
  partition_destroy(P1);
  partition_destroy(P2);
  if(is_iso)
    return pho;
  array_destroy(pho);
  return NULL;
}
