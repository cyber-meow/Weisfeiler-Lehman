/* list.h */
#ifndef _LIST_H_
#define _LIST_H_

typedef struct _node
{
  int data;
  struct _node *next;
} node;

typedef struct 
{
  int length;
  node *head;
} list;

list *list_create();
void list_destroy(list* lis);

void list_prepend(list* lis, int i);
void list_delete(list* lis, int i);
void list_print(const list* lis);
void list_iter(const list* lis, void (*action)(int));
int list_at(const list* lis, int pos);

#endif

