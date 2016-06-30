/*************************************************************************                                 
     > File Name: ea_simplebuffer.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, June 23, 2016 PM06:23:49 CST
 ************************************************************************/

#include <net/ea_simplebuffer.h>

#include <string.h>
#include <stdlib.h>

namespace sdk {
namespace net {

CSimpleBuffer::CSimpleBuffer()
    : IBuffer()
    , roff(0)
    , woff(0)
    , capac(s_nMinCapacity)
{
    pbuffer = (char *)malloc(capac);
}

CSimpleBuffer::~CSimpleBuffer()
{
    if (pbuffer != NULL) {
        free(pbuffer);
        pbuffer = NULL;
    }
}

char * CSimpleBuffer::data()
{
    return pbuffer + roff;
}

char * CSimpleBuffer::tail()
{
    return pbuffer + woff;
}

size_t CSimpleBuffer::size()
{
    return woff - roff;
}

void CSimpleBuffer::append(char * data, size_t size)
{
    if (capacity() - woff < size) {
        size_t newcap = capacity() + s_nMinCapacity;
        char * ptr = (char *)realloc(pbuffer, newcap);
        if (ptr == NULL) {
            return;
        }
        capac = newcap;
        pbuffer = ptr;
    }
    memmove(pbuffer + woff, data, size);
    woff += size;
}

void CSimpleBuffer::rewind()
{
    if (roff == 0) {
        return;
    }
    memmove(pbuffer, data(), size());
    woff = woff - roff;
    roff = 0;
}

bool CSimpleBuffer::empty()
{
    return woff == roff;
}

void CSimpleBuffer::erase(size_t offset, size_t length)
{
    if (offset >= capac) {
        return;
    }

    if (offset > woff || offset + length < roff) {
        return;
    }

    if (length > size()) {
        length = size();
    }

    if (offset == roff) {
        roff += length;
    }

    rewind();
}

size_t CSimpleBuffer::capacity()
{
    return capac;
}

size_t CSimpleBuffer::freesize()
{
    return capac - woff;
}


}
}
