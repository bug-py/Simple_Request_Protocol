#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "api/server.h"
#include "tool/alloc.h"
void Upper(char* dest ,char* src,size_t len){
    for(size_t i=0;i<len;i++){
        dest[i]=(char)toupper(src[i]);
    }
}
void response(char* ip,uint16_t port,message_t* request,message_t* response){
    memset(response,0,sizeof(message_t));
    printf("ip :%s port : %u",ip,(unsigned int)port);
    if(request->status==GET){
         switch(request->id){
            case 0:
                size_t len=request->length;
                printf("%lu\n",len);
                char* upper_str=safe_alloc(sizeof(char),len,NULL);
                Upper(upper_str,request->body,len);
                response->status=OK;
                response->length=len;
                response->body=upper_str;
                return;

            default:
                response->status=NOT_FOUND;
                return ;
          }
          
    }
    response->status=NOT_SUPPORTED;
    return ;
    

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
            
     
    ServerLoop(serverfd,true,response);
    perror("Error Sys");
}