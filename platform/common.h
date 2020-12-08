#ifndef     COMMON_H
#define     COMMON_H

#include "log/Log.h"
#include <stdio.h>



using namespace std;

//日志宏
#define   LOG_DEBUG(fmt,...)    CLog::get_inst()->WriteLog(CLog::LL_DEBUG,"%s:%d:" fmt,__FILE__,__LINE__,## __VA_ARGS__)
#define   LOG_INFO(fmt,...)     CLog::get_inst()->WriteLog(CLog::LL_INFO,"%s:%d:" fmt,__FILE__,__LINE__,## __VA_ARGS__)
#define   LOG_WARN(fmt,...)     CLog::get_inst()->WriteLog(CLog::LL_WARN,"%s:%d" fmt,__FILE__,__LINE__,## __VA_ARGS__)
#define   LOG_ERROR(fmt,...)    CLog::get_inst()->WriteLog(CLog::LL_ERROR,"%s:%d:" fmt,__FILE__,__LINE__,## __VA_ARGS__)


#endif
