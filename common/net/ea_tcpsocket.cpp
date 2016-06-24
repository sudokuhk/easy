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

#include <thread/ea_criticalsection.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

namespace sdk {
namespace net {

CTcpSocket::CTcpSocket()
    : CBaseSocket(TYPE_TCP)
{
    m_pLock         = thread::CCriticalSection::CreateCriticalSection();
    m_pRcvBuffer    = IBuffer::CreateBuffer();
    m_pSendBuffer   = IBuffer::CreateBuffer();
}

CTcpSocket::~CTcpSocket()
{
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

bool CTcpSocket::Listen(const char * szIp, int port)
{
    if (szIp == NULL || strlen(szIp) == 0 || port == 0) {
        LOG_PRINT(log_error, "invalid ip/port(%d)", port);
        return false;
    }

    
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
        m_pSendBuffer->append(const_cast<char *>(pdata), size);
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
    if (m_tSocket > 0) {
        close(m_tSocket);
        m_tSocket = -1;
    }
}

void CTcpSocket::SetTimeout(int timeo)
{
}

int CTcpSocket::onRead()
{
}

int CTcpSocket::onWrite()
{
}

void CTcpSocket::onError()
{
}

}   // namespace net 
}   // namespace sdk
