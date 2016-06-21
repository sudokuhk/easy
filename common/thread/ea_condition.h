/*************************************************************************
    > File Name: ea_condition.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#ifndef __EA_CONDITION_H__
#define __EA_CONDITION_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <unistd.h>

namespace sdk {
namespace thread {

class CCriticalSection;

class CCondition
{
public:
    static CCondition * CreateCondition(CCriticalSection * inputLock = 0);
    
public:
    virtual ~CCondition() {}

    // Same as above but with a timeout.      
    virtual void Sleep() = 0;          

    // Same as above but with a timeout.       
    virtual bool Sleep(size_t ms) = 0;        

    // Wakes one thread calling SleepCS().     
    virtual void Wake() = 0;  

    // Wakes all threads calling SleepCS().
    virtual void WakeAll() = 0;
};
    
}   
}  

#endif //__EA_CONDITION_H__