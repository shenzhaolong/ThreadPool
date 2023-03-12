//
// Created by 77263 on 2023/3/12.
//

#ifndef THREADPOOL_THREAD_ASSIGNER_H
#define THREADPOOL_THREAD_ASSIGNER_H

#include <functional>
#include <queue>
#include <mutex>

// 任务
class ThreadTasks
{
public:
    std::function<void()> func;          // 任务本身

    ThreadTasks(std::function<void()> f, int l);
    ThreadTasks(const ThreadTasks& tasks) = default;
    ThreadTasks& operator=(const ThreadTasks& tasks) = default;
    ThreadTasks() = delete;

    bool operator<(const ThreadTasks& tasks) const;

private:
    int level;                           // 重要程度
};

// 容器
class ThreadAssigner
{
public:
    ThreadAssigner();
    ThreadAssigner(int ms);

    bool put(std::function<void()> func,int level);
    bool put(ThreadTasks&& t);
    bool pop(std::function<void()> &t);
    bool empty();

private:
   std::priority_queue<ThreadTasks> tasks;
   std::mutex m;
   int maxsize;
};


#endif //THREADPOOL_THREAD_ASSIGNER_H
