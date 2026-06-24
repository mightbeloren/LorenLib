#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#ifndef ARENA
#define ARENA

#define ALIGN_UP(currentOff) (((currentOff)+7)/8)*8 //aligning the current offset to next 8byte address in memory

typedef struct {
    size_t capacity;
    char* data;
    size_t offset;
}arena;

arena* arena_new(size_t size){
    arena* a=(arena*)malloc(sizeof(arena));
    if(a==NULL){
        printf("arena creation failed\n");
        return NULL;
    }
    a->data=(char*)malloc(size);
    if(a->data==NULL){
        free(a);
        printf("arena creation failed\n");
        return NULL;
    }
    a->offset=0;
    a->capacity=size;
    return a;
}

void* arena_alloc(arena* a,size_t size){
    a->offset=ALIGN_UP(a->offset);
    if(a->offset+size > a->capacity){
        printf("arena memory full");
        return NULL;
    }
    void* allocation=a->data+a->offset;
    a->offset=a->offset+size;
    return allocation;
}

void arena_reset(arena* a){
    if(a==NULL){
        return;
    }
    a->offset=0;
    return;
}

int arena_free(arena* a){
    if(a==NULL){
        return 1;
    }
    if(a->data!=NULL){
        free(a->data);
    }
    free(a);
    return 0;
}

#endif
