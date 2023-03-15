//
// Created by 77263 on 2023/3/12.
//

#include <iostream>
#include "thread_assigner.h"
#include "thread_pool.h"
#include <random>
#include <time.h>

#define random(x) (rand()%x)


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

std::vector<int> arr(1000);

int assignerTest(int a,int b)
{
    std::vector<int> c(arr);
    for(int i=1;i<1000;i++)
    {
        int j=c[i];
        for(int k = i-1; k >= 0; k--)
        {
            if(c[k]>=j) c[k+1]=c[k];
        }
    }
    return 0;
}


void sortTest()
{
    for(int i=0;i<1000;i++) arr[i]=random(10000);
    clock_t start = clock();
    ThreadPool pool(1);
    for(int i = 0; i < 10000 ;i++)
    {
        pool.submit(assignerTest,1,20000);
    }
    pool.allStop();
    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    std::cout<<elapsed<<std::endl;
}

int main()
{
    std::cout<<"Thread Pool"<<std::endl;
    // assignerTest();
    // threadpoolTest();
    sortTest();

    return 0;
}
