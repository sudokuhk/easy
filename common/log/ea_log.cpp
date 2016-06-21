/*************************************************************************                                 
     > File Name: ea_log.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 21, 2016 AM10:43:49 CST
 ************************************************************************/

#include <log/ea_log.h> 

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>

namespace sdk {
namespace log {    

const char * s_ea_log_level_string[] =
{
    "",
    "EMERG",
    "ALERT",
    "CRIT",
    "ERROR",
    "NOTICE",
    "INFO",
    "DEBUG",
};

//#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#define FILENAME(x) strrchr(x,'/') ? strrchr(x,'/') + 1 : x

void ea_log_print(int level, const char * fullpath, const char * funcname,
    int line, const char * fmt, ...)
{
    if (level < log_emerge || level > log_debug) {
        return;
    }
    
    const char * szLevel = s_ea_log_level_string[level];
    char buf[4096] = {0};
    int off = 0;

    off += snprintf(buf + off, 4096 - off, "[%s:%s:%d][%s][P:%ld]", 
        FILENAME(fullpath), funcname, line, szLevel, pthread_self());
    
    va_list ap;
	va_start(ap, fmt);
	off += vsnprintf(buf + off, 4096 - off, fmt, ap);
	va_end(ap);
    buf[off ++] = '\n';
    buf[off ++] = '\0';

    fprintf(stdout, buf, off);
    return;
}

}   // namespace log 
}   // namespace sdk
