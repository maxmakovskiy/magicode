#include <stdio.h>
#include <inttypes.h>

#include "file.h"
#include "stack.h"
#include "dl_list.h"


#define ASCII_SHIFT (48)

void 
calc_stats(
   const uint32_t* data,
   uint32_t len, 
   uint32_t* most_common, 
   uint32_t* least_common
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

    if (n_ones == n_zeros) {
      (*most_common) |= 1 << i;
    } else if (n_ones - n_zeros > 0 && n_zeros > 0) {
      (*most_common) |= 1 << i;
    } else if (n_ones > 0){
      (*least_common) |= 1 << i;
    }
  }
}


int main() {
  const uint32_t test[12] = {
    0b00100u,
    0b11110u,
    0b10110u,
    0b10111u,
    0b10101u,
    0b01111u,
    0b00111u,
    0b11100u,
    0b10000u,
    0b11001u,
    0b00010u,
    0b01010u
  };

  List* list = init_list();

  for (int i = 0; i < 12; i++) {
    insert_into(list, 0, (i+1));
  }

  printf("list size is %d\n", list->size);

  del_list(list);

  
  // uint32_t most = 0, least = 0;
  // calc_stats(test, 12, &most, &least);
  // printf("most %d; least %d; prod %d\n", most, least, (most*least));
  
  // const char* filename = "data.txt";

  // stack* st = init_stack();
  // read_file_as_uint(filename, st);

  // uint32_t most = 0, least = 0;
  // calc_stats(st->data, st->size, &most, &least);

  // // List* list = create_list_from(st);
  
  // printf("most %d; least %d; prod %d\n", most, least, (most*least));
  // del_stack(st);
  
  return 0;
}
