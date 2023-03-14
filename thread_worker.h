//
// Created by 77263 on 2023/3/14.
//

#ifndef THREADPOOL_THREAD_WORKER_H
#define THREADPOOL_THREAD_WORKER_H
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include "thread_pool.h"
class ThreadWorker
{
public:
    //接受引用作为参数
    ThreadWorker(ThreadPool&pool);
    //重载运算符，线程中执行任务
    void operator()();
    
private:
    // ThreadPool* threadPool;
    int id;
    ThreadPool& pool;
};


#endif //THREADPOOL_THREAD_WORKER_H
