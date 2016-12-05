/* list.c */
/* En général, si on passe un pointeur dans une fonction,
   ce n'est pas le pointeur null, cette règle s'applique 
   au projet entier */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

list *list_create()
{
  list *lis = (list*)malloc(sizeof(list));
  assert(lis);
  lis -> length = 0;
  lis -> head = NULL;
  return lis;
}

void list_destroy(list* lis)
{
  while(lis -> head)
  {
    node *current = lis -> head;
    lis -> head = current -> next;
    free(current);
  }
  free(lis);
}

void list_prepend(list* lis, int i)
{
  node *newnode = (node*)malloc(sizeof(node));
  assert(newnode);
  newnode -> data = i;
  newnode -> next = lis -> head;
  lis -> head = newnode;
  ++ lis -> length;
}

/* supprimer tous les instances de i, non modifié si il n'y en a pas*/
void list_delete(list* lis, int i)
{
  if(lis -> head)
  {
    node *current = lis -> head;
    while(current -> next)
    {
      if(current -> next -> data == i)
      {
        node *tmp = current -> next;
        current -> next = current -> next -> next;
        free(tmp);
        -- lis -> length;
      }
      else
        current = current -> next;
    }
    if(lis -> head -> data == i)
    {
      node *tmp = lis -> head;
      lis -> head = lis -> head -> next;
      free(tmp);
      -- lis -> length;
    }
  }
}

void list_print(const list* lis)
{
  node *current = lis -> head;
  while(current)
  {
    printf("%d -> ", current -> data);
    current = current -> next;
  }
  printf("NULL\n");
}

void list_iter(const list* lis, void (*action)(int))
{
  node *current = lis -> head;
  while(current)
  {
    action(current -> data);
    current = current -> next;
  }
}

int list_at(const list* lis, int pos)
{
  assert(pos < lis -> length);
  node *current = lis -> head;
  for(int i=0;i<pos;++i)
    current = current -> next;
  return current -> data;
}
