//
// Created by 77263 on 2023/3/14.
//

#include "thread_worker.h"
#include <iostream>


ThreadWorker::ThreadWorker(ThreadPool& pool, int id) :pool(pool), id(id){}

void ThreadWorker::operator()()
{
    std::function<void()> task;
    while (true)
    {
        try
        {
            // std::cout<<"id :"<<id<<" is run"<<std::endl;
            auto t = pool.assigner.getcvbyid(id);
            while(!pool.stopped && pool.assigner.empty(id))
            {
                {
                    std::unique_lock<std::mutex> lock(*t.first);
                    (*t.second).wait(lock);
                }
            }
            if (pool.stopped && pool.assigner.empty(id))
            {
                return;
            }
            pool.assigner.pop(task,id);
            task();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown exception" << std::endl;
        }

    }
}