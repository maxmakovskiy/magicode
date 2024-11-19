#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_LEN 24


static int ones[SAMPLE_LEN];
static int zeros[SAMPLE_LEN];


void 
treat_line(
  const char* line, int line_len
) {
  for (int i = 0; i < line_len; i++) {
    const int ASCII_0 = 48;
    if (line[i] == ASCII_0) {
      zeros[i] += 1;
    } else {
      ones[i] += 1;
    }
  }
}

int compute_most_common() {
  int result = 0;
  
  for (int i = 0; i < SAMPLE_LEN; i++) {
    if (ones[i] > zeros[i]) {
      result = 1 | (result << 1);
    } else {
      result = 0 | (result << 1);
    }
  }

  return result;
}


int compute_least_common() {
  int result = 0;
  
  for (int i = 0; i < SAMPLE_LEN; i++) {
    if (ones[i] < zeros[i]) {
      result = 1 | (result << 1);
    } else {
      result = 0 | (result << 1);
    }
  }

  return result;
}

int main(int argc, char** argv) {
  // if (argc < 2) {
  //   printf("You should pass the name of a file to open\n");
  //   return -1;
  // }

  // char* filename = argv[1];
  // char* filename = "data.txt";
  char* filename = "big_data.txt";

  FILE* file;
  file = fopen(filename, "r");
  printf("Opening file: %s ...\n", filename);


  char line_buf[128];
  int iter_buf = 0;
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    if ((char)ch == '\n') {
      treat_line(line_buf, iter_buf);
      iter_buf = 0;
    } else {
      line_buf[iter_buf] = (char)ch;
      iter_buf += 1;
    }
  }

  fclose(file);

  int most = compute_most_common();
  int least = compute_least_common();
  int result = most * least;

  printf("result = %d\n", result);

  return 0;
}
