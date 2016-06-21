/*************************************************************************
    > File Name: ea_thread.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#ifndef __EA_THREAD_H__
#define __EA_THREAD_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <unistd.h>

#ifndef EA_THREAD_STACK_SIZE
#define EA_THREAD_STACK_SIZE 1024 * 1024
#endif

namespace sdk {
namespace thread {

enum ThreadPriority
{
    kLowPriority = 1,
    kNormalPriority = 2,
    kHighPriority = 3,
    kHighestPriority = 4,
    kRealtimePriority = 5
};

typedef void * ThreadObj;
typedef void * (*ThreadRunFunction)(ThreadObj);

class CThread
{
public:
    enum {
        kThreadMaxNameLength = 64
    };

public:
    static CThread * CreateThread(
            ThreadRunFunction func = 0,
            ThreadObj obj= 0,
            const char* threadName = 0,
            ThreadPriority prio = kNormalPriority);

    static int sleep(size_t ms);

public:
    CThread();
    
    virtual ~CThread();

    virtual bool Start() = 0;

    virtual bool Stop() = 0;

    virtual bool IsStarted();
    
protected:
    bool m_bIsStart;
};

}   
}  

#endif //__EA_THREAD_H__