/*************************************************************************
    > File Name: ea_criticalsection.h
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#ifndef __EA_CRITICAL_SECTION_H__
#define __EA_CRITICAL_SECTION_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace sdk {
namespace thread {
    
class CCriticalSection
{
public:
    static CCriticalSection * CreateCriticalSection();
    
public:
    virtual ~CCriticalSection(){}

    virtual void Enter() = 0;
    virtual void Leave() = 0;
};
    
}   
}  

#endif //__CRITICAL_SECTION_H__