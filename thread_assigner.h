//
// Created by 77263 on 2023/3/12.
//

#ifndef THREADPOOL_THREAD_ASSIGNER_H
#define THREADPOOL_THREAD_ASSIGNER_H

#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>


// 任务
class ThreadTasks
{
public:
    std::function<void()> func;          // 任务本身

    explicit ThreadTasks(std::function<void()> f, int l = 0);
    ThreadTasks(const ThreadTasks& tasks) = default;
    ThreadTasks& operator=(const ThreadTasks& tasks) = default;
    ThreadTasks() = delete;

    // 重载比较符号，优先队列能够比较ThreadTasks
    bool operator<(const ThreadTasks& tasks) const;
    int level;                           // 重要程度
};


// 维护多个优先队列
class MultiQueue
{

public:
    // 构造函数传入线程数量，每个线程对应一个优先队列
    MultiQueue(int qs);
    ~MultiQueue();

    // 将任务送到指定线程的队列中
    void puttoid(ThreadTasks &&tt, int id);

    // 将指定线程的任务取出来
    bool getbyid(std::function<void()> &func, int id);

    // 判断指定线程队列是否为空
    int emptybyid(int id);

    std::pair<std::mutex*,std::condition_variable*> getcvbyid(int id)
    {
        return {mutexs[id],cvs[id]};
    }
    std::vector<std::condition_variable*> cvs;

private:
    std::vector<std::mutex*> mutexs;
    std::vector<std::priority_queue<ThreadTasks>> queues;
    int queueSize;
};


// 容器
class ThreadAssigner
{
public:
    explicit ThreadAssigner(int ms = 128);

    // 提交任务
    void put(std::function<void()> func,int level = 0);
    void put(ThreadTasks& t);

    // 获取任务并弹出，成功返回true，否则返回false
    bool pop(std::function<void()> &t, int id);

    // 判空
    bool empty(int id);

    std::pair<std::mutex*,std::condition_variable*> getcvbyid(int id);


    void stopAll();
    

private:
    MultiQueue multiQueue;
    volatile int count;
    int maxSize;       // 指定线程的最大数量
};


#endif //THREADPOOL_THREAD_ASSIGNER_H
