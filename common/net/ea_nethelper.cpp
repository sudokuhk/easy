/*************************************************************************                                 
     > File Name: ea_nethelper.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#include <net/ea_nethelper.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
       
namespace sdk {
namespace net {
    
bool setNBlock(socket_t socket)
{
    int fflags = fcntl(socket, F_GETFL);
    if (-1 == fflags) {
        return false;
    }
    fflags |= O_NONBLOCK;
    if (fcntl(socket, F_SETFL, fflags) == -1) {
        return false;
    }
    return true;
}

char * net_ntoa(char * buf, struct in_addr in)
{
    snprintf(buf, 16, "%u.%u.%u.%u",
        (in.s_addr >> 0) & 0xFF,
        (in.s_addr >> 8) & 0xFF, 
        (in.s_addr >> 16) & 0xFF, 
        (in.s_addr >> 24) & 0xFF);

    return buf;
}

}
}