#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "api/client.h"
#include "internal/net.h"

int CreateConnection(connection_t* connection,char* ip,uint16_t port){
        connection->fd=-1;
        return CreateAddr(&(connection->addr),ip,port);
}
int Connect(struct sockaddr_in* addr){
    int socketfd=socket(AF_INET,SOCK_STREAM,0);
    if(socketfd<0){
        return -1;
    }
    if(connect(socketfd,(struct sockaddr*) addr,sizeof(struct sockaddr_in))<0){
        close(socketfd);
        return -1;
    }
    return socketfd;
}
state_request_t Request(connection_t* connection,message_t* request,message_t* response,bool close_connection){
    if(request==NULL || response==NULL){
        if(close_connection && connection->fd!=-1){
            close(connection->fd);
            connection->fd=-1;
            return REQUEST_SUCCESS;
        }
        return REQUEST_FAILED;
    }
    if(connection->fd==-1){
        int fd=Connect(&(connection->addr));
        if(fd<0){
            return CONNECTION_ERROR;
        }
        connection->fd=fd;
    }
    if(SendMessage(connection->fd,request)==NET_SYS){
            close(connection->fd);
            connection->fd=-1;
            return REQUEST_FAILED;
    }
    parsing_context_t ctx;
    init_parser(&(ctx),response,CLIENT_LIMIT_LENGTH);
    NetworkResult_t state;
    while(true){
            state=RecvMessage(connection->fd,&ctx);
            switch(state){
                case NET_WAIT:
                    continue;
                case NET_SUCCESS:
                    free_parser(&(ctx));
                    if(close_connection){
                        close(connection->fd);
                        connection->fd=-1;
                    }
                    return REQUEST_SUCCESS;
                case NET_SYS:
                case NET_PROTOCOL:
                case NET_DISCONNECTED:
                    free_parser(&(ctx));
                    close(connection->fd);
                    connection->fd=-1;
                    return REQUEST_FAILED;
            }
    }
    
}

