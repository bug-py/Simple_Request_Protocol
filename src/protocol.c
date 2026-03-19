#include "internal/protocol.h"
#include <stdlib.h>
void free_message(message_t* message){
    if(message->length>0){
        free(message->body);
    }
    message->status=OK;
    message->length=0;
    message->id=0;
    message->body=NULL;
}