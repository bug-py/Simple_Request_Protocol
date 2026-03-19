#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "internal/protocol.h"


typedef enum{
    PARSER_ERR=-1,
    PARSER_FINISH=0,
    PARSER_BODY=1, 
    PARSER_HEADER=2,
}state_parsing_t;

typedef struct{
    message_t* current;
    char* buffer;
    size_t length;
    size_t limit_length;
    state_parsing_t state;
}parsing_context_t;
void init_parser(parsing_context_t* ctx,message_t* message,size_t limit_length);
int stream_to_message(parsing_context_t* ctx,uint8_t* octet_stream,size_t len);
int message_to_stream(message_t* message,uint8_t* buffer,size_t* len);
state_parsing_t get_state(parsing_context_t* ctx);
void reset_parser(parsing_context_t* ctx,message_t* message);
void free_parser(parsing_context_t* ctx);
#endif
