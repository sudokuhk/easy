/*************************************************************************
    > File Name: main.cpp
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM09:45:55 CST
 ************************************************************************/

#include <net/ea_tcpsocket.h>
#include <net/ea_simplebuffer.h>

#include <log/ea_log.h>
#include <string.h>

int main(int argc, char * argv[])
{
    LOG_PRINT(log_debug, "begin!");
    sdk::net::IBuffer::setMinCapacity(4);
    
    sdk::net::IBuffer * buffer = new sdk::net::CSimpleBuffer();
    LOG_PRINT(log_debug, "buffer length:%d, capacity:%d", 
        buffer->size(), buffer->capacity());

    buffer->append("abcd", 4);
    LOG_PRINT(log_debug, "buffer length:%d, capacity:%d, content:%s", 
        buffer->size(), buffer->capacity(), buffer->data());

    buffer->erase(0, 2);
    LOG_PRINT(log_debug, "buffer length:%d, capacity:%d, content:%s", 
        buffer->size(), buffer->capacity(), buffer->data());

    buffer->append("efg", 3);
    LOG_PRINT(log_debug, "buffer length:%d, capacity:%d, content:%s", 
        buffer->size(), buffer->capacity(), buffer->data());
    return 0;
    
    sdk::net::CBaseSocket * socket = new sdk::net::CTcpSocket();
    socket->Connect("127.0.0.1", 12345);

    const char * buf = "hello, world!";
    socket->Write(buf, strlen(buf));

    char rcv[1024];
    socket->Read(rcv, 1024);
    LOG_PRINT(log_debug, "read data:%s", rcv);

    socket->Close();
    delete socket;
    
    LOG_PRINT(log_debug, "exit!");
    
    return 0;
}