#ifndef DL_LIST_H
#define DL_LIST_H

#include <inttypes.h>
#include <stdlib.h>
#include "stack.h"

typedef struct Node {
  struct Node* prev;
  struct Node* next;
  uint32_t value;
} Node;

typedef struct List {
  Node* head;
  uint32_t size; 
} List;

List* init_list(); 
Node* _init_node(uint32_t value);
void del_list(List* list);
void _del_node(Node* node);

void insert_into(List* list, uint32_t pos, uint32_t value);
Node* find_in(List* list, uint32_t what);
int32_t remove_from(List* list, uint32_t what);

List* create_list_from(stack* st);

#endif
