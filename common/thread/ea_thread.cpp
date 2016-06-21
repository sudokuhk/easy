/*************************************************************************
    > File Name: ea_condition.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/

#include "thread/ea_thread.h"
#include "thread/ea_thread_posix.h"

namespace sdk {
namespace thread {

CThread * CThread::CreateThread(ThreadRunFunction func,
    ThreadObj obj, const char* threadName, ThreadPriority prio)
{
    return CThreadPosix::Create(func, obj, threadName, prio);
}

int CThread::sleep(size_t ms)
{
}

CThread::CThread()
    : m_bIsStart(false)
{
}

CThread::~CThread()
{
}

bool CThread::IsStarted()
{
    return m_bIsStart;
}

}   
}  