#include <stdio.h>
#include <time.h>
#include "tool/log.h"
#define MAX_LEN_STR_TIME 22
void LOG(bool log,log_t type,char* content){
   if(log){
      time_t timestamp=time(NULL);
      struct tm* now=localtime(&timestamp);
      char strtime[MAX_LEN_STR_TIME];
      strftime(strtime,MAX_LEN_STR_TIME,"[ %D %T ]",now);
      switch(type){
         case LOG_INFO:
            printf("%s INFO : %s \n",strtime,content);
            break;
         case LOG_WARN:
            printf("%s WARN : %s \n",strtime,content);
            break;
         case LOG_ERR:
            printf("%s FATAL ERROR : %s \n",strtime,content);
            break;
      }
   }
}
