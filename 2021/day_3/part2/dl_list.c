#include "dl_list.h"

List* init_list() {
  List* list = (List*)malloc(sizeof(List));
  list->head = NULL;
  list->size = 0;
  return list;
}

Node* _init_node(uint32_t value) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->prev = node->next = NULL;
  node->value = value;
  return node;
}

void _del_node(Node* node) {
  free(node);
  node = NULL;
}

void del_list(List* list) {
  for (Node* iter = list->head; iter; ) {
    Node* to_del = iter;
    iter = iter->next;
    _del_node(to_del);
  }
  free(list);
  list = NULL;
}

void insert_into(List* list, uint32_t pos, uint32_t value) {
  Node* to_insert = _init_node(value);

  Node* iter = list->head;

  if (iter == NULL) {
    list->head = to_insert;
  } else {
    uint32_t cur_pos = 0;
    while(iter) {
      if (cur_pos == pos) {
        to_insert->prev = iter->prev;
        to_insert->next = iter;
        iter->prev = to_insert;
        break;
      }
      iter = iter->next;
    }
  }

  list->size += 1;
  return;
}

Node* find_in(List* list, uint32_t what) {
  Node* iter = list->head;
  while(iter) {
    if (iter->value == what) {
      return iter;
    }
    iter = iter->next;
  }
  return NULL;
}

int32_t remove_from(List* list, uint32_t what) {
  Node* to_remove = find_in(list, what); 
  if (to_remove == NULL) {
    return -1;
  }
  Node* before = to_remove->prev;
  Node* after = to_remove->next;
  before->next = after;
  after->prev = before;
  _del_node(to_remove);
  list->size -= 1;
  
  return 0;
}

List* create_list_from(stack* st) {
  List* list = init_list();

  for (uint32_t i = 0; i < st->size; i++) {
    insert_into(list, 0, st->data[i]); 
  }

  return list;
}



