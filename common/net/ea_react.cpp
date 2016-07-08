/*************************************************************************                                 
     > File Name: ea_react.cpp
     > Author: sudoku.huang
     > Mail: sudoku.huang@gmail.com 
     > Created Time: Tuesday, July 7, 2016 PM 10:24:49 CST
 ************************************************************************/

#include "ea_react.h"

#include <time.h>

namespace sdk {
namespace net {    

size_t IReact::GetMilliSecond()
{
    struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

}
}