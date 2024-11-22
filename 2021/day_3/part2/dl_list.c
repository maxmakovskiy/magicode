#include "stdio.h"
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
  node->prev = NULL;
  node->next = NULL;
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

void 
insert_after_pos(
  List* list, 
  uint32_t pos, 
  uint32_t value
) {
  if (list->head == NULL) {
    enqueue(list, value);
  } else {
    Node* iter = list->head;
    uint32_t cur_pos = 0;
    while(iter->next && cur_pos == pos) {
      iter = iter->next;
    }
    Node* to_insert = _init_node(value);
    to_insert->next = iter->next;
    iter->next = to_insert;
    to_insert->prev = iter;
    list->size += 1;
  }
}

void 
insert_after_node(
  List* list,
  Node* node,
  uint32_t value
) {
  if (list->head == NULL) {
    enqueue(list, value);
  } {
    Node* iter = list->head;
    while (iter->next && (iter != node)) {
      iter = iter->next;
    }
    Node* to_insert = _init_node(value);
    to_insert->next = iter->next;
    iter->next = to_insert;
    to_insert->prev = iter;
    list->size += 1;
  }
}

Node* find_node(List* list, uint32_t what) {
  Node* iter = list->head;
  while(iter) {
    if (iter->value == what) {
      return iter;
    }
    iter = iter->next;
  }
  return NULL;
}

void remove_node(List* list, Node* to_remove) {
  Node* before = to_remove->prev;
  Node* after = to_remove->next;
  if (before == NULL && after != NULL) {
    // it's head
    after->prev = NULL;
    list->head = after;
  } else if (after == NULL && before != NULL) {
    // it's tail
    before->next = NULL;
  } else if (before != NULL && after != NULL) {
    // otherwise we're somewhere in the middle 
    before->next = after;
    after->prev = before;
  }
  _del_node(to_remove);
  list->size -= 1;
}
 
void remove_from(List* list, uint32_t what) {
  Node* to_remove = find_node(list, what);
  remove_node(list, to_remove);
}


List* create_list_from(stack* st) {
  List* list = init_list();

  for (uint32_t i = 0; i < st->size; i++) {
    insert_after_pos(list, 0, st->data[i]); 
  }

  return list;
}

void enqueue(List* list, uint32_t value) {
  Node* new_head = _init_node(value);

  Node* old_head = list->head;
  list->head = new_head;
  
  if (old_head != NULL) {
    new_head->next = old_head;
    old_head->prev = new_head;
  }
  list->size += 1;
}

void push_back(List* list, uint32_t value) {
  if (list->head == NULL) {
    enqueue(list, value);
  } else {
    Node* back = _init_node(value);
    Node* iter = list->head;

    while(iter->next != NULL) {
      iter = iter->next;
    }

    iter->next = back;
    back->prev = iter;
    list->size += 1;
  }
}

uint32_t min(uint32_t v1, uint32_t v2) {
  return (v1 > v2) ? v2 : v1;
}

void print_list_head(List* list, uint32_t n_elements) {
  printf("List (0x%x) | size: %d\n", list, list->size);
  
  uint32_t n = min(list->size, n_elements);
  printf("Printing first %d elements\n", n);
  
  Node* iter = list->head;
  for (uint32_t i = 0; i < n && iter; i++, iter = iter->next) {
    printf(">>> addr: 0x%x | value: %d\n", iter, iter->value);
  }
}

void print_list(List* list) {
  printf("List (0x%x) | size: %d\n", list, list->size);
  
  Node* iter = list->head;
  for(; iter; iter = iter->next) {
    printf(">>> addr: 0x%x | value: %d\n", iter, iter->value);
  }
} 

List* copy_list(List* source) {
  List* dest = init_list();

  Node* iter = source->head;
  for(; iter; iter = iter->next) {
    push_back(dest, iter->value);
  }

  return dest;
}
