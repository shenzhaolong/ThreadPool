//
// Created by 77263 on 2023/3/12.
//


#include <utility>
#include <condition_variable>
#include "thread_assigner.h"
#include <iostream>

ThreadTasks::ThreadTasks(std::function<void()> f, int l): func(std::move(f)), level(l){}


bool ThreadTasks::operator<(const ThreadTasks &tasks) const
{
    return this->level < tasks.level;
}

ThreadAssigner::ThreadAssigner(int ms) :maxSize(ms), multiQueue(ms), count(0){}

void ThreadAssigner::put(ThreadTasks& t)
{
    int u = this->count % maxSize;
    this->count++;
    this->multiQueue.puttoid(std::move(t),u);
}


void ThreadAssigner::put(std::function<void()> func,int level)
{
    int u = this->count % maxSize;
    this->count++;
    this->multiQueue.puttoid(ThreadTasks(std::move(func),level),u);
}


bool ThreadAssigner::empty(int id)
{
    return this->multiQueue.emptybyid(id);
}

void ThreadAssigner::stopAll()
{
    for(int i = 0; i < maxSize; i++)
    {
        this->multiQueue.cvs[i]->notify_one();
    }
}

std::pair<std::mutex*,std::condition_variable*> ThreadAssigner::getcvbyid(int id)
{
    return this->multiQueue.getcvbyid(id);
}



bool ThreadAssigner::pop(std::function<void()> &t,int id)
{
    this->multiQueue.getbyid(t,id);
    return true;
}

MultiQueue::MultiQueue(int qs): queueSize(qs)
{
    for(int i = 0; i < qs; i++)
    {
        auto t = new std::mutex();
        this->mutexs.push_back(t);
        auto cv = new std::condition_variable();
        this->cvs.push_back(cv);
    }
    this->queues.resize(qs);
}

MultiQueue::~MultiQueue()
{
    for(int i = 0; i < queueSize; i++)
    {
        delete this->mutexs[i];
        delete this->cvs[i];
    }
}

void MultiQueue::puttoid(ThreadTasks &&tt, int id)
{
    std::lock_guard lgd(*(this->mutexs[id]));
    this->queues[id].push(tt);
    cvs[id]->notify_one();
}

bool MultiQueue::getbyid(std::function<void()> &func, int id)
{
    std::lock_guard lgd(*(this->mutexs[id]));
    if(this->queues[id].size() == 0) return false;
    func = this->queues[id].top().func;
    this->queues[id].pop();
    return true;
}

int MultiQueue::emptybyid(int id)
{
    std::lock_guard lgd(*(this->mutexs[id]));
    return this->queues[id].size() == 0;
}





