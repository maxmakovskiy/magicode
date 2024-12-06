#include <inttypes.h>

#ifndef STACK_H_
#define STACK_H_

typedef struct stack {
  uint32_t size;
  uint32_t capacity;
  uint32_t* data;
} stack;

stack* init_stack(); 
void push(stack* st, uint32_t element);
uint32_t pop(stack* st);
void del_stack(stack* st);

void print_stack(stack* st, size_t elems_per_line);

#endif
