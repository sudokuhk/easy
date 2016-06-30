/*************************************************************************                                 
     > File Name: ea_timer.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2014 PM 05:12:49 CST
 ************************************************************************/

#include <net/ea_timer.h>

namespace sdk {
namespace net {    

bool ITimer::OnTimeout(size_t ms_now)
{
    if (ms_now - m_nBaseTime >= m_nInterval) {
        OnEvent();
    }
}

void ITimer::SetTimeout(size_t ms_interval)
{
    m_nInterval = ms_interval;
}

}   // namespace  
}   // namespace sdk
