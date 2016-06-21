/*************************************************************************
    > File Name: ea_thread_posix.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#ifndef __EA_THREAD_POSIX_H__
#define __EA_THREAD_POSIX_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "thread/ea_thread.h"

#include <pthread.h>

namespace sdk {
namespace thread {

class CCriticalSection;
class CCondition;

class CThreadPosix 
    : public CThread
{
public:
    static CThread * Create(ThreadRunFunction func,
        ThreadObj obj, const char* threadName, ThreadPriority prio);
    
public:
    CThreadPosix(ThreadRunFunction func,
        ThreadObj obj, const char* threadName, ThreadPriority prio);
    
    virtual ~CThreadPosix();

    virtual bool Start();

    virtual bool Stop();

    void Run();

    bool Init();
    
private:
    static void * ThreadFunc(void * arg);
    
private:
    ThreadRunFunction       m_pFunc;
    ThreadObj               m_pObj;

    CCriticalSection *      m_pCritSec;
    bool                    m_bAlive;
    bool                    m_bDead;
    ThreadPriority          m_ePrio;
    
    //cond for start
    CCriticalSection*       m_pCriCond;
    CCondition*             m_pCond;
    bool                    m_bRunning;

    // zero-terminated thread name string
    char                    m_szName[kThreadMaxNameLength];
    bool                    m_bInitName;

    pthread_attr_t          m_cAttr;
    pthread_t               m_tThreadId;
};

}   
}  

#endif //__EA_THREAD_POSIX_H__