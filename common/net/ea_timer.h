/*************************************************************************                                 
     > File Name: ea_timer.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2014 PM 05:12:49 CST
 ************************************************************************/

#ifndef __EA_TIMER_H__
#define __EA_TIMER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <unistd.h>

namespace sdk {
namespace net {    

class ITimer
{
public:
    virtual  ~ITimer() {}

    bool OnTimeout(size_t ms_now);
    void SetTimeout(size_t ms_interval);

    virtual void OnEvent() = 0;
    
protected:
    size_t m_nBaseTime;
    size_t m_nInterval;
};

}   // namespace  
}   // namespace sdk

#endif //__EA_TIMER_H__