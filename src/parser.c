
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "internal/parser.h"
#include "tool/alloc.h"

void init_parser(parsing_context_t* ctx,message_t* message,size_t limit_length){
    memset(message,0,sizeof(message_t));
    ctx->current=message;
    ctx->buffer=safe_alloc(limit_length+SIZEHEADER,1,NULL);
    ctx->state=PARSER_HEADER;
    ctx->length=0;
    ctx->limit_length=limit_length;
   
}
int stream_to_message(parsing_context_t* ctx,uint8_t* octet_stream,size_t len){
    if(ctx->state==PARSER_ERR) return -1;
    if(ctx->state==PARSER_FINISH && len>0){
        ctx->state=PARSER_ERR;
        return -1;
    }
   
    if(ctx->length+len>ctx->limit_length+SIZEHEADER){
            ctx->state=PARSER_ERR;
            return -1;
    }
    memcpy(ctx->buffer+ctx->length,octet_stream,len);
    ctx->length+=len;
    if(ctx->state==PARSER_HEADER && ctx->length>=SIZEHEADER){
         int8_t status=ctx->buffer[0];
         uint16_t id;
         uint32_t length;
         memcpy(&id,ctx->buffer+1,2);
         memcpy(&length,ctx->buffer+3,4);
         id=ntohs(id);
         length=ntohl(length);
         if(length>ctx->limit_length){
            ctx->state=PARSER_ERR;
            return -1;
         }
         ctx->current->status=(status_t)status;
         ctx->current->id=id;
         ctx->current->length=length;
         ctx->state=PARSER_BODY;
           
    }
    if(ctx->state==PARSER_BODY){
        if(ctx->length==ctx->current->length+SIZEHEADER){
           ctx->current->body=safe_alloc(ctx->current->length,1,NULL);
           memcpy(ctx->current->body,ctx->buffer+SIZEHEADER,ctx->current->length);
           ctx->state=PARSER_FINISH;
        }else if(ctx->length>ctx->current->length+SIZEHEADER){
            ctx->state=PARSER_ERR;
            return -1;
        }
    }
    return 0;
    
}

int message_to_stream(message_t* message,uint8_t* buffer,size_t* len){
    size_t size=message->length+SIZEHEADER;
    if(*len<size){
        *len=size;
        return -1;
    }
    buffer[0]=(uint8_t)message->status;
    uint16_t id=htons(message->id);
    uint32_t length=htonl(message->length);
    memcpy(buffer+1,&(id),2);
    memcpy(buffer+3,&(length),4);
    if(message->length){
       memcpy(buffer+SIZEHEADER,message->body,message->length);
    }
    *len=size;
    return 0;

    
}

state_parsing_t get_state(parsing_context_t* ctx){
    return ctx->state;
}
void reset_parser(parsing_context_t* ctx,message_t* message){
    memset(message,0,sizeof(message_t));
    ctx->current=message;
    ctx->length=0;
    ctx->state=PARSER_HEADER;
}

void free_parser(parsing_context_t* ctx){
    free(ctx->buffer);
}