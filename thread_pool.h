//create by zl
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "thread_assigner.h"

class ThreadPool {
public:
    //接收构造线程池数目参数，默认为硬件支持并发线程数
    explicit ThreadPool(int num_threads = std::thread::hardware_concurrency());
    ~ThreadPool();
    //函数模板，添加任务到线程池
    template<typename Func, typename... Args>
    void enqueue(Func&& func, int level, Args&&... args);

private:
    
    friend class ThreadWorker;//声明友元类
    ThreadAssigner assigner;//任务分配器
    std::vector<std::thread> workers;//存储工作线程
    std::mutex mtx;//互斥锁，保护对assigner访问
    std::condition_variable cv;//条件变量，等待、通知新任务到来
    bool stopped;//线程池本身是否停止
    
};
//添加任务到assigner，并通知有新任务到
template<typename Func, typename... Args>
void ThreadPool::enqueue(Func&& func, int level,Args&&... args) {
    std::unique_lock<std::mutex> lock(mtx);
    assigner.put(std::bind(std::forward<Func>(func), std::forward<Args>(args)...),level);
    cv.notify_one();
}

#endif // THREAD_POOL_H
