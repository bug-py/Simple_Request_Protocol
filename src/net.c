#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "internal/net.h"
#include "tool/alloc.h"

NetworkResult_t SendMessage(int fd,message_t* message){
     size_t len=message->length+SIZEHEADER;
     uint8_t* stream=safe_alloc(sizeof(uint8_t),len,NULL);
     message_to_stream(message,stream,&len);
     size_t send_octet=0;
     while(send_octet!=len){
      ssize_t n=send(fd,stream+send_octet,len-send_octet,0);
      if(n<=0){
         free(stream);
         return NET_SYS;
      }
      send_octet+=n;
     }
     free(stream);
     return NET_SUCCESS;
}
NetworkResult_t RecvMessage(int fd,parsing_context_t* ctx){
    uint8_t buffer[8000];
    ssize_t n=recv(fd,buffer,sizeof(buffer),0);
    if(n==0) return NET_DISCONNECTED;
    if(n<0) return NET_SYS;
    stream_to_message(ctx,buffer,n);
    switch(get_state(ctx)){
        case PARSER_ERR:
            return NET_PROTOCOL;
        case PARSER_FINISH:
            return NET_SUCCESS;
        case PARSER_BODY:
        case PARSER_HEADER:
            return NET_WAIT;
    }


}
int CreateAddr(struct sockaddr_in* addr,char* ip,uint16_t port){
    memset(addr,0,sizeof(struct sockaddr_in));
    addr->sin_family=AF_INET;
    addr->sin_port=htons(port);
    if(inet_pton(AF_INET,ip,&(addr->sin_addr))==1){
           return 0;
    }
    return -1;
    
}