//
// Created by 77263 on 2023/3/12.
//

#include <iostream>
#include "thread_assigner.h"
#include "thread_pool.h"
#include <random>

int assignerTest(int a,int b)
{
    return a+b;
}

std::mutex cout_mutex;

void task(int id)
{
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Task " << id << " started" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Task " << id << " finished" << std::endl;
    }
}

void threadpoolTest()
{   std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<
    >dis(1,10);
    int level =dis(gen);
    ThreadPool pool(4);
    for (int i = 1; i <= 10; i++)
    {
        pool.submit([i](){ task(i); });
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    

}


int main()
{
    std::cout<<"Thread Pool"<<std::endl;
    // assignerTest();
    // threadpoolTest();
    ThreadPool pool(4);
    auto k = pool.submit(assignerTest,1,2);
    std::cout<<k.get() <<std::endl;
    return 0;
}
