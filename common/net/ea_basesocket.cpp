/*************************************************************************                                 
     > File Name: ea_basesocket.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 22, 2016 AM08:46:49 CST
 ************************************************************************/

#include <net/ea_basesocket.h> 

namespace sdk {
namespace net {

CBaseSocket::CBaseSocket(int socketType)
    : m_tSocket(-1)
    , m_nPort(0)
    , m_eSocketType(socketType)
    , m_szIp()
    , m_bAsync(false)
{
}

CBaseSocket::~CBaseSocket() 
{
}

socket_t CBaseSocket::GetSocket()
{
    return m_tSocket;
}
    
}   // namespace log 
}   // namespace sdk
