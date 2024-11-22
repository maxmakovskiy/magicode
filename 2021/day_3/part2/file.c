#include <stdio.h>
#include <stdlib.h>
#include "file.h"

#define ASCII_SHIFT (48)

uint32_t str_bin_to_uint(const char* buffer) {
  uint32_t res = 0;
  for (int i = 0; buffer[i] == 48 || buffer[i] == 49; i++) {
    res = res << 1;
    res |= (buffer[i] - ASCII_SHIFT);
  }
  return res;
}

int32_t read_file_as_uint(const char* filename, stack* storage) {
  FILE* file;
  file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return -1;
  }
  
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
      push(storage, t);
      i = 0;
    }
  }

  return (int32_t)(storage->size);
}


int32_t read_file_to_list(const char* filename, List* storage) {
  FILE* file;
  file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return -1;
  }
  
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
      push_back(storage, t);
      i = 0;
    }
  }

  return (int32_t)(storage->size);
}

