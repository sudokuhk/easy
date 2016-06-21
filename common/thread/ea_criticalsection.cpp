/*************************************************************************
    > File Name: ea_criticalsection.cpp
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#include "thread/ea_criticalsection.h"

#include "thread/ea_criticalsection_posix.h"

namespace sdk {
namespace thread {
    
CCriticalSection * CCriticalSection::CreateCriticalSection()
{
    return new CCriticalSectionPosix();
}
    
}   
}  