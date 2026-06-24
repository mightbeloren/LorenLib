#include "arena.h"
#include <stdlib.h>
#include <string.h>
#ifndef VECTOR
#define VECTOR

typedef struct {
    size_t capacity;
    size_t count;
    size_t elementSize;
    void *data;
    arena *allocator;
} vector;

vector *vec_new(size_t elementSize, size_t baseCapacity, arena *allocator) {
    if (allocator == NULL) {
        printf("allocator is not initialized\n");
        return NULL;
    }
    vector *vec = (vector *)arena_alloc(allocator, sizeof(vector));
    if (vec == NULL) {
        printf("unable to allocate vector instance\n");
        return NULL;
    }
    vec->allocator = allocator;
    vec->elementSize = elementSize;
    vec->count = 0;
    vec->capacity = baseCapacity;
    vec->data = arena_alloc(allocator, elementSize * baseCapacity);
    if (vec->data == NULL) {
        return NULL;
    }
    return vec;
}

// returns 1 if failed to push element to vector
// returns 0 if success
int vec_push(vector *vec, void *element) {
    if (vec == NULL) {
        printf("vector is not initialized\n");
        return 1;
    }
    if (element == NULL) {
        printf("element is NULL\n");
        return 1;
    }

    if (vec->capacity == vec->count) {
        printf("vector is full\n");
        return 1;
    }

    memcpy((char *)vec->data + (vec->elementSize * vec->count), element,
        vec->elementSize);
    vec->count++;
    return 0;
}

int vec_pop(vector *vec) {
    if (vec == NULL) {
        printf("vector is not initialized\n");
        return 1;
    }
    if (vec->count == 0) {
        printf("vector is already empty, nothing to pop\n");
        return 1;
    }
    vec->count--;
    return 0;
}

void *vec_at(vector *vec, size_t index) {
    if (vec == NULL) {
        printf("vector is not initialized\n");
        return NULL;
    }
    if (vec->count < index) {
        printf("vector out of bounds\n");
        return NULL;
    }
    void *element = (char *)vec->data + (index * vec->elementSize);
    return element;
}


int vec_clear(vector *vec) {
    if (vec == NULL) {
        printf("vector is not initialized\n");
        return 1;
    }
    vec->count = 0;
    arena_reset(vec->allocator);
    return 0;
}

#endif
