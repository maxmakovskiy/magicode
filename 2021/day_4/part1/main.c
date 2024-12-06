#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>

#include "stack.h"


uint32_t to_number10(const char* src, size_t src_len) {
  uint32_t result = 0;

  for(size_t i = 0; i < src_len; i++) {
    result += pow(10, src_len-i-1) * (src[i] - 48);
  }

  return result;
}


size_t read_boards_from_file(const char* filename, stack** boards) {
  FILE* file;
  file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return 0;
  }

  char buf[32];
  size_t buf_idx = 0;
  int ch;
  int nl_counter = 0;
  int wh_counter = 0;
  size_t board_idx = 0;

  boards[board_idx] = init_stack();

  while((ch = fgetc(file)) != EOF) {
    if (nl_counter > 1) {
      board_idx += 1;
      boards[board_idx] = init_stack();
      nl_counter  = 0;
      if (ch == ' ') {
        continue;
      } 
    }
    
    if (ch == ' ' || ch == '\n') {
      if (ch == ' ') {
        wh_counter += 1;
      }
      if (ch == '\n') {
        nl_counter += 1;
      }

      if (wh_counter + nl_counter <= 1) {
        uint32_t val = to_number10(buf, buf_idx);
        push(boards[board_idx], val);  
        buf_idx = 0;
      }
      
    } else {
      wh_counter = 0;
      nl_counter = 0;

      buf[buf_idx] = ch;
      buf_idx += 1;
    }
  }

  return board_idx+1;
}


int find_pos(stack* where, uint32_t what) {
  for(int i = 0; i < where->size; i++) {
    if (where->data[i] == what) {
      return i;
    }
  }
  return -1;
}


void 
one_bingo_round(
  stack** boards,
  size_t n_boards,
  stack** marks,
  uint32_t number
) {
  for (size_t i = 0; i < n_boards; i++) {
    int pos = find_pos(boards[i], number);
    if (pos != -1) {
      marks[i]->data[pos] = 1;
    }
  }
}


void 
balk_init(
  stack** dest,
  size_t num_to_init
) {
  for (size_t i = 0; i < num_to_init; i++) {
    dest[i] = init_stack();
    for (size_t j = 0; j < 25; j++) {
      push(dest[i], 0);
    }
  }
}


int check_cols_win(stack* board_marks) {
  for (size_t i = 0; i < 5; i++) {
    int res = 1;
    for (size_t j = 0; j < 5; j++) {
      res *= board_marks->data[(i)+(j*5)];
    }
    if (res) {
      return res;
    }
  }

  return 0;
}

int check_rows_win(stack* board_marks) {
  for (size_t i = 0; i < 5; i++) {
    int res = 0;
    for (size_t j = 0; j < 5; j++) {
      res += board_marks->data[(i*5)+j];
    }

    if (res == 5) {
        return 5;
    }
  }

  return 0;
}

int check_for_win(stack* board_marks) {
  size_t idx_tracker_lns = 0;
  size_t idx_tracker_cols = 0;
  for (size_t i = 0; i < board_marks->size; i++) {
    if (board_marks->data[i] == 1) {
      idx_tracker_lns += i / 5;
      idx_tracker_cols += i % 5;
     
    }
  }

  int rows_check = (
    (idx_tracker_lns == 0  && idx_tracker_cols == 10) ||
    (idx_tracker_lns == 5  && idx_tracker_cols == 10) ||
    (idx_tracker_lns == 10 && idx_tracker_cols == 10) ||
    (idx_tracker_lns == 15 && idx_tracker_cols == 10) ||
    (idx_tracker_lns == 25 && idx_tracker_cols == 10)
  );

  int cols_check = (
    (idx_tracker_cols == 0  && idx_tracker_lns == 10) ||
    (idx_tracker_cols == 5  && idx_tracker_lns == 10) ||
    (idx_tracker_cols == 10 && idx_tracker_lns == 10) ||
    (idx_tracker_cols == 15 && idx_tracker_lns == 10) ||
    (idx_tracker_cols == 25 && idx_tracker_lns == 10)
  );
  
  return rows_check || cols_check;
}


uint32_t 
calc_score(
  stack* board, 
  stack* board_marks, 
  uint32_t number
) {
  int result = 0;
  for (size_t i = 0; i < 25; i++) {
    if (board_marks->data[i] == 0) {
      result += board->data[i];
    }
  }
  return result * number;
}

#define n_rounds (100)
int main() {
  // const uint32_t n_rounds = 100;
  const uint32_t rounds[n_rounds] = {
    13,47,64,52,60,69,80,85,57,1,2,6,30,81,
    86,40,27,26,97,77,70,92,43,94,8,78,3,88,
    93,17,55,49,32,59,51,28,33,41,83,67,11,91,
    53,36,96,7,34,79,98,72,39,56,31,75,82,62,
    99,66,29,58,9,50,54,12,45,68,4,46,38,21,24,
    18,44,48,16,61,19,0,90,35,65,37,73,20,22,
    89,42,23,15,87,74,10,71,25,14,76,84,5,63,95
 };

  stack* boards[1000];
  stack* marks[1000];
  size_t n_boards = read_boards_from_file(
    "data.txt", boards);

  balk_init(marks, n_boards);

  int winner_found = 0;
  for (size_t i = 0; i < n_rounds; i++) {
    one_bingo_round(boards, n_boards, marks, rounds[i]);
    for (size_t j = 0; j < n_boards; j++) {
      // int round_res = check_for_win(marks[j]);
      int round_res = check_rows_win(marks[j]) || check_cols_win(marks[j]);
      if (round_res) {
        winner_found = 1;
        printf("Board num #%d is the winner, current number %d\n", j, rounds[i]);
        uint32_t score = calc_score(boards[j], marks[j], rounds[i]);
        printf("SCORE: %d\n", score);
        break;
      }
    }
    if (winner_found) {
      break;
    }
  }
  
  for(size_t i = 0; i < n_boards; i++) {
    // print_stack(boards[i], 5);
    // print_stack(marks[i], 5);
    // printf("/////////////////\n");
    del_stack(boards[i]);
    del_stack(marks[i]);
  }



  
  // const uint32_t rounds[] = {
  //   7,4,9,5,11,17,23,2,0,14,21,24,
  //   10,16,13,6,15,25,12,22,18,20,
  //   8,19,3,26,1
  // };

  // stack* boards[32];
  // size_t n_boards = read_boards_from_file(
  //   "test_boards.txt", boards);
  // stack* marks[32];

  // balk_init(marks, n_boards);

  // int winner_found = 0;
  // for (size_t i = 0; i < 27; i++) {
  //   one_bingo_round(boards, n_boards, marks, rounds[i]);
  //   for (size_t j = 0; j < n_boards; j++) {
  //     // int round_res = check_for_win(marks[j]);
  //     int round_res = check_rows_win(marks[j]) || check_cols_win(marks[j]);
  //     if (round_res) {
  //       winner_found = 1;
  //       printf("Board num #%d is the winner, current number %d\n", j, rounds[i]);
  //       uint32_t score = calc_score(boards[j], marks[j], rounds[i]);
  //       printf("SCORE: %d\n", score);
  //       break;
  //     }
  //   }
  //   if (winner_found) {
  //     break;
  //   }
  // }
  
  // for(size_t i = 0; i < n_boards; i++) {
  //   // print_stack(boards[i], 5);
  //   // print_stack(marks[i], 5);
  //   // printf("/////////////////\n");
  //   del_stack(boards[i]);
  //   del_stack(marks[i]);
  // }

  
  return 0;
}
