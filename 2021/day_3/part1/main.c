#include <stdio.h>
#include <inttypes.h>

#include "stack.h"

#define ASCII_SHIFT (48)

void 
calc_rates(
   const uint32_t* data,
   uint32_t len, 
   uint32_t* gamma, 
   uint32_t* eps
) {
  
  // const static int n_bits = sizeof(uint32_t) * 8;
  const static uint32_t n_bits = 32;
  for (uint32_t i = 0; i < n_bits; i++) {
    int32_t n_zeros = 0;
    int32_t n_ones = 0;
    for(uint32_t j=0; j < len; j++) {
      const uint32_t mask = 1 << i;
      const uint32_t indicator = mask & data[j];
      if (indicator == 0) {
        n_zeros += 1;
      } else {
        n_ones += 1;
      }
    }
    if (n_ones - n_zeros > 0 && n_zeros > 0) {
      (*gamma) |= 1 << i;
    } else if (n_ones > 0){
      (*eps) |= 1 << i;
    }
  }
}

uint32_t str_bin_to_uint(const char* buffer) {
  uint32_t res = 0;
  for (int i = 0; buffer[i] != '\0'; i++) {
    res = res << 1;
    res |= (buffer[i] - ASCII_SHIFT);
  }
  return res;
}

int main() {
  FILE* file;
  file = fopen("data.txt", "r");
  if (!file) {
    perror("data.txt");
    return -1;
  }

  stack* st = init_stack();
  
  char buffer[32];
  int ch;
  int i = 0;
  int nl_counter = 0;
  while((ch = fgetc(file)) != EOF) {
    if (ch != '\n' && ch != '\0') {
      buffer[i] = ch;
      i += 1;
    } else {
      nl_counter += 1;
      uint32_t t = str_bin_to_uint(buffer);
      push(st, t);
      // printf("Buffer: %s; uint: %d (%x)\n", buffer, t, t);
      i = 0;
    }
  }
  // printf("number of lines: %d\n", nl_counter);
  // printf("size of stack: %d\n", st->size);
  
  // while(st->size != 0) {
  //   uint32_t t = pop(st);
  //   printf("%d (0x%x)\n", t, t);
  // }
  

  uint32_t gamma = 0, eps = 0;
  calc_rates(st->data, st->size, &gamma, &eps);

  printf("gamma (most common) %d\n", gamma);
  printf("epsilon (least common) %d\n", eps);

  
  del_stack(st);
  
  
  // uint32_t tests[] = {
  //   0b00100u, 0b11110u, 0b10110u, 0b10111u,
  //   0b10101u, 0b01111u, 0b00111u, 0b11100u,
  //   0b10000u, 0b11001u, 0b00010u, 0b01010u
  // };
  // uint32_t gamma_test = 0;
  // uint32_t eps_test = 0;
  // calc_rates(tests, 12, &gamma_test, &eps_test);

  // printf("gamma (most common) %d\n", gamma_test);
  // printf("epsilon (least common) %d\n", eps_test);
    
  return 0;
}
