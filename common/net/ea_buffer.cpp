/*************************************************************************                                 
     > File Name: ea_buffer.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#include <net/ea_buffer.h>
#include <net/ea_simplebuffer.h>

namespace sdk {
namespace net {

size_t IBuffer::s_nMinCapacity = IBuffer::enMinCapacity;
IBuffer::createbuffer IBuffer::s_fcreate = NULL;

IBuffer::~IBuffer()
{
}

void IBuffer::setMinCapacity(size_t capacity)
{
    s_nMinCapacity = capacity;
}

IBuffer * IBuffer::CreateBuffer()
{
    if (s_fcreate == NULL) {
        return new CSimpleBuffer();
    } else {
        return s_fcreate();
    }
}

}
}