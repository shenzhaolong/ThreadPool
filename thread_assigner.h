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

    ThreadTasks(std::function<void()> f, int l = 0);
    ThreadTasks(const ThreadTasks& tasks) = default;
    ThreadTasks& operator=(const ThreadTasks& tasks) = default;
    ThreadTasks() = delete;

    // 重载比较符号，优先队列能够比较ThreadTasks
    bool operator<(const ThreadTasks& tasks) const;

private:
    int level;                           // 重要程度
};

// 容器
class ThreadAssigner
{
public:
    ThreadAssigner() = default;
    ThreadAssigner(int ms = 128);

    // 提交任务
    bool put(std::function<void()> func,int level = 0);
    bool put(ThreadTasks&& t);

    // 获取任务，成功返回true，否则返回false
    bool pop(std::function<void()> &t);

    // 判空
    bool empty();
    

private:
   std::priority_queue<ThreadTasks> tasks;
   std::mutex m;
   int maxsize;
};


#endif //THREADPOOL_THREAD_ASSIGNER_H
