/*************************************************************************                                 
     > File Name: ea_buffer.h
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#ifndef __EA_BUFFER_H__
#define __EA_BUFFER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <unistd.h>

namespace sdk {
namespace net {    

class IBuffer
{
public:
    enum {
        enMinCapacity = 64 * 1024, //64k
    };

    typedef IBuffer * (*createbuffer)();

public:
    virtual ~IBuffer();
    
    virtual char * data() = 0;
    virtual char * tail() = 0;
    virtual size_t size() = 0;
    virtual void append(char * data, size_t size) = 0;
    virtual void rewind() = 0;
    virtual bool empty() = 0;
    virtual void erase(size_t offset, size_t size) = 0;
    virtual size_t capacity() = 0;
    virtual size_t freesize() = 0;

    static void setMinCapacity(size_t capacity);
    static IBuffer * CreateBuffer();
protected:
    static size_t s_nMinCapacity;
    static createbuffer s_fcreate;
};

}   // namespace  
}   // namespace sdk

#endif //__EA_NET_HELPER_H__
