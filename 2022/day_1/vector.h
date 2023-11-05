#ifndef _ELF_VECTOR_H_
#define _ELF_VECTOR_H_

#include <stdlib.h>
#include "no_implemented.h"


typedef struct {
    int* content;
    int length;
} batch;

typedef struct {
    int length; // number of elements overall in all batches
    batch** batches; 
    int number_of_batches;
    batch* current_batch;
} vector;

#define _BATCH_CAPACITY (32)
#define internal static


internal batch* create_batch() {
    batch* b = malloc(sizeof(batch));
    b->content = malloc(_BATCH_CAPACITY * sizeof(int));
    b->length = 0;
    return b;
}


internal void free_batches(batch** batches, int total) {
    int i;
    for (i = 0; i < total; i++) {
        free(batches[i]->content);
        free(batches[i]);
    }
    free(batches);
}


internal void integrate_batch(vector* v, batch* b) {
    batch** news = malloc((v->number_of_batches + 1) * sizeof(batch*));

    int k;
    for(k = 0; k < v->number_of_batches; k++) {
        news[k] = v->batches[k];
    }

    news[k] = b;

    free(v->batches);
    v->batches = news;
    v->number_of_batches += 1;
}


vector* create_vector() {
    vector* v = malloc(sizeof(vector));
    v->length = 0;
    v->number_of_batches = 1;

    v->batches = malloc(sizeof(batch*));
    v->batches[0] = create_batch();
    
    v->current_batch = v->batches[0];
    return v;
}


internal void push_item_to_batch(batch* b, int item) {
    int next = b->length;
    b->content[next] = item;
    b->length += 1;
}


void push_to(vector* v, int value) {
    if (v->current_batch->length < _BATCH_CAPACITY) {
        push_item_to_batch(v->current_batch, value);
    } else {
        batch* b = create_batch();
        push_item_to_batch(b, value);
        integrate_batch(v, b);
        v->current_batch = b;
    }
    v->length += 1;
}


void free_vector(vector* v) {
    free_batches(v->batches, v->number_of_batches);
    v->current_batch = NULL;
    free(v);
}


int get_at(vector* v, int index) {
    int batch_index = index / _BATCH_CAPACITY;
    int in_index = index % _BATCH_CAPACITY;

    return v->batches[batch_index]->content[in_index];
}


#endif