#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_LEN 5


void 
treat_line(
  const char* line, int line_len,
  int* ones, int* zeros
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


int 
most_common_stats(
  const int* ones, const int* zeros
) {
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


int 
least_common_stat(
  const int* ones, const int* zeros
) {
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
  char* filename = "data.txt";

  FILE* file;
  file = fopen(filename, "r");
  printf("Opening file: %s ...\n", filename);

  int ones_stat[SAMPLE_LEN] = {0, 0, 0, 0, 0};
  int zeros_stat[SAMPLE_LEN] = {0, 0, 0, 0, 0};

  char line_buf[128];
  int iter_buf = 0;
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    if ((char)ch == '\n') {
      treat_line(line_buf, iter_buf, ones_stat, zeros_stat);
      iter_buf = 0;
    } else {
      line_buf[iter_buf] = (char)ch;
      iter_buf += 1;
    }
  }

  fclose(file);

  int most = most_common_stats(ones_stat, zeros_stat);
  int least = least_common_stat(ones_stat, zeros_stat);
  int result = most * least;

  printf("result = %d\n", result);

    return 0;
}
