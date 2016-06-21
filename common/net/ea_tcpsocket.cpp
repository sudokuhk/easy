/*************************************************************************                                 
     > File Name: ea_tcpsocket.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 22, 2016 PM05:34:49 CST
 ************************************************************************/

#include <net/ea_tcpsocket.h> 

namespace sdk {
namespace net {

CTcpSocket::CTcpSocket()
    : CBaseSocket(TYPE_TCP)
{
}

CTcpSocket::~CTcpSocket()
{
}

bool CTcpSocket::Listen(const char * szIp, int port)
{
}

bool CTcpSocket::Connect(const char * szIp, int port, bool asnyc/* = false*/)
{
}

bool CTcpSocket::Write(const char * pdata, size_t size)
{
}

bool CTcpSocket::Close()
{
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
