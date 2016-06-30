/*************************************************************************                                 
     > File Name: ea_react.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2014 PM 05:12:49 CST
 ************************************************************************/

#ifndef __EA_REACT_H__
#define __EA_REACT_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <unistd.h>

namespace sdk {
namespace net {    

class ITimer;
class CBaseSocket;

#ifndef REACT_SIZE
#define REACT_SIZE 1024
#endif

class IReact
{
public:
    enum OP_EVENT {
        EA_NONE         = 0,
        EA_READABLE     = (1 << 0),
        EA_WRITABLE     = (1 << 1),
    };
public:
    virtual  ~IReact() {}

    virtual bool AddEvent(CBaseSocket * socket, int mask) = 0;
    virtual bool DelEvent(CBaseSocket * socket, int mask) = 0;

    virtual void AddTimer(ITimer * timer) = 0;
    virtual void DelTimer(ITimer * timer) = 0;

    virtual size_t GetMilliSecond() = 0;

    virtual bool Init() = 0;
    virtual void Run() = 0;
    virtual void Stop() = 0;
};

}   // namespace  
}   // namespace sdk

#endif //__EA_REACT_H__