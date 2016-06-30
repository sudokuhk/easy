/*************************************************************************
    > File Name: main.cpp
    > Author: sudoku.huang
    > Mail: sudoku.huang@gmail.com 
    > Created Time: Monday, June 20, 2016 PM09:45:55 CST
 ************************************************************************/

#include <thread/ea_condition.h>
#include <thread/ea_criticalsection.h>
#include <thread/ea_thread.h>
#include <log/ea_log.h>

#include <stdio.h>
#include <pthread.h>

void * fthread(void * arg)
{
    int i = 10;
    while (i --) {
        LOG_PRINT(log_debug, "i:%d!", i);
        sleep(1);
    }
    return NULL;
}

class CThreadTest
{
public:
    static void * ThreadFunc(void * arg);

    void Run()
    {
        int i = 10;
        while (i --) {
            LOG_PRINT(log_debug, "i:%d!", i);
            sleep(1);
        }
    }
};

void * CThreadTest::ThreadFunc(void * arg)
{
    CThreadTest * pObj = (CThreadTest *)arg;
    pObj->Run();

    return NULL;
}

int main(int argc, char * argv[])
{
    LOG_PRINT(log_debug, "begin!");
    sdk::thread::CThread * thread = sdk::thread::CThread::CreateThread(fthread, NULL, "t1");
    thread->Start();
    
    CThreadTest * pThreadTestObj = new CThreadTest();
    sdk::thread::CThread * thread1 = sdk::thread::CThread::CreateThread(
        CThreadTest::ThreadFunc, pThreadTestObj, "t2");
    thread1->Start();

    LOG_PRINT(log_debug, "wait stop!");
    thread->Stop();
    thread1->Stop();
    
    delete thread;
    delete thread1;
    delete pThreadTestObj;
   
    LOG_PRINT(log_debug, "exit!");
    
    return 0;
}