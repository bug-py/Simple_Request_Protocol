#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>

#include "internal/protocol.h"


#define CLIENT_LIMIT_LENGTH  10000
typedef struct{
   struct sockaddr_in addr;
   int fd;
}connection_t;

typedef enum{
  CONNECTION_ERROR=-2,
  REQUEST_FAILED=-1,
  REQUEST_SUCCESS=0
}state_request_t;
int CreateConnection(connection_t* connection,char* ip,uint16_t port);
state_request_t Request(connection_t* connection,message_t* request,message_t* response,bool close_connection);
#endif