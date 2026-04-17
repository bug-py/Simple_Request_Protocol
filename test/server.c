#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "api/server.h"

void Upper(char* dest ,char* src,size_t len){
    for(size_t i=0;i<len;i++){
        dest[i]=(char)toupper(src[i]);
    }
}
void Lower(char* dest,char* src,size_t len){
    for(size_t i=0;i<len;i++){
        dest[i]=(char)tolower(src[i]);
    }
}
void response(char* ip,uint16_t port,message_t* request,message_t* response){
    memset(response,0,sizeof(message_t));
    printf("IP :%s PORT : %u\n",ip,(unsigned int)port);
    if(request->status==POST){
        size_t len=request->length;
         switch(request->id){
            case 0:
                char* upper_str=malloc(sizeof(char)*len);
                Upper(upper_str,request->body,len);
                response->status=OK;
                response->length=len;
                response->body=upper_str;
                break;
            case 1:
                char* lower_str=malloc(sizeof(char)*len);
                Lower(lower_str,request->body,len);
                response->status=OK;
                response->length=len;
                response->body=lower_str;
                break;
            default:
                response->status=NOT_FOUND;
                break;
          }
          
    }else{
          response->status=NOT_SUPPORTED;
    }
}
int main(){
    int serverfd=CreateServer("127.0.0.1",3000);
    switch(serverfd){
        case CREATE_IP:
            fprintf(stderr,"Ip error format\n");
            return 1;
        case CREATE_SYS:
            perror("create socket err");
            return 1;
    }
            
     
    if(ServerLoop(serverfd,true,response)==-1){
        perror("Server error");
    }
    
}