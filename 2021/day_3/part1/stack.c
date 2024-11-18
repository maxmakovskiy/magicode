#include "stack.h"
#include <stdlib.h>
#include <string.h>


#define INIT_CAP (16)

stack* init_stack() {
  stack* st;
  st = (stack*)malloc(sizeof(stack));
  st->size = 0;
  st->capacity = INIT_CAP;
  st->data = (uint32_t*)malloc(sizeof(uint32_t) * INIT_CAP);
  return st;
}

void _copy(const uint32_t* old, uint32_t* new, uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    new[i] = old[i];
  }
}


void push(stack* st, uint32_t element) {
  if (st->size >= st->capacity) {
     uint32_t new_cap = st->capacity * 2;
     uint32_t* new_data = (uint32_t*)malloc(sizeof(uint32_t) * new_cap);
     // memcpy(st->data, new_data, st->size);
     _copy(st->data, new_data, st->size);
     free(st->data);
     st->data = new_data;
     st->capacity = new_cap;
  }

  st->data[st->size] = element;
  st->size += 1;
}

uint32_t pop(stack* st) {
  st->size -= 1;
  uint32_t element = st->data[st->size];
  return element;
}

void del_stack(stack* st) {
  free(st->data);
  free(st);
  st = NULL;
}

