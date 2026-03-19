#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define SIZEHEADER sizeof(int8_t)+sizeof(uint16_t)+sizeof(uint32_t)
typedef enum{
    NOT_SUPPORTED=-2,
    NOT_FOUND=-1,
    OK=0,
    GET=1,
    POST=2
}status_t;

typedef struct{
    status_t status;
    uint16_t id;
    uint32_t length;
    char* body; 
}message_t; 
void free_message(message_t* message);
   
#endif
