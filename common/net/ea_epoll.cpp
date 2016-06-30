/*************************************************************************                                 
     > File Name: ea_epoll.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#include <net/ea_epoll.h>

#include <log/ea_log.h>

#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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
}

CEpoll::~CEpoll()
{
    if (m_pEvents) {
        free(m_pEvents);
        m_pEvents = NULL;
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

    SocketContainer::iterator it = m_cSockets.find(socket);
    if (it != m_cSockets.end()) {
        oldmask = it->second;
        it->second |= mask;
    } else {
        m_cSockets[socket] = mask;
    }

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

    SocketContainer::iterator it = m_cSockets.find(socket);
    if (it != m_cSockets.end()) {
        oldmask = it->second;
        it->second &= ~mask;
    } 

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
}

void CEpoll::DelTimer(ITimer * timer)
{
}

size_t CEpoll::GetMilliSecond()
{
    struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
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

    m_iEpoll = epoll_create(m_nMaxfd);
    if (m_iEpoll < 0) {
        LOG_PRINT(log_error, "epoll create failed!(%d:%s)", 
            errno, strerror(errno));
        return false;
    }
    ret = true;
    LOG_PRINT(log_debug, "epoll create (%d) success!", m_iEpoll);
    
    return ret;
failed:
    if (m_pEvents) {
        free(m_pEvents);
        m_pEvents = NULL;
    }
    return ret;
}

void CEpoll::Run()
{
    LOG_PRINT(log_debug, "epoll run[%d]!", m_iEpoll);
    m_bRunning = true;

    while (m_bRunning) {
        int retval, numevents = 0;
            
        retval = epoll_wait(m_iEpoll, m_pEvents, m_nMaxfd, 1000);
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

                CBaseSocket * psocket = (CBaseSocket *)e->data.ptr;
                onEvent(psocket, mask);
            }
        }
    }
    LOG_PRINT(log_debug, "epoll run end!");
}

void CEpoll::Stop()
{
    LOG_PRINT(log_debug, "epoll stop!");
    m_bRunning = false;
}

void CEpoll::onEvent(CBaseSocket * psocket, int mask)
{   
    LOG_PRINT(log_debug, "event on socket:%p, mask:%d", psocket, mask);
    
    if (psocket == NULL) {
        LOG_PRINT(log_info, "epoll event, null socket event!!");
        return;
    }
    
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

}
}
