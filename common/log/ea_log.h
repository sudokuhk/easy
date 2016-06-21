/*************************************************************************                                 
     > File Name: ea_log.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 21, 2016 AM10:43:49 CST
 ************************************************************************/

#ifndef __EA_LOG_H__
#define __EA_LOG_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace sdk {
namespace log {    

#define log_emerge  1
#define log_alert   2
#define log_crit    3
#define log_error   4
#define log_notice  5
#define log_info    6
#define log_debug   7

extern void ea_log_print(int level, const char * fullpath, const char * funcname,
    int line, const char * fmt, ...);

}   // namespace sdk 
}   // namespace 


#define LOG_PRINT(level, fmt, args...)   \
    sdk::log::ea_log_print(level, __FILE__, __FUNCTION__, __LINE__, fmt, ##args);

#endif //__EA_LOG_H__