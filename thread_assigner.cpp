//
// Created by 77263 on 2023/3/12.
//

#include "thread_assigner.h"

ThreadTasks::ThreadTasks(std::function<void()> f, int l): func(f), level(l){}


bool ThreadTasks::operator<(const ThreadTasks &tasks) const
{
    return this->level < tasks.level;
}

ThreadAssigner::ThreadAssigner(int ms) :maxsize(ms) {}

bool ThreadAssigner::put(ThreadTasks&& t)
{
    std::lock_guard<std::mutex> lgd(m);
    if (tasks.size() == maxsize) return false;
    this->tasks.push(t);
}


bool ThreadAssigner::put(std::function<void()> func,int level)
{
    return put(ThreadTasks{func,level});
}


bool ThreadAssigner::empty()
{
    std::lock_guard<std::mutex> lgd(m);
    return this->tasks.size() == 0;
}


bool ThreadAssigner::pop(std::function<void()> &t)
{
    std::lock_guard<std::mutex> lgd(m);
    if (this->tasks.size() == 0) return false;
    t = this->tasks.top().func;
    return true;
}



