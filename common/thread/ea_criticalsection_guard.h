/*************************************************************************
    > File Name: ea_criticalsection_guard.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#ifndef __EA_CRITICAL_SECTION_GUARD_H__
#define __EA_CRITICAL_SECTION_GUARD_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace sdk {
namespace thread {

class CCriticalSection;

class CCriticalSectionGuard
{
public:
    CCriticalSectionGuard(CCriticalSection * section);
    CCriticalSectionGuard(CCriticalSection & section);
    
    ~CCriticalSectionGuard();

private:
    void Leave();

private:
    CCriticalSection * m_pCriticalSection;
};
    
}   
}  

#endif //__EA_CRITICAL_SECTION_GUARD_H__