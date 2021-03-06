/*************************************************************************                                 
     > File Name: ea_tcpsocket.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 21, 2016 PM17:43:49 CST
 ************************************************************************/

#ifndef __EA_TCP_SOCKET_H__
#define __EA_TCP_SOCKET_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <net/ea_basesocket.h>

namespace sdk {

namespace thread {
    class CCriticalSection;
}

namespace net {    

class IBuffer;
class IHandler;

class CTcpSocket
    : public CBaseSocket
{
public:
    CTcpSocket(IHandler * handler = NULL);
    CTcpSocket(socket_t sfd, IHandler * handler = NULL,
        const char * szip = "", int port = 0);
    virtual ~CTcpSocket();

    virtual bool Listen(const char * szIp, int port, bool async = false);
    virtual bool Connect(const char * szIp, int port, bool async = false);
    virtual size_t Read(char * buf, size_t bufsize);
    virtual bool Write(const char * pdata, size_t size);
    virtual bool Close();
    virtual CBaseSocket * Accept();
    
    virtual void onRead();
    virtual void onWrite();
    virtual void onError();
    virtual void OnAccept();

    virtual void SetHandler(IHandler * handler);
    virtual void SetTimeout(int timeo);
    
private:
    IBuffer * m_pRcvBuffer;
    IBuffer * m_pSendBuffer;
    thread::CCriticalSection * m_pLock;
};

}   // namespace  
}   // namespace sdk

#endif //__EA_TCP_SOCKET_H__