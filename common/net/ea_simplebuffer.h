/*************************************************************************                                 
     > File Name: ea_simplebuffer.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#ifndef __EA_SIMPLE_BUFFER_H__
#define __EA_SIMPLE_BUFFER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <net/ea_buffer.h>

namespace sdk {
namespace net {    

class CSimpleBuffer
    : public IBuffer
{
public:
    CSimpleBuffer();
    virtual ~CSimpleBuffer();

    virtual char * data();
    virtual char * tail();
    virtual size_t size();
    virtual void append(char * data, size_t size);
    virtual void rewind();
    virtual bool empty();
    virtual void erase(size_t offset, size_t size);
    virtual size_t capacity();
    virtual size_t freesize();

private:
    char * pbuffer;
    size_t roff;
    size_t woff;
    size_t capac;
};

}   // namespace  
}   // namespace sdk

#endif //__EA_NET_HELPER_H__
