/*************************************************************************                                 
     > File Name: ea_epoll.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#ifndef __EA_EPOLL_H__
#define __EA_EPOLL_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <net/ea_react.h>
#include <net/ea_buffer.h>
#include <net/ea_basesocket.h>

#include <set>
#include <map>
#include <sys/epoll.h>

namespace sdk {

namespace thread {
    class CCriticalSection;
}

namespace net {    

class CEpoll
    : public IReact
{
public:
    CEpoll(size_t maxfd = REACT_SIZE);
    virtual  ~CEpoll();

    virtual bool AddEvent(CBaseSocket * socket, int mask);
    virtual bool DelEvent(CBaseSocket * socket, int mask);

    virtual void AddTimer(ITimer * timer);
    virtual void DelTimer(ITimer * timer);

    virtual size_t GetMilliSecond();

    virtual bool Init();
    virtual void Run();
    virtual void Stop();

    virtual void Wake();

private:
    void onEvent(CBaseSocket * psocket, int mask);
    
private:
    typedef std::set<ITimer *> TimerContainer;
    typedef std::map<CBaseSocket *, int> SocketContainer;

    TimerContainer  m_cTimers;
    SocketContainer m_cSockets;

    int m_iEpoll;
    struct epoll_event * m_pEvents;

    bool m_bRunning;
    size_t m_nMaxfd;
    socket_t m_tpair[2]; 
    
    thread::CCriticalSection * m_pLock;
};

}   // namespace  
}   // namespace sdk

#endif //__EA_EPOLL_H__