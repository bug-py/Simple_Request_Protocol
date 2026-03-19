#include "tool/alloc.h"
#include<stdio.h>
#include <stdlib.h>
void* safe_alloc(size_t length,size_t count,void* ptr){
    void* new_ptr=realloc(ptr,length*count);
    if(new_ptr==NULL){
        perror("safe_alloc failed");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}