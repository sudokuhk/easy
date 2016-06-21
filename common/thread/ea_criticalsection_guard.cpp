/*************************************************************************
    > File Name: ea_criticalsection.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#include "thread/ea_criticalsection_guard.h"

#include "thread/ea_criticalsection.h"

namespace sdk {
namespace thread {

CCriticalSectionGuard::CCriticalSectionGuard(CCriticalSection * section)
    : m_pCriticalSection(section)
{
}

CCriticalSectionGuard::CCriticalSectionGuard(CCriticalSection & section)
    : m_pCriticalSection(&section)
{
}
    
CCriticalSectionGuard::~CCriticalSectionGuard()
{
    if (m_pCriticalSection) {
        Leave();
    }
}

void CCriticalSectionGuard::Leave()
{
    m_pCriticalSection->Leave();
    m_pCriticalSection = 0;
}
    
}   
}  