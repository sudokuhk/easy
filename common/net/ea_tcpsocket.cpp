/*************************************************************************                                 
     > File Name: ea_tcpsocket.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 22, 2016 PM05:34:49 CST
 ************************************************************************/

#include <net/ea_tcpsocket.h> 
#include <log/ea_log.h>
#include <net/ea_nethelper.h>
#include <net/ea_simplebuffer.h>
#include <net/ea_handler.h>

#include <thread/ea_criticalsection.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

namespace sdk {
namespace net {

CTcpSocket::CTcpSocket(IHandler * handler/* = NULL*/)
    : CBaseSocket(TYPE_TCP, handler)
{
    m_pLock         = thread::CCriticalSection::CreateCriticalSection();
    m_pRcvBuffer    = IBuffer::CreateBuffer();
    m_pSendBuffer   = IBuffer::CreateBuffer();
}

CTcpSocket::CTcpSocket(socket_t sfd, IHandler * handler, 
    const char * szip, int port)
    : CBaseSocket(TYPE_TCP, handler)
{
    m_tSocket = sfd;
    strncpy(m_szIp, szip, min(strlen(szip), (size_t)128));
    m_nPort = port;

    m_pLock         = thread::CCriticalSection::CreateCriticalSection();
    m_pRcvBuffer    = IBuffer::CreateBuffer();
    m_pSendBuffer   = IBuffer::CreateBuffer();
}

CTcpSocket::~CTcpSocket()
{
    if (m_tSocket >= 0) {
        close(m_tSocket);
        m_tSocket = -1;
    }
    
    if (m_pLock) {
        delete m_pLock; m_pLock = NULL;
    }
    if (m_pRcvBuffer) {
        delete m_pRcvBuffer; m_pRcvBuffer = NULL;
    }
    if (m_pSendBuffer) {
        delete m_pSendBuffer; m_pSendBuffer = NULL;
    }
}

bool CTcpSocket::Listen(const char * szIp, int port, bool async/* = false*/)
{
    bool ret = false;
    m_tSocket = -1;
    
    if (szIp == NULL || strlen(szIp) == 0 || port == 0) {
        LOG_PRINT(log_error, "invalid ip/port(%d)", port);
        goto failed;
    }
    
    strncpy(m_szIp, szIp, min(strlen(szIp), (size_t)128));
    m_nPort     = port;
    m_bAsync    = async;

    LOG_PRINT(log_debug, "listen [%s:%d]", szIp, port);
    m_tSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_tSocket < 0) {
        LOG_PRINT(log_error, "create socket failed!");
        return false;
    }

    struct sockaddr_in sockaddr;
    sockaddr.sin_family         = AF_INET;
    sockaddr.sin_port           = htons(port);
    sockaddr.sin_addr.s_addr    = inet_addr(szIp);
    int flag, len;
    flag = 1;
    len = sizeof(flag);

    if(setsockopt(m_tSocket, SOL_SOCKET, SO_REUSEADDR, &flag, len) == -1) {
        LOG_PRINT(log_error, "reuse addr socket failed!");
        goto failed;
    }

    if(bind(m_tSocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1) {
        LOG_PRINT(log_error, "bind socket failed!");
        goto failed;
    }

    if(listen(m_tSocket, 0) == -1) { 
        LOG_PRINT(log_error, "listen socket failed!");
        goto failed;
    }

    if (m_bAsync) {
        setNBlock(m_tSocket);
    }

    LOG_PRINT(log_debug, "listen sicceed, fd:%d!", m_tSocket);
    ret       = true;
    m_bListen = true;
    
    return ret;
    
failed:
    if (m_tSocket >= 0) {
        close(m_tSocket);
        m_tSocket = -1;
    }
    LOG_PRINT(log_error, "create listen failed!");
    return ret;
}

bool CTcpSocket::Connect(const char * szIp, int port, bool async/* = false*/)
{
    if (szIp == NULL || strlen(szIp) == 0 || port == 0) {
        LOG_PRINT(log_error, "invalid ip/port(%d)", port);
        return false;
    }

    strncpy(m_szIp, szIp, min(strlen(szIp), (size_t)128));
    m_nPort     = port;
    m_bAsync    = async;
    
    LOG_PRINT(log_debug, "connect to [%s:%d]", szIp, port);

    m_tSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_tSocket < 0) {
        LOG_PRINT(log_error, "create socket failed!");
        return false;
    }

    struct sockaddr_in sockaddr;
    sockaddr.sin_family         = AF_INET;
    sockaddr.sin_port           = htons(port);
    sockaddr.sin_addr.s_addr    = inet_addr(szIp);

    if (async && setNBlock(m_tSocket)) {
        LOG_PRINT(log_error, "[%s:%d]-async, but can't set non-block!", szIp, port);
        Close();
        return false;
    }

    int ret = connect(m_tSocket,(struct sockaddr *)&sockaddr, sizeof(sockaddr));
    if(ret == -1) {
        LOG_PRINT(log_error, "[%s:%d]-async, but can't set non-block!", szIp, port);
        if (async && errno == EINPROGRESS) {
            
        } else {
            Close();
        }
    }
    return true;
}

size_t CTcpSocket::Read(char * buf, size_t bufsize)
{
    if (m_tSocket < 0) {
        LOG_PRINT(log_notice, "socket not connect!!");
        return -1;
    }
    
    ssize_t rdcnt = ::read(m_tSocket, buf, bufsize);
    if (!m_bAsync && rdcnt <= 0) {
        if (rdcnt == 0) {
            LOG_PRINT(log_notice, "peer close socket!")
        } else {
            LOG_PRINT(log_error, "recv data failed! errno:%d, %s!",
                    errno, strerror(errno));
        }
    }

    return rdcnt;
}

bool CTcpSocket::Write(const char * pdata, size_t size)
{    
    if (m_tSocket < 0) {
        LOG_PRINT(log_notice, "socket not connect!!");
        return false;
    }
    
    if (pdata == NULL || size == 0) {
        LOG_PRINT(log_debug, "no data to transfer!");
        return true;
    }
    
    if (m_bAsync) {
        size_t wrisize = ::write(m_tSocket, pdata, size);
        if (wrisize < size) {
            m_pSendBuffer->append(const_cast<char *>(pdata + wrisize), 
                size - wrisize);
        }
        return true;
    } else {
        if (::write(m_tSocket, pdata, size) < 0) {
            LOG_PRINT(log_error, "transfer data failed! errno:%d, %s!",
                errno, strerror(errno));
            return false;
        }
        return true;
    }
}

bool CTcpSocket::Close()
{
    LOG_PRINT(log_debug, "close socket(%d)..!", m_tSocket);
    if (m_tSocket > 0) {
        close(m_tSocket);
        m_tSocket = -1;
    }
}

void CTcpSocket::SetTimeout(int timeo)
{
}

void CTcpSocket::SetHandler(IHandler * handler)
{
    m_pHandler = handler;
}

CBaseSocket * CTcpSocket::Accept()
{
    if (m_bAsync) {
        return NULL;
    }
    
    struct sockaddr_in sockaddr;
    socklen_t len = 0; 
    memset(&sockaddr, 0, sizeof(struct sockaddr_in));
    socket_t cli = accept(m_tSocket, (struct sockaddr *)&sockaddr, &len);

    char buf[16];
    net_ntoa(buf, sockaddr.sin_addr);
    int port = ntohs(sockaddr.sin_port);
    
    LOG_PRINT(log_info, "accept [%s:%d]", buf, port);
    return new CTcpSocket(cli, NULL, buf, port);
}

void CTcpSocket::onRead()
{
    char * ptr = m_pRcvBuffer->tail();
    size_t remain = m_pRcvBuffer->freesize();

    size_t addlen = 10 * 1024;
    char buf[10 * 1024];
    ssize_t rcv_len = read(m_tSocket, buf, addlen);

    if (rcv_len == 0) {
        LOG_PRINT(log_info, "socket reset by peer!");
        Close();

        if (m_pHandler != NULL) {
            m_pHandler->OnClose(this);
        }
        return;
    }

    if (rcv_len > 0) {
        m_pRcvBuffer->append(buf, rcv_len);
    }
    
    if (m_pHandler != NULL) {
        ssize_t dealsize = m_pHandler->OnRead(this, 
            m_pRcvBuffer->data(), m_pRcvBuffer->size());
        
        LOG_PRINT(log_debug, "handle read size:%d!", dealsize);
        if (dealsize >= 0) {
            m_pRcvBuffer->erase(0, dealsize);
        }
    }
}

void CTcpSocket::onWrite()
{
}

void CTcpSocket::onError()
{
}

void CTcpSocket::OnAccept()
{
    int cnt = 10;
    struct sockaddr_in sockaddr;
    
    //accept 10 client socket once time.
    while (cnt > 0) {
        memset(&sockaddr, 0, sizeof(struct sockaddr_in));
        socklen_t len = 0; 
        socket_t cli = accept(m_tSocket, (struct sockaddr *)&sockaddr, &len);

        if (cli < 0) {
            return;
        }
        
        char buf[16];
        net_ntoa(buf, sockaddr.sin_addr);
        int port = ntohs(sockaddr.sin_port);
        
        LOG_PRINT(log_info, "accept [%s:%d]", buf, port);

        if (m_pHandler) {
            m_pHandler->OnAccept(this, new CTcpSocket(cli));
        }
        cnt --;
    }
}


}   // namespace net 
}   // namespace sdk
