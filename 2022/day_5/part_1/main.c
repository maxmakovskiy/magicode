#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

#include "darray.h"


#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#define NO_IMPLEMENTED (assert(("Hasn't implemented yet", 0)))

enum {
  CRATE_CAP = 72,
  BUFF_LEN = 21,
  TOKENS_CMD = 6,
  NUM_OF_STACKS = 9
};

typedef struct stack {
  uint8_t* crates;  
  size_t len;  
} Stack;


void feed(Stack* st, const uint8_t* source, size_t len) {
  size_t fitted = MIN(CRATE_CAP-st->len, len);
  size_t current_len = st->len;
  for (size_t i = 0; i < fitted; i++) {
    st->crates[i+current_len] = source[i];
    st->len++;
  }
}


void move(Stack* source, Stack* dest, size_t amount) {
  size_t fitted = MIN(CRATE_CAP-dest->len, amount);
  size_t cur_src = source->len;
  size_t cur_dest = dest->len;

  for (size_t i = 0; i < fitted; i++) {
    dest->crates[i+cur_dest] = source->crates[cur_src-(i+1)];
    source->len--;
    dest->len++; 
  }
}

/*

                [B]     [L]     [S]
        [Q] [J] [C]     [W]     [F]
    [F] [T] [B] [D]     [P]     [P]
    [S] [J] [Z] [T]     [B] [C] [H]
    [L] [H] [H] [Z] [G] [Z] [G] [R]
[R] [H] [D] [R] [F] [C] [V] [Q] [T]
[C] [J] [M] [G] [P] [H] [N] [J] [D]
[H] [B] [R] [S] [R] [T] [S] [R] [L]
 1   2   3   4   5   6   7   8   9 
*/

// Par example, la ligne:
// move 9 from 1 to 9
// Peut-être couper par:
// move 9
// from 1
// to 9
typedef enum TokenType {
  TOKEN_EOF = 0,
  TOKEN_NAME = 256,
  TOKEN_INT
} TokenType;

typedef struct Token {
  TokenType type;
  union {
    uint32_t value;
    struct {
      const char* start;
      const char* end;
    };
  };
} Token;



uint32_t process(char* stream, Token* token, uint32_t offset) {
  char* iter = stream + offset;
  uint32_t bytes_read = 0;
  while (*iter == ' ' || *iter == '\n') {
    iter++;
    bytes_read++;
  }
  switch(*iter) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      uint32_t v = 0;
      while(isdigit(*iter)) {
        v *= 10;
        v += (*iter) - '0';
        iter++;
        bytes_read++;
      }
      token->type = TOKEN_INT;
      token->value = v;
      break;
    }
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case '_': {
      const char* start = iter;
      iter++;
      bytes_read++;
      while(isalnum(*iter) || *iter == '_') {
        iter++;
        bytes_read++;
      }
      token->end = iter;
      token->start = start;
      token->type = TOKEN_NAME;
      break;
    }
    case '\0': {
      bytes_read = 1;
      token->type = TOKEN_EOF;
      break;
    }
    default: {
      token->type = *iter++;
      bytes_read++;
      break;
    }
  }
  
  return bytes_read;
}


uint32_t xpow(uint32_t base, uint32_t exp) {
  uint32_t result = 1;
  for (size_t i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}


uint32_t to_int(char* buf, size_t len) {
  uint32_t result = 0;

  for (size_t i = 0; i < len; i++) {
    uint32_t digit = buf[i] - 0x30;
    result += xpow(digit, len-(i+1));
  }

  return result;  
}


uint32_t compare_str(const char* str1, const char* str2, uint32_t len) {
  for (size_t i = 0; i < len; i++) {
    if (str1[i] != str2[i]) {
      return 0;
    }
  }

  return 1;
}


typedef struct MoveCmd {
  Stack* to;
  Stack* from;
  uint32_t how_much;
} MoveCmd;


Stack* stacks[NUM_OF_STACKS];


uint32_t build_MoveCmd(Token** tokens, MoveCmd* command) {
  if (*tokens == NULL || (*tokens)->type == TOKEN_EOF) {
    return 0;
  } else if (
    tokens[0]->type == TOKEN_NAME &&
    tokens[1]->type == TOKEN_INT
  ) {
    if (compare_str(
      tokens[0]->start, "move",
      tokens[0]->end-tokens[0]->start)
    ) {
      command->how_much = tokens[1]->value;
      build_MoveCmd(tokens+2, command);
    } else if (compare_str(
      tokens[0]->start, "from",
      tokens[0]->end-tokens[0]->start)
    ) {
      command->from = stacks[tokens[1]->value-1];
      build_MoveCmd(tokens+2, command);
    } else if (compare_str(
      tokens[0]->start, "to",
      tokens[0]->end-tokens[0]->start)
    ) {
      command->to = stacks[tokens[1]->value-1];
      // ?
      // build_MoveCmd(tokens+2, command);
    }
  }
  return 1;

}

void execute_cmd(MoveCmd* cmd) {
  move(cmd->from, cmd->to, cmd->how_much);
}

void create_stacks() {
  for (uint32_t i = 0; i < NUM_OF_STACKS; i++) {
    stacks[i] = malloc(sizeof(Stack));
    stacks[i]->crates = malloc(sizeof(uint8_t) * CRATE_CAP);
    stacks[i]->len = 0;
  }
}

void init_stacks() {
  uint8_t init9[8] = {
    'L', 'D', 'T', 'R', 'H', 'P', 'F', 'S'
  };
  feed(stacks[8], init9, 8);
  uint8_t init8[5] = {
    'R', 'J', 'Q', 'G', 'C'
  };
  feed(stacks[7], init8, 5);
  uint8_t init7[8] = {
    'S', 'N', 'V', 'Z', 'B', 'P', 'W', 'L'
  };
  feed(stacks[6], init7, 8);
  uint8_t init6[4] = {
    'T', 'H', 'C', 'G'
  };
  feed(stacks[5], init6, 4);
  uint8_t init5[8] = {
    'R', 'P', 'F', 'Z', 'T', 'D', 'C', 'B'
  };
  feed(stacks[4], init5, 8);
  uint8_t init4[7] = {
    'S', 'G', 'R', 'H', 'Z', 'B', 'J'    
  };
  feed(stacks[3], init4, 7);
  uint8_t init3[7] = {
    0x52, 0x4D, 0x44, 0x48, 0x4A, 0x54, 0x51 
  };
  feed(stacks[2], init3, 7);
  uint8_t init2[6] = {
    0x42, 0x4A, 0x48, 0x4C, 0x53, 0x46
  };
  feed(stacks[1], init2, 6);
  uint8_t init1[3] = {
    0x48, 0x43, 0x52
  }; 
  feed(stacks[0], init1, 3);
}

void clear_stacks() {
  for (uint32_t i = 0; i < NUM_OF_STACKS; i++) {
    free(stacks[i]->crates);
    free(stacks[i]);
  }
}


int main(void) {
  enum {
    BYTES_TO_READ = 128
  };
  int file = open("input.txt", O_RDONLY);
  char temp_buf[BYTES_TO_READ];
  char* raw = NULL;
  size_t k;
  while ((k = read(file, temp_buf, BYTES_TO_READ))) {
    for (size_t i = 0; i < k; i++) {
      buf_push(raw, temp_buf[i]);
    }
  }
  close(file);

  create_stacks();
  init_stacks();
  
  uint32_t offset = 0;
  printf("buf let %ld\n", buf_len(raw));
  while (offset < buf_len(raw)) { // one line = one command
    Token** tokens = malloc(TOKENS_CMD * sizeof(Token*));
    for (size_t i = 0; i < TOKENS_CMD; i++) {
      tokens[i] = malloc(sizeof(Token));
      offset += process(raw, tokens[i], offset);
      if (tokens[i]->type == TOKEN_EOF) {
        break;
      }
    }
    MoveCmd* cmd = malloc(sizeof(MoveCmd));
    uint32_t op_res = build_MoveCmd(tokens, cmd);
    if (op_res > 0) {
      execute_cmd(cmd);
    }
    for (size_t i = 0; i < TOKENS_CMD; i++) {
      if (tokens[i]->type == TOKEN_EOF) {
        free(tokens[i]);
        break;
      }
      free(tokens[i]);
    }
    free(cmd);
    free(tokens);
  }
  
  for (uint32_t i = 0; i < NUM_OF_STACKS; i++) {
    uint8_t lastone = stacks[i]->crates[stacks[i]->len-1];
    printf("Stack %d has last crate with symbol %c\n", i+1, lastone);
  }  

  
  clear_stacks();
  buf_free(raw);

  

    return 0;
}


