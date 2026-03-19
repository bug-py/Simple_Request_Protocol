#ifndef NET_H
#define NET_H
#include <stdint.h>
#include <netinet/in.h>

#include "internal/protocol.h"
#include "internal/parser.h"

typedef enum{
    NET_WAIT=2,
    NET_DISCONNECTED=1,
    NET_SUCCESS=0,
    NET_SYS=-1,
    NET_PROTOCOL=-3,
} NetworkResult_t;

NetworkResult_t SendMessage(int fd,message_t* message);
NetworkResult_t RecvMessage(int fd,parsing_context_t* ctx);

int CreateAddr(struct sockaddr_in* addr,char* ip,uint16_t port);
#endif