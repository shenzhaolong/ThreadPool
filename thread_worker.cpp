//
// Created by 77263 on 2023/3/14.
//

#include "thread_worker.h"


ThreadWorker::ThreadWorker(ThreadPool& pool) :pool(pool){}
void ThreadWorker::operator()
(){
    std::function<void()> task;
    while (true) {
        {   
            try{
            std::unique_lock<std::mutex> lock(pool.mtx);
            pool.cv.wait(lock, [this]{ return pool.stopped || !pool.assigner.empty(); });
            if (pool.stopped && pool.assigner.empty()) {
                return;
            }
            pool.assigner.pop(task);
        task();
        }
         catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception" << std::endl;
        }
    }
}
}