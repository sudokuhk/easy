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
    
    sdk::net::CBaseSocket * socket = new sdk::net::CTcpSocket();
    socket->Listen("127.0.0.1", 12345);
    
    sdk::net::CBaseSocket * cli = socket->Accept();
    char buf[1024];
    size_t rcvcnt = 0;
    while ((rcvcnt = cli->Read(buf, 1024)) > 0) {
        rcvcnt -= 2;
        if (rcvcnt <= 0) {
            continue;
        }
        buf[rcvcnt] = '\0';
        LOG_PRINT(log_debug, "read data(%d):%s", rcvcnt, buf);
        if (strcmp("quit", buf) == 0) {
            break;
        }
        cli->Write(buf, rcvcnt);
    }
    cli->Close();
    delete cli;

    socket->Close();
    delete socket;
    
    LOG_PRINT(log_debug, "exit!");
    
    return 0;
}