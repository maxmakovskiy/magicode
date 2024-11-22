#ifndef FILEZZ_H_
#define FILEZZ_H_

#include "stack.h"
#include "dl_list.h"

uint32_t str_bin_to_uint(const char* buffer);
int32_t read_file_as_uint(const char* filename, stack* storage);
int32_t read_file_to_list(const char* filename, List* storage);

#endif
