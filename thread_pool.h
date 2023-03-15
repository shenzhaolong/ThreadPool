//create by zl
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <functional>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <condition_variable>
#include "thread_assigner.h"


class ThreadPool
{
public:
    //接收构造线程池数目参数，默认为硬件支持并发线程数
    explicit ThreadPool(int num_threads = (int)std::thread::hardware_concurrency());
    ~ThreadPool();


    //添加任务到assigner，并通知有新任务到
    template<typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args)
    {
        return submit(0, func , args...);
    }

    void allStop();

    //添加任务到assigner，并通知有新任务到
    template<typename Func, typename... Args>
    auto submit(int level, Func&& func, Args&&... args)
    {
        // 函数和参数绑定
        auto bindTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);

        // 包装任务以供未来调用
        auto packagedTask = std::make_shared<std::packaged_task<decltype(func(args...))()>>(bindTask);

        // 再次包装函数以提供标准的类型函数给任务队列
        auto putTask = [packagedTask](){ (*packagedTask)();};

        // 由put notify
        assigner.put(putTask);


        return packagedTask->get_future();
    }


private:

    friend class ThreadWorker;//声明友元类
    ThreadAssigner assigner;//任务分配器
    std::vector<std::thread> workers;//存储工作线程
    std::mutex mtx;//互斥锁，保护对assigner访问
    std::condition_variable cv;//条件变量，等待、通知新任务到来
    volatile bool stopped;//线程池本身是否停止
};


#endif // THREAD_POOL_H
