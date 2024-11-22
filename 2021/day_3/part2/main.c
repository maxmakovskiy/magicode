#include <stdio.h>
#include <inttypes.h>

#include "file.h"
#include "stack.h"
#include "dl_list.h"


void
bit_stats(
  const List* list,
  uint32_t n_bit,
  int32_t* n_zeros,
  int32_t* n_ones
) {
  const uint32_t mask = 1 << n_bit;
  const Node* iter = list->head;
  for(; iter != NULL; iter = iter->next) {
    const uint32_t indicator = mask & iter->value;
    if (indicator == 0) {
      (*n_zeros) += 1;
    } else {
      (*n_ones) += 1;
    }
  }
}

void filter_most_common(List* list) {
  const static int32_t n_bits = 32;
  for (int32_t i = n_bits-1; i >= 0; i--) {
    int32_t n_zeros = 0;
    int32_t n_ones = 0;
    bit_stats(list, i, &n_zeros, &n_ones);

    uint32_t most_common = n_ones >= n_zeros;
    
    Node* iter = list->head;
    while (iter != NULL) {
      if (list->size == 1) break;
      uint32_t testy = (iter->value >> i) & 1;
      int eval_crit = (most_common == 1)
                      ? (testy & 1) == 1
                      : (testy | 0) == 0;
      if (eval_crit) {
        iter = iter->next;
      } else {
        Node* to_remove = iter;
        iter = iter->next;
        remove_node(list, to_remove);
      }
    }
  }
}

void filter_least_common(List* list) {
  const static int32_t n_bits = 32;
  for (int32_t i = n_bits-1; i >= 0; i--) {
    int32_t n_zeros = 0;
    int32_t n_ones = 0;
    bit_stats(list, i, &n_zeros, &n_ones);

    if (abs(n_zeros-n_ones) == list->size) continue;

    uint32_t least_common = n_ones < n_zeros;

    Node* iter = list->head;
    while (iter != NULL) {
      if (list->size == 1) break;
      uint32_t testy = (iter->value >> i) & 1;
      int eval_crit = (least_common == 1)
                      ? (testy & 1) == 1
                      : (testy | 0) == 0;
      if (eval_crit) {
        iter = iter->next;
      } else {
        Node* to_remove = iter;
        iter = iter->next;
        remove_node(list, to_remove);
      }
    }
  }
}


int main() {

  List* list = init_list();
  read_file_to_list("data.txt", list);
  List* list2 = copy_list(list);
  
  filter_most_common(list);

  filter_least_common(list2);

  printf("Size list = %d\n", list->size);
  printf("Size list2 = %d\n", list2->size);
  printf("oxygen generator rating : %d\n", list->head->value);
  printf("CO2 scrubber rating : %d\n", list2->head->value);
  printf("What is the life support rating of the submarine? It's %d\n",
        (list->head->value * list2->head->value));
  del_list(list);
  del_list(list2);

  
  // const uint32_t test[12] = {
  //   0b00100u,
  //   0b11110u,
  //   0b10110u,
  //   0b10111u,
  //   0b10101u,
  //   0b01111u,
  //   0b00111u,
  //   0b11100u,
  //   0b10000u,
  //   0b11001u,
  //   0b00010u,
  //   0b01010u
  // };

  // const uint32_t test[7] = {
  //   0b1101, // O2
  //   0b1110,
  //   0b1100,
  //   0b1000,
  //   0b0110, // CO2
  //   0b0011,
  //   0b0010
  // }; // correct O2 is 13 (1101) and the correct CO2 is 6 (0110)
  
  // List* list = init_list();
  // for (uint32_t i = 0; i < 7; i++) {
  //   push_back(list, test[i]);
  // }
  // List* list2 = copy_list(list);
  
  // filter_most_common(list);

  // filter_least_common(list2);

  // print_list(list);
  // print_list(list2);
  
  // del_list(list);
  // del_list(list2);


  
  return 0;
}
