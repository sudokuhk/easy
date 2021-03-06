/*************************************************************************                                 
     > File Name: ea_handler.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 30, 2014 PM 05:30:49 CST
 ************************************************************************/

#ifndef __EA_HANDLER_H__
#define __EA_HANDLER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <unistd.h>

namespace sdk {
namespace net {    

class CBaseSocket;

class IHandler
{
public:
    virtual  ~IHandler() {}

    virtual size_t OnRead(CBaseSocket * psocket, const char * pdata, size_t size) = 0;
    virtual size_t OnWrite(CBaseSocket * psocket, const char * pdata, size_t size) = 0;
    virtual void OnClose(CBaseSocket * psocket, bool passive = true) = 0;
    virtual void OnAccept(CBaseSocket * psocket, CBaseSocket * pclient) = 0;
    virtual void onError(CBaseSocket * psocket, int errcode) = 0;
};

}   // namespace  
}   // namespace sdk

#endif //__EA_TIMER_H__