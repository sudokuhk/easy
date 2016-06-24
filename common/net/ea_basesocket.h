/*************************************************************************                                 
     > File Name: ea_basesocket.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 21, 2016 AM10:43:49 CST
 ************************************************************************/

#ifndef __EA_BASE_SOCKET_H__
#define __EA_BASE_SOCKET_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <unistd.h>

namespace sdk {
namespace net {    

typedef int socket_t;

class CBaseSocket
{
public:
    enum enSocketType {
        TYPE_TCP = 0,
        TYPE_UDP,
        TYPE_FIFO,
        TYPE_DOMAIN,
    };
    
public:
    CBaseSocket(int socketType);
    virtual ~CBaseSocket();

    virtual bool Listen(const char * szIp, int port) = 0;
    virtual bool Connect(const char * szIp, int port, bool async = false) = 0;
    virtual size_t Read(char * buf, size_t bufsize) = 0;
    virtual bool Write(const char * pdata, size_t size) = 0;
    virtual bool Close() = 0;
    virtual void SetTimeout(int timeo) = 0;
    
    virtual int onRead()    = 0;
    virtual int onWrite()   = 0;
    virtual void onError()  = 0;
    
    virtual socket_t GetSocket();

protected:
    socket_t    m_tSocket;
    int         m_nPort;
    int         m_eSocketType;
    char        m_szIp[128];
    bool        m_bAsync;
};

}   // namespace  
}   // namespace sdk

#endif //__EA_BASE_SOCKET_H__