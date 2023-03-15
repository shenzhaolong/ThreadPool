//create by zl

#include <iostream>
#include "thread_pool.h"
#include "thread_worker.h"


ThreadPool::ThreadPool(int num_threads): assigner(num_threads)
{
    stopped = false;
    for (int i = 0; i < num_threads; ++i)
    {
        workers.emplace_back(ThreadWorker(*this, i));
    }
}

void ThreadPool::allStop()
{
    stopped = true;
    this->assigner.stopAll();
    for (auto& worker : workers)
    {
        worker.join();
    }
}


ThreadPool::~ThreadPool()
{

//    stopped = true;
//
//    this->assigner.stopAll();
//    for (auto& worker : workers)
//    {
//        worker.join();
//    }
}








