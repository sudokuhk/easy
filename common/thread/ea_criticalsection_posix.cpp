/*************************************************************************
    > File Name: ea_criticalsection_posix.cpp
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#include "thread/ea_criticalsection_posix.h"

namespace sdk {
namespace thread {

CCriticalSectionPosix::CCriticalSectionPosix()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_cMutex, &attr);
}

CCriticalSectionPosix::~CCriticalSectionPosix()
{
    pthread_mutex_destroy(&m_cMutex);
}

void CCriticalSectionPosix::Enter()
{
    pthread_mutex_lock(&m_cMutex);
}

void CCriticalSectionPosix::Leave()
{
    pthread_mutex_unlock(&m_cMutex);
}

}   
}  