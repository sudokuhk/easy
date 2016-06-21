/*************************************************************************
    > File Name: ea_condition_posix.cpp
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#include "thread/ea_condition_posix.h"
#include "thread/ea_criticalsection_posix.h"
#include "thread/ea_criticalsection_guard.h"

namespace sdk {
namespace thread {

CCondition * CConditionPosix::Create(CCriticalSection * critSect)
{
    CConditionPosix * pCond = new CConditionPosix(critSect);
    if (pCond == NULL) {
        return NULL;
    }

    if (!pCond->Init()) {
        delete pCond;
        pCond = NULL;
    }

    return pCond;
}

CConditionPosix::CConditionPosix(CCriticalSection * critSect)
    : m_pInputCritSect(critSect)
{
    if (m_pInputCritSect == NULL) {
        m_pCritSect = CCriticalSection::CreateCriticalSection();
    }
}
    
CConditionPosix::~CConditionPosix()
{
    m_pInputCritSect = NULL;
    if (m_pCritSect) {
        delete m_pCritSect;
    }
    pthread_cond_destroy(&m_cCond);
}

bool CConditionPosix::Init()
{
    if (pthread_cond_init(&m_cCond, NULL)) {
        return false;
    } 
    return true;
}

// Same as above but with a timeout.      
void CConditionPosix::Sleep()
{
    CCriticalSection * crit = (m_pInputCritSect == NULL ? 
        m_pCritSect : m_pInputCritSect);
    
    CCriticalSectionPosix * cs = 
        reinterpret_cast<CCriticalSectionPosix *>(crit);

    if (m_pInputCritSect == NULL) {
        CCriticalSectionGuard guard(m_pCritSect);
        pthread_cond_wait(&m_cCond, &cs->m_cMutex);
    } else {
        pthread_cond_wait(&m_cCond, &cs->m_cMutex);
    }
}          

// Same as above but with a timeout.       
bool CConditionPosix::Sleep(size_t ms)
{
}      

// Wakes one thread calling SleepCS().     
void CConditionPosix::Wake()
{
    if (m_pInputCritSect == NULL) {
        CCriticalSectionGuard guard(m_pCritSect);
        pthread_cond_signal(&m_cCond);
    } else {
        pthread_cond_signal(&m_cCond);
    }
    
} 

// Wakes all threads calling SleepCS().
void CConditionPosix::WakeAll()
{
    if (m_pInputCritSect == NULL) {
        CCriticalSectionGuard guard(m_pCritSect);
        pthread_cond_broadcast(&m_cCond);
    } else {
        pthread_cond_broadcast(&m_cCond);
    }
}
    
}   
}  