#ifndef SERVER_H

#include <stdint.h>
#include <stdbool.h>

#include "internal/protocol.h"

#define SERVER_LIMIT_LENGTH  10000
typedef enum{
   CREATE_IP=-1,
   CREATE_SYS=-2
}CreateResult_t;
typedef void (*callback_t)(char* ip,uint16_t port,message_t* request,message_t* response);
CreateResult_t CreateServer(char* ip,uint16_t port);
int ServerLoop(int serverfd,bool log,callback_t callback);
#endif