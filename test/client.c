#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api/client.h"

int GetChoice(){
    printf("Convert string => [UPPER/LOWER] : ");
    char* ptr;
    size_t len;
    while(1){
        ptr=NULL;
        len=0;
        ssize_t n=getline(&ptr,&len,stdin);
        if(n==EOF){
          exit(EXIT_FAILURE);   
        }
        if(strcmp(ptr,"UPPER\n")==0){
            free(ptr);
            return 0;
        }
        if(strcmp(ptr,"LOWER\n")==0){
            free(ptr);
            return 1;
        }
        free(ptr);
    }
}
int main(){
    connection_t connection;
    if(CreateConnection(&connection,"127.0.0.1",3000)<0){
            fprintf(stderr,"Format ip erreur\n");
            return 1;
    }
    message_t request;
    message_t response;
    uint16_t id=(uint16_t)GetChoice();
    while(true){
        printf("Input : ");
        char* str=NULL;
        size_t n=0;
        ssize_t len=getline(&str,&n,stdin);
        if(len==EOF){
            Request(&connection,NULL,NULL,true);
            break;
        }
        if(len==1){
            continue;
        }
        request.status=GET;
        request.id=id;
        request.length=len-1;
        request.body=str;
        switch(Request(&connection,&request,&response,false)){
            case CONNECTION_ERROR:
                fprintf(stderr,"CONNECTED ERROR\n");
                return 1;
            case REQUEST_FAILED :
                perror("error");
                fprintf(stderr,"REQUEST FAILED\n");
                return 1;
            case REQUEST_SUCCESS:
                printf("Output :");

                for(size_t i=0;i<response.length;i++){
                    putchar(response.body[i]);
                }
                putchar('\n');
                putchar('\n');
                free_message(&response);
                free(str);
        }
    }

}
