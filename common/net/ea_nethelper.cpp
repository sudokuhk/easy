/*************************************************************************                                 
     > File Name: ea_nethelper.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#include <net/ea_nethelper.h>

#include <unistd.h>
#include <fcntl.h>

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

}
}