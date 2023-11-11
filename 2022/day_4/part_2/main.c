#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#include "darray.h"


// + new line feed?
// So, new line feed it is +1
#define DATA_STR_L (12)


int clean_line(char* line, int len) {
  int i;
  int actual_len = 0;
  for (i = 0; i < len; i++) {
    if (line[i] == '\n') {
      actual_len = i+1;
      break;
    }
  }
  for (; i < len; i++) {
    line[i] = '\0';
  }
  return actual_len;
}


int pow(int base, int exp) {
  int result = 1;
  int i = exp;
  for (; i > 0; i--) {
    result *= base;
  }
  return result;
}


// "23" -> int[2, 3] -> 2 * 10^1 + 3 * 10^0
int str_to_int(const char* input, int length) {
  int k;
  int result = 0;
  for (k = 0; k < length; k++) {
    result += (((int)input[k]) - 0x30) * pow(10, length-k-1);
  }
  return result;
}

int is_numeric(char ch) {
  return (0x30 <= ch && ch <= 0x39) ? 1 : 0;
}

int process_line(const char* line, int length) {
  // 8-41,8-79
  // we would have this integer types
  // 8 = 0x00'00'00'08  || 0x08
  // 41 = 0x00'00'00'29 || 0x29
  // 8 = 0x00'00'00'08  || 0x08
  // 79 = 0x00'00'00'4F || 0x4F
  // let's reduce it to byte
  // then let's compose it into one 32-bit int number
  // 0x08
  //     0x29
  //         0x08
  //             0x4F
  // 0x0829084F as result
  // but we need 64 bit
  
  int i;

  char buf[2];
  int buf_iter = 0;

  int res_iter = 0;
  uint64_t result = 0;

  for(i = 0; i < length; i++) {
    if (is_numeric(line[i])) {
      buf[buf_iter++] = line[i];
    } else {
      uint64_t number = (uint64_t)str_to_int(buf, buf_iter);
      uint64_t t = (number << ((3 - res_iter) * sizeof(uint64_t)));
      result |= t;
      res_iter++;
      buf_iter = 0;
    }
  }

  return result;
}


int fully_overlapped(int f1, int f2, int s1, int s2) {
  return (s2 >= f2 && s1 <= f1) || (f2 >= s2 && f1 <= s1); 
}

int at_least_partially_overlapped(int f1, int f2, int s1, int s2) {
  return  1;
}

int decode_using(uint64_t input, int(*comparator)(int, int, int, int)) {
  // 0x0829084f
  // >> for 3 * 8 = 24 bits - 6 bytes
  // 0x0'000000'8 
  // >> for 2 * 8 = 16 bites - 4 bytes
  // 0x
  int f1 = (input & 0xff000000) >> (3 * sizeof(uint64_t));
  int f2 = (input & 0x00ff0000) >> (2 * sizeof(uint64_t));
  int s1 = (input & 0x0000ff00) >> (1 * sizeof(uint64_t));
  int s2 =  input & 0x000000ff;

  return comparator(f1, f2, s1, s2);
}


void print_buffer(const char* p, int len) {
  int i;
  for (i = 0; i < len; i++) {
    printf("%c", (p[i]));
  }
  printf("\n");
}

int main(int argc, char** argv) {
  int f_descr = open("input.txt", O_RDONLY);
  if (f_descr < 0) {
    printf("Can't open file\n");
    return -1;
  }

  char buffer[DATA_STR_L];

    
  int k;
  int next_jump = 0;
  int total = 0;
  
  while(k = read(f_descr, buffer, DATA_STR_L)) {
    int clean_len = clean_line(buffer, DATA_STR_L);
    next_jump += clean_len;
    print_buffer(buffer, clean_len);
    int r = process_line(buffer, clean_len);
    printf("code: 0x%08x\n", r);
    int overlapped = decode_using(r, fully_overlapped);
    printf("Is it fully overlapped %s\n", (overlapped ? "yes" : "no"));
    lseek(f_descr, next_jump, SEEK_SET);
    total += overlapped;
  }

  printf("Fully overlapped in total %d\n", total);
  close(f_descr);
  
  return 0;
}

/*
int main(void) {
  int* ptr = NULL;
  enum { N = 2048 };
  int i;
  for (i = 0; i < N; i++) {
    buf_push(ptr, i);
  }
  buf_free(ptr);
 
  return 0;
}
*/

/*
#include <stddef.h>

typedef struct example {
  int field1;
  char field2;
  char trick[0];
} example;

int main(void) {
  printf("size of struct example %ld\n", sizeof(example));
  printf("field1 offset %ld\n", offsetof(example, field1));
  printf("field2 offset %ld\n", offsetof(example, field2));
  printf("tricky field3 has offset %ld\n", offsetof(example, trick));
  printf("byte's aligning %ld\n",
     sizeof(example)-sizeof(int)-sizeof(char)-sizeof(char));
  return 0;
}
*/
