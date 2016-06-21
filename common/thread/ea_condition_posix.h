/*************************************************************************
    > File Name: ea_condition.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#ifndef __EA_CONDITION_POSIX_H__
#define __EA_CONDITION_POSIX_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "thread/ea_condition.h"
#include <pthread.h>

namespace sdk {
namespace thread {

class CCriticalSection;

class CConditionPosix
    : public CCondition
{
public:
    static CCondition * Create(CCriticalSection * critSect);
public:
    CConditionPosix(CCriticalSection * critSect);
    
    virtual ~CConditionPosix();

    // Same as above but with a timeout.      
    virtual void Sleep();          

    // Same as above but with a timeout.       
    virtual bool Sleep(size_t ms);        

    // Wakes one thread calling SleepCS().     
    virtual void Wake();  

    // Wakes all threads calling SleepCS().
    virtual void WakeAll();

private:
    bool Init();
    
private:
    pthread_cond_t          m_cCond;
    CCriticalSection * m_pInputCritSect;
    CCriticalSection * m_pCritSect;
};
    
}   
}  

#endif //__EA_CONDITION_POSIX_H__