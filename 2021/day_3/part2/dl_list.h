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

void enqueue(List* list, uint32_t value);
void push_back(List* list, uint32_t value);
void insert_after_pos(List* list, uint32_t pos, uint32_t value);
void insert_after_node(List* list, Node* node, uint32_t value);

Node* find_node(List* list, uint32_t what);

void remove_from(List* list, uint32_t what);
void remove_node(List* list, Node* to_remove);

List* copy_list(List* source);
List* create_list_from(stack* st);

void print_list_head(List* list, uint32_t n_elements); 
void print_list(List* list); 
  

#endif
