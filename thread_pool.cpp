//create by zl

#include "thread_pool.h"
#include "thread_worker.h"
#include <iostream>
ThreadPool::ThreadPool(int num_threads)
    : assigner(128), stopped(false) {
        
    for (int i = 0; i < num_threads; ++i) {
        workers.emplace_back(ThreadWorker(*this));
        std::cout <<"success"<< i << std::endl;
    }
    
}

ThreadPool::~ThreadPool() {
    
    std::cout<<"Stop is called, ThreadPool will end."<<std::endl;
    {
        std::unique_lock<std::mutex> lock(mtx);
        stopped = true;
    }
    
    cv.notify_all();
    for (auto& worker : workers) {
        worker.join();
    }
}





