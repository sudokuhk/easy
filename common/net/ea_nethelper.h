/*************************************************************************                                 
     > File Name: ea_nethelper.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#ifndef __EA_NET_HELPER_H__
#define __EA_NET_HELPER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <net/ea_basesocket.h>

namespace sdk {
namespace net {    

bool setNBlock(socket_t socket);

template<typename T>
T min(T lhs, T rhs)
{
    return lhs > rhs ? rhs : lhs;
}

template<typename T>
T max(T lhs, T rhs)
{
    return lhs < rhs ? rhs : lhs;
}

}   // namespace  
}   // namespace sdk

#endif //__EA_NET_HELPER_H__