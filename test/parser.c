#include <stdio.h>
#include <string.h>
#include "internal/parser.h"
#include "tool/alloc.h"
void PrintMessage(message_t* message){
    printf("STATUS : ");
    switch(message->status){
        case OK:
            puts("OK");
            break;
        case GET:
            puts("GET");
            break;
        case POST:
            puts("POST");
            break;
        case NOT_FOUND:
            puts("NOT_FOUND");
            break;
        case NOT_SUPPORTED:
            puts("NOT_SUPPORTED");
            break;
    }
    printf("ID : %i\n",(int)message->id);
    printf("LENGTH : %u\n",(unsigned int)message->length);
    printf("BODY : ");
    for(size_t i=0;i<message->length;i++){
        putchar(message->body[i]);
    }
    putchar('\n');
}
int main(){
    message_t message;
    parsing_context_t ctx;
    uint8_t stream[]={0,0,0,0,0,0,13,72,101,108,108,111,32,87,111,114,108,100,32,33};
    init_parser(&ctx,&message,1000);
    stream_to_message(&ctx,stream,sizeof(stream));
    switch(get_state(&ctx)){
        case PARSER_FINISH:
            printf("Parisng success\n");
            break;
        case PARSER_ERR:
            printf("Parsing failed\n");
            return 1;
        case PARSER_HEADER:
        case PARSER_BODY:
            printf("Stream Incomplete\n");
            return 1;
    }
    PrintMessage(&message);
    free_message(&message);
    free_parser(&ctx);
    char str[]="PLAYER POSTION\nX\r4584552554455455.4555554555\r\nY\r4565884555414555.575757588\r\n";
    size_t len=strlen(str)+SIZEHEADER;
    uint8_t buffer[len];
    message.status=GET;
    message.id=1485;
    message.length=strlen(str);
    message.body=safe_alloc(sizeof(char),strlen(str)+1,NULL);
    strcpy(message.body,str);
    int result=message_to_stream(&message,buffer,&len);
    if(result<0){
        printf("buffer overflow\n");
        return 1;
    }
    printf("STREAM : ");
    for(size_t i=0;i<len;i++){
        printf(" |%d| ",(int) buffer[i]);
    }
    putchar('\n');


    
    
    return 0;
}

