/*************************************************************************                                 
     > File Name: ea_timer.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2014 PM 05:12:49 CST
 ************************************************************************/

#include <net/ea_timer.h>

#include <net/ea_react.h>

namespace sdk {
namespace net {   

ITimer::ITimer()
    : m_nInterval(0)
    , m_nExpireTime(0)
    , m_bValid(true)
{
}

bool ITimer::OnEvent(size_t ms_now)
{
    if (m_bValid && ms_now > m_nExpireTime) {
        SetValid(false);
        OnTimeout();
    }
}

void ITimer::SetTimeout(size_t ms_interval)
{
    m_nInterval   = ms_interval;
    m_nExpireTime = IReact::GetMilliSecond() + m_nInterval;
    m_bValid      = true;
}

size_t ITimer::GetExpireTime() const
{
    return m_nExpireTime;
}

size_t ITimer::GetInterval() const
{
    return m_nInterval;
}

void ITimer::SetValid(bool valid)
{
    m_bValid = valid;

    if (m_bValid) {
        SetTimeout(m_nInterval);
    }
}

int ITimerComapre::operator()(const void * lhs, const void * rhs)
{
    const ITimer * t1 = (const ITimer *)lhs;
    const ITimer * t2 = (const ITimer *)rhs;

    return t1->GetExpireTime() < t2->GetExpireTime();
}

}   // namespace  
}   // namespace sdk
