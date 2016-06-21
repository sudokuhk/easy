/*************************************************************************
    > File Name: ea_condition.cpp
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM03:51:01 CST
 ************************************************************************/
 
#include "thread/ea_condition.h"
#include "thread/ea_condition_posix.h"

namespace sdk {
namespace thread {

CCondition * CCondition::CreateCondition(CCriticalSection * inputLock)
{
    return CConditionPosix::Create(inputLock);
}
    
}   
}  