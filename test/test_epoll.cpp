/*************************************************************************
    > File Name: main.cpp
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM09:45:55 CST
 ************************************************************************/

#include <net/ea_epoll.h>
#include <thread/ea_thread.h>
#include <net/ea_tcpsocket.h>
#include <net/ea_handler.h>
#include <net/ea_timer.h>

#include <log/ea_log.h>
#include <string.h>
#include <stdio.h>

sdk::net::IReact * p1;
sdk::net::IReact * p2;

sdk::net::ITimer * t1000;
sdk::net::ITimer * t5000;
sdk::net::ITimer * t10000;

class CTimer :
    public sdk::net::ITimer
{
public:
    CTimer(int timeout) : sdk::net::ITimer()
    {
        SetTimeout(timeout);
    }

    virtual ~CTimer() {}

    virtual void OnTimeout()
    {
        LOG_PRINT(log_debug, "timeout(%d)....", GetInterval());
        SetValid(true);
        p1->AddTimer(this);
    }
};

class CSocketHandler :
    public sdk::net::IHandler
{
    virtual  ~CSocketHandler() {}

    virtual size_t OnRead(sdk::net::CBaseSocket * psocket, 
        const char * pdata, size_t size)
    {
        LOG_PRINT(log_debug, "read data:(%d)-%s", size, pdata);
        if (size >= 4 && strncmp("quit", pdata, 4) == 0) {
            p1->DelEvent(psocket, sdk::net::IReact::EA_READABLE);
            psocket->Close();
            delete psocket;
            return -1;
        }
        
        psocket->Write(pdata, size);
        return size;
    }
    
    virtual size_t OnWrite(sdk::net::CBaseSocket * psocket, 
        const char * pdata, size_t size)
    {
    }

    virtual void OnClose(sdk::net::CBaseSocket * psocket, bool passive)
    {   
        LOG_PRINT(log_debug, "close socket, passive:%d", passive);
    }
    
    virtual void OnAccept(sdk::net::CBaseSocket * psocket, 
        sdk::net::CBaseSocket * pclient)
    {
        pclient->SetHandler(this);
        p1->AddEvent(pclient, sdk::net::IReact::EA_READABLE);
    }
    
    virtual void onError(sdk::net::CBaseSocket * psocket, int errcode)
    {
    }
};
    
void * fthread(void * arg)
{
    LOG_PRINT(log_debug, "begin!");
    sdk::net::IReact * pReact = (sdk::net::IReact *)arg;
    
    pReact->Run();
    LOG_PRINT(log_debug, "end!");
    
    return NULL;
}

int main(int argc, char * argv[])
{
    LOG_PRINT(log_debug, "begin!");
        
    p1 = new sdk::net::CEpoll();
    //p2 = new sdk::net::CEpoll();

    p1->Init();

    sdk::thread::CThread * thread1 = sdk::thread::CThread::CreateThread(
        fthread, p1, "epoll1");
    thread1->Start();
    
    CSocketHandler * handler = new CSocketHandler();

    sdk::net::CBaseSocket * socket = new sdk::net::CTcpSocket(handler);
    socket->Listen("127.0.0.1", 12345, true);
    p1->AddEvent(socket, sdk::net::IReact::EA_READABLE);

    t1000 = new CTimer(1000);
    t5000 = new CTimer(5000);
    t10000 = new CTimer(10000);
    p1->AddTimer(t1000);
    p1->AddTimer(t5000);
    p1->AddTimer(t10000);
    
    int n;
    while (scanf("%d", &n) != -1);
    
    LOG_PRINT(log_debug, "wait stop!");
    p1->Stop();
    
    thread1->Stop();
    
    LOG_PRINT(log_debug, "exit!");
    
    return 0;
}