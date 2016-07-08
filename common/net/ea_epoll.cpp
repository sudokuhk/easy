/*************************************************************************                                 
     > File Name: ea_epoll.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#include <net/ea_epoll.h>

#include <net/ea_nethelper.h>
#include <net/ea_timer.h>
#include <log/ea_log.h>
#include <thread/ea_criticalsection.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

namespace sdk {
namespace net {

CEpoll::CEpoll(size_t maxfd)
    : IReact()
    , m_cTimers()
    , m_cSockets()
    , m_iEpoll(-1)
    , m_pEvents(NULL)
    , m_bRunning(false)
    , m_nMaxfd(maxfd)
{
    m_tpair[0]  = m_tpair[1] = -1;
    m_pLock     = thread::CCriticalSection::CreateCriticalSection();
}

CEpoll::~CEpoll()
{
    if (m_pEvents) {
        free(m_pEvents);
        m_pEvents = NULL;
    }

    if (m_tpair[0] > 0) {
        close(m_tpair[0]);
        close(m_tpair[1]);
        m_tpair[0] = m_tpair[1] = -1;
    }

    if (m_pLock) {
        delete m_pLock;
        m_pLock = NULL;
    }
}

bool CEpoll::AddEvent(CBaseSocket * socket, int mask)
{
    LOG_PRINT(log_debug, "add event socket:%p, mask:%d", socket, mask);
    
    if (socket == NULL) {
        LOG_PRINT(log_info, "NULL socket");
        return false;
    }
    struct epoll_event ev;
    int op = 0, oldmask = EA_NONE;

    m_pLock->Enter();
    SocketContainer::iterator it = m_cSockets.find(socket);
    if (it != m_cSockets.end()) {
        oldmask = it->second;
        it->second |= mask;
    } else {
        m_cSockets[socket] = mask;
    }
    m_pLock->Leave();

    mask |= oldmask;
    op = oldmask == EA_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    ev.events   = 0;
    ev.data.ptr = socket;
    
    if (mask & EA_READABLE) ev.events |= EPOLLIN;
    if (mask & EA_WRITABLE) ev.events |= EPOLLOUT;

    if (epoll_ctl(m_iEpoll, op, socket->GetSocket(), &ev) == -1) {
        LOG_PRINT(log_error, "epoll_ctl failed, socket:%d, error(%d:%s)", 
            ev.data.fd, errno, strerror(errno));
        return false;
    }
    
    return true;
}

bool CEpoll::DelEvent(CBaseSocket * socket, int mask)
{
    if (socket == NULL) {
        LOG_PRINT(log_info, "NULL socket");
        return false;
    }

    struct epoll_event ev;
    int op = 0, oldmask = EA_NONE;

    m_pLock->Enter();
    SocketContainer::iterator it = m_cSockets.find(socket);
    if (it != m_cSockets.end()) {
        oldmask = it->second;
        it->second &= ~mask;
    } 
    m_pLock->Leave();

    op = (mask == EA_NONE) ? EPOLL_CTL_DEL : EPOLL_CTL_MOD;
    ev.events   = 0;
    ev.data.ptr = socket;
    
    if (mask & EA_READABLE) ev.events |= EPOLLIN;
    if (mask & EA_WRITABLE) ev.events |= EPOLLOUT;

    if (epoll_ctl(m_iEpoll, op, socket->GetSocket(), &ev) == -1) {
        LOG_PRINT(log_error, "epoll_ctl failed, socket:%d", ev.data.fd);
        return false;
    }
    return true;
}

void CEpoll::AddTimer(ITimer * timer)
{
    if (timer == NULL) {
        return;
    }
    LOG_PRINT(log_debug, "add timer:%p, timeout:%d", timer, timer->GetInterval());
    
    m_pLock->Enter();
    TimerContainer::const_iterator it = m_cTimers.find(timer);
    if (it == m_cTimers.end()) {
        m_cTimers.insert(timer);
    }
    m_pLock->Leave();

    Wake();
}

void CEpoll::DelTimer(ITimer * timer)
{
    if (timer == NULL) {
        return;
    }

    m_pLock->Enter();
    TimerContainer::const_iterator it = m_cTimers.find(timer);
    if (it != m_cTimers.end()) {
        m_cTimers.erase(timer);
    }
    m_pLock->Leave();
}

bool CEpoll::Init()
{
    bool ret = false;
    m_bRunning = false;
    
    if (m_iEpoll > 0) {
        LOG_PRINT(log_info, "epoll fd %d, close first!", m_iEpoll);
        free(m_pEvents);
    }

    m_pEvents = (struct epoll_event *)malloc(sizeof(epoll_event) * m_nMaxfd);
    if (m_pEvents == NULL) {
        LOG_PRINT(log_error, "malloc %d events failed.", m_nMaxfd);
        goto failed;
    }

    if(socketpair(AF_UNIX, SOCK_STREAM, 0, m_tpair) == -1) {
        LOG_PRINT(log_error, "socketpair failed!(%d:%s)", 
            errno, strerror(errno));
        goto failed;
    }

    m_iEpoll = epoll_create(m_nMaxfd);
    if (m_iEpoll < 0) {
        LOG_PRINT(log_error, "epoll create failed!(%d:%s)", 
            errno, strerror(errno));
        goto failed;
    }

    struct epoll_event ev;
    ev.events   = EPOLLIN;
    ev.data.u64 = 0;
    ev.data.fd  = m_tpair[1];

    setNBlock(m_tpair[1]);
    
    if (epoll_ctl(m_iEpoll, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1) {
        LOG_PRINT(log_error, "epoll_ctl failed, pair:%d, error(%d:%s)", 
            ev.data.fd, errno, strerror(errno));
        goto failed;
    }
    
    ret = true;
    LOG_PRINT(log_debug, "epoll create (%d) success!", m_iEpoll);
    
    return ret;
failed:
    if (m_iEpoll > 0) {
        close(m_iEpoll);
        m_iEpoll = -1;
    }
    
    if (m_tpair[0] > 0) {
        close(m_tpair[0]);
        close(m_tpair[1]);
        m_tpair[0] = m_tpair[1] = -1;
    }
    
    if (m_pEvents) {
        free(m_pEvents);
        m_pEvents = NULL;
    }
    return ret;
}

void CEpoll::Run()
{
    LOG_PRINT(log_debug, "epoll run[%d]!", m_iEpoll);
    if (m_iEpoll < 0) {
        LOG_PRINT(log_error, "epoll run failed, invalid epoll fd(%d)!", m_iEpoll);
        return;
    }
    
    m_bRunning = true;

    while (m_bRunning) {
        int retval, numevents = 0;

        int timeout = -1;
        size_t minval = 0;
        size_t now = GetMilliSecond();
        
        m_pLock->Enter();
        if (m_cTimers.size() > 0) {
            TimerContainer::const_iterator it = m_cTimers.begin();
            LOG_PRINT(log_debug, "get min timer, expire:%d, timeval:%d!", 
                (*it)->GetExpireTime(), (*it)->GetInterval());
            minval = (*it)->GetExpireTime();
            if (minval > now) {
                timeout = minval - now;
            } else {
                timeout = 0;
            }
            ++ it;
        }
        m_pLock->Leave();
        
        LOG_PRINT(log_debug, "epoll_wait now:%d timeout:%d!", now, timeout);
            
        retval = epoll_wait(m_iEpoll, m_pEvents, m_nMaxfd, timeout);
        LOG_PRINT(log_debug, "epoll_wait return :%d!", retval);

        if (retval > 0) {
            int j;

            numevents = retval;
            for (j = 0; j < numevents; j++) {
                int mask = 0;
                struct epoll_event *e = m_pEvents + j;

                if (e->events & EPOLLIN)  mask |= EA_READABLE;
                if (e->events & EPOLLOUT) mask |= EA_WRITABLE;
                if (e->events & EPOLLERR) mask |= EA_WRITABLE;
                if (e->events & EPOLLHUP) mask |= EA_WRITABLE;

                if (e->data.fd == m_tpair[1]) {
                    LOG_PRINT(log_debug, "wake up epoll!");
                    char buf[1024];
                    int rn = ::read(m_tpair[1], buf, 1024);
                    buf[rn] = '\0';
                    LOG_PRINT(log_debug, "wake up times:%d:%s", rn, buf);
                    continue;
                }

                CBaseSocket * psocket = (CBaseSocket *)e->data.ptr;
                OnEvent(psocket, mask);
            }
        }

        OnTimeout();
    }
    LOG_PRINT(log_debug, "epoll run end!");
}

void CEpoll::Stop()
{
    LOG_PRINT(log_debug, "epoll stop!");
    Wake();
    m_bRunning = false;
}

void CEpoll::Wake()
{
    m_pLock->Enter();
    char buf = 'w';
    ::write(m_tpair[0], (char *)&buf, 1);
    m_pLock->Leave();
}

void CEpoll::OnEvent(CBaseSocket * psocket, int mask)
{   
    LOG_PRINT(log_debug, "event on socket:%p, mask:%d", psocket, mask);
    
    if (psocket == NULL) {
        LOG_PRINT(log_info, "epoll event, null socket event!!");
        return;
    }

    m_pLock->Enter();
    SocketContainer::iterator it = m_cSockets.find(psocket);
    if (it == m_cSockets.end()) {
        LOG_PRINT(log_info, "epoll on event, socket(%p) not in container!", psocket);
        return;
    } 
    m_pLock->Leave();
    
    if (mask & EA_READABLE) {
        if (psocket->IsListen()) {
            psocket->OnAccept();
        } else {
            psocket->onRead();
        }
    }

    if (mask & EA_WRITABLE) {
        psocket->onWrite();
    }
}

void CEpoll::OnTimeout()
{
    size_t now = GetMilliSecond();

    TimerContainer tmpC;
    
    m_pLock->Enter();
    tmpC.swap(m_cTimers);
    
    while (tmpC.size() > 0) {
        TimerContainer::iterator it = tmpC.begin();
        if ((*it)->GetExpireTime() > now) {
            break;
        }
        tmpC.erase(it);
        (*it)->OnEvent(now);
    }

    if (tmpC.size() > 0) {
        m_cTimers.insert(tmpC.begin(), tmpC.end());
    }
    m_pLock->Leave();

}

}
}
