#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "vector.h"


int pow(int base, int exp) {
    if (exp == 0) {
        return 1;
    }
    int result = base;
    int i;
    for (i = 1; i < exp; i++) {
        result *= base;
    }

    return result;
}

int str_to_int(int* str) {
    int result = 0;

    int j, i;
    for (j = 1, i = str[0]-1; j <= str[0]; j++, i--) {
        result += (str[j] - 48) * pow(10, i);
    }

    return result;
}


void traverse_file_for_top_one(FILE* fp) {
    int c;

    int elf = 1;
    int winner = elf;
    int highest = 0;
    int current = 0;

    int buffer[10];
    int i = 1;

    while((c = getc(fp)) != EOF) {
        if ((48 <= c) && (c <= 57)) {
            buffer[i] = c;
            i += 1;
        } else {
            buffer[0] = i-1;

            if (buffer[0]) {
                current += str_to_int(buffer);

                i = 1;
            } else {
                printf("elf %d has %d calories\n", elf, current);

                if (current > highest) {
                    highest = current;
                    winner = elf;
                }

                elf += 1;
                current = 0;
            }
        }         
    }

    printf("elf %d has highest number of calories %d\n", winner, highest);
}


void print_hello(int num) {
    printf("elf says hello to %d\n", num);
}



void traverse_file(FILE* fp, vector* v) {
    int c;

    int buffer[10];
    int i = 1;

    int elf = 0;
    int current = 0;

    while((c = getc(fp)) != EOF) {
        if ((48 <= c) && (c <= 57)) {
            buffer[i] = c;
            i += 1;
        } else {
            buffer[0] = i-1;

            if (buffer[0]) {
                current += str_to_int(buffer);
                i = 1;
            } else {
                printf("elf %d has %d calories\n", elf, current);

                push_to(v, current);

                elf += 1; 
                current = 0;
            }
        }                
    }


}


int main() {

    FILE *file = NULL;

    if ((file = fopen("input.txt", "r")) == NULL) {
        printf("can't open file\n");
        return 1; 
    }

    vector* v = create_vector();

    traverse_file(file, v);


    // do work here






    free_vector(v);

    fclose(file);
     



    // vector* v = create_vector();

    // int j = 1;
    // for (; j <= 740000; j++) {
    //     push_to(v, j);
    // }

    // // for(j = 0; j < v->length; j++) {
    // //     int item = get_at(v, j);
    // //     printf("element %d at index %d\n", item, j);
    // // }    
    // printf("vector has %d elements in total\n", v->length);

    // free_vector(v);


    // int input[10];
    // input[0] = 6;
    // input[1] = 49;
    // input[2] = 50;
    // input[3] = 51;
    // input[4] = 52;
    // input[5] = 53;
    // input[6] = 54;
    // int result = str_to_int(input);
    // printf("%d\n", result);
    // assert(result == 123);

    return 0;
}