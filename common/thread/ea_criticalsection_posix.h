/*************************************************************************
    > File Name: ea_criticalsection_posix.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#ifndef __EA_CRITICAL_SECTION_POSIX_H__
#define __EA_CRITICAL_SECTION_POSIX_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <pthread.h>

#include "thread/ea_criticalsection.h"
#include "thread/ea_condition_posix.h"

namespace sdk {
namespace thread {
    
class CCriticalSectionPosix
    : public CCriticalSection
{
public:
    CCriticalSectionPosix();
    virtual ~CCriticalSectionPosix();

    virtual void Enter();
    virtual void Leave();

private:
    pthread_mutex_t m_cMutex;
    friend class CConditionPosix;
};
    
}   
}  

#endif //__EA_CRITICAL_SECTION_POSIX_H__