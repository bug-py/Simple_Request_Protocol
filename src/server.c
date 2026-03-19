#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <errno.h>

#include<sys/socket.h>
#include <sys/select.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#include <unistd.h>

#include "api/server.h"
#include "internal/net.h"
#include "tool/log.h"
#include "tool/alloc.h"

typedef struct{
    int fd;
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
    message_t request;
    parsing_context_t ctx;
}client_t;

typedef enum{
   ACCEPT_SUCCESS=0,
   ACCEPT_ERR=-1,
   ACCEPT_OVERFLOW=-2
}AcceptResult_t;
void ClearClient(client_t* client,fd_set* current_socket){
   free_parser(&(client->ctx));
   FD_CLR(client->fd,current_socket);
   close(client->fd);
   free(client);
}
void Clean(int serverfd,client_t* client[FD_SETSIZE],fd_set* current_socket){
      for(int i=0;i<FD_SETSIZE;i++){
             
             if(FD_ISSET(i,current_socket) && i!=serverfd){
                 
                 ClearClient(client[i],current_socket);
             }
      }
      
      close(serverfd);
     
}

NetworkResult_t HandleClient(client_t* client,callback_t callback){
   NetworkResult_t state_recv=RecvMessage(client->fd,&(client->ctx));
   switch(state_recv){
      case NET_SUCCESS:
         message_t response;
         callback(client->ip,client->port,&(client->request),&response);
         free_message(&(client->request));
         reset_parser(&(client->ctx),&(client->request));
         NetworkResult_t state_send=SendMessage(client->fd,&response);
         free_message(&response);
         return state_send;
      default:
         return state_recv;
   }
}
AcceptResult_t AcceptClient(int serverfd,client_t* client,fd_set* current_socket){
      struct sockaddr_in addr;
      socklen_t len=sizeof(addr);
      int clientfd=accept(serverfd,(struct sockaddr*) &addr,&len);
      if(clientfd<0){
         return ACCEPT_ERR;
      }
      if(clientfd>=FD_SETSIZE){
         close(clientfd);
         return  ACCEPT_OVERFLOW;
      }
      client->fd=clientfd;
      inet_ntop(AF_INET,&addr.sin_addr,client->ip,INET_ADDRSTRLEN);
      client->port=ntohs(addr.sin_port);
      init_parser(&(client->ctx),&(client->request),SERVER_LIMIT_LENGTH);
      FD_SET(clientfd,current_socket);
      return ACCEPT_SUCCESS;
}
CreateResult_t CreateServer(char* ip,uint16_t port){
     struct sockaddr_in serveraddr;
     if(CreateAddr(&serveraddr,ip,port)<0){
         return CREATE_IP;
     }

     int serverfd=socket(AF_INET,SOCK_STREAM,0);
     if(serverfd<0){
        return CREATE_SYS;
     }
     int opt=1;
     if(setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))<0){
        close(serverfd);
        return CREATE_SYS;
     }
    
     if(bind(serverfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
        close(serverfd);
        return CREATE_SYS;
     }
     listen(serverfd,5);
     return serverfd;
}
int ServerLoop(int serverfd,bool log,callback_t callback){
    if(serverfd<0 || serverfd>=FD_SETSIZE){
      errno=EBADF;
      LOG(log,LOG_ERR,"Serverfd is invalid");
      return -1;
    }
    client_t* clients[FD_SETSIZE]={0};
    fd_set current_socket;
    fd_set ready_socket;
    FD_ZERO(&current_socket);
    FD_SET(serverfd,&current_socket);
    
    while(1){
      
      ready_socket=current_socket;
     
      if(select(FD_SETSIZE,&ready_socket,NULL,NULL,NULL)<0){
           
            Clean(serverfd,clients,&current_socket);
            
            LOG(log,LOG_ERR,"Select failed");
            return -1;
      }
      for(int i=0;i<FD_SETSIZE;i++){
         if(FD_ISSET(i,&ready_socket)){
            if(i==serverfd){
               client_t* NewClient=safe_alloc(sizeof(client_t),1,NULL);
               switch(AcceptClient(serverfd,NewClient,&current_socket)){
                  case ACCEPT_ERR:
                     Clean(serverfd,clients,&current_socket);
                     LOG(log,LOG_ERR,"accept failed");
                     return -1;
                  case ACCEPT_OVERFLOW:
                     free(NewClient);
                     LOG(log,LOG_WARN,"Refused connection too many client");
                     break;
                  case ACCEPT_SUCCESS:
                    clients[NewClient->fd]=NewClient;
                    LOG(log,LOG_INFO,"Client connect");
                    break;
               }
              
            }else{
               switch(HandleClient(clients[i],callback)){
                  case NET_WAIT:
                        LOG(log,LOG_INFO,"Recv data client");
                        continue;
                  case NET_SUCCESS:
                        LOG(log,LOG_INFO,"Send request client");
                        continue;
                  case NET_DISCONNECTED:
                        LOG(log,LOG_INFO,"Client disconnected");
                        break;
                  case NET_PROTOCOL:
                        LOG(log,LOG_WARN,"Client protocol error");
                        break;
                  case NET_SYS:
                       LOG(log,LOG_WARN,"Client brutal disconnection");
                       break;
                     
               }
               ClearClient(clients[i],&current_socket);
               clients[i]=NULL;
            } 
            

           }
      }
    }

}