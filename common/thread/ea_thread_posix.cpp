/*************************************************************************
    > File Name: ea_condition.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/

#include "thread/ea_thread_posix.h"
#include "thread/ea_criticalsection.h"
#include "thread/ea_condition.h"
#include "thread/ea_criticalsection_guard.h"

#include "log/ea_log.h"

#include <string.h>

namespace sdk {
namespace thread {

CThread * CThreadPosix::Create(ThreadRunFunction func,
    ThreadObj obj, const char* threadName, ThreadPriority prio)
{
    CThreadPosix * pThread = new CThreadPosix(func, obj, threadName, prio);
    if (pThread != NULL && !pThread->Init()) {
        delete pThread;
        pThread = NULL;
    }

    return pThread;
}

CThreadPosix::CThreadPosix(ThreadRunFunction func,
    ThreadObj obj, const char* threadName, ThreadPriority prio)
    : m_pFunc(func)
    , m_pObj(obj)
    , m_pCritSec(CCriticalSection::CreateCriticalSection())
    , m_bAlive(false)
    , m_bDead(true)
    , m_ePrio(prio)
    , m_bRunning(false)
    , m_szName()
    , m_bInitName(false)
    , m_cAttr()
    , m_tThreadId(0)
{
    m_pCriCond  = CCriticalSection::CreateCriticalSection();
    m_pCond     = CCondition::CreateCondition(m_pCriCond);
    
    if (threadName) {
        m_bInitName = true;
        strncpy(m_szName, threadName, kThreadMaxNameLength);
    } 
    m_szName[kThreadMaxNameLength] = '\0';
    
    LOG_PRINT(log_debug, "[%s]Create Thread!", m_szName);
}
    
CThreadPosix::~CThreadPosix()
{
    pthread_attr_destroy(&m_cAttr);

    delete m_pCond;
    delete m_pCriCond;
    delete m_pCritSec;
}

void * CThreadPosix::ThreadFunc(void * arg)
{
    CThreadPosix * pObj = static_cast<CThreadPosix *>(arg);
    pObj->Run();

    return 0;
}

bool CThreadPosix::Init()
{
    if (pthread_attr_init(&m_cAttr)) {
        return false;
    } 
    
    return true;
}

bool CThreadPosix::Start()
{
    LOG_PRINT(log_debug, "[%s]Start Thread!", m_szName);
    if (m_pFunc == 0) {
        LOG_PRINT(log_info, "start failed, null func!");
        return false;
    }
    
    LOG_PRINT(log_debug, "[%s]set thread attribute!", m_szName);
    int result = 0;

    //result |= pthread_attr_setdetachstate(&m_cAttr, PTHREAD_CREATE_DETACHED);
    result |= pthread_attr_setstacksize(&m_cAttr, EA_THREAD_STACK_SIZE);

    result |= pthread_create(&m_tThreadId, &m_cAttr, 
        &CThreadPosix::ThreadFunc, this);

    LOG_PRINT(log_debug, "[%s]ret value:%d!", m_szName, result);
    if (result != 0) {
        return false;
    }
    
    m_bIsStart = true;

    {
        CCriticalSectionGuard guard(m_pCriCond);
        if (!m_bRunning) {
            return false;
        }
    }
    LOG_PRINT(log_debug, "[%s]Start Thread Done!", m_szName);
}


bool CThreadPosix::Stop()
{
    bool dead = false;
    LOG_PRINT(log_debug, "[%s]Stop Thread Begin!", m_szName);
    
    {
        CCriticalSectionGuard guard(m_pCritSec);
        m_bAlive    = false;
        dead        = m_bDead;
    }

    pthread_join(m_tThreadId, NULL);
    LOG_PRINT(log_debug, "[%s]pthread_join done!", m_szName);
    
    m_bIsStart = false;

    return true;
}

void CThreadPosix::Run()
{
    LOG_PRINT(log_debug, "[%s]Run Thread!", m_szName);
    {
        CCriticalSectionGuard guard(m_pCritSec);
        m_bAlive    = true;
        m_bDead     = false;
    }

    {
        CCriticalSectionGuard guard(m_pCriCond);
        m_bRunning  = true;
        m_pCond->WakeAll();
    }

    bool running = true;
    do {
        if (!m_pFunc || !m_pFunc(m_pObj)) {
            running = false;
        }

        {
            CCriticalSectionGuard guard(m_pCritSec);
            if (!running) {
                m_bAlive = running;
            }
            running = m_bAlive;
        }
    } while (running);

    {
        CCriticalSectionGuard guard(m_pCritSec);
        m_bDead     = true;
    }
    LOG_PRINT(log_debug, "[%s]Run End Thread!", m_szName);
}

}   
}  
