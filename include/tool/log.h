#ifndef LOG_H
#define LOG_H
#include<stdbool.h>
typedef enum{
   LOG_INFO=1,
   LOG_WARN=0,
   LOG_ERR= -1
}log_t;

void LOG(bool log,log_t type,char* content);
#endif 