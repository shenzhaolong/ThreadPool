//
// Created by 77263 on 2023/3/12.
//

#include <iostream>
#include "thread_assigner.h"


void assignerTest()
{
    std::function<void()> func = []()->void{ std::cout<<"func..."<<std::endl;};
    ThreadTasks t(func);
    t.func();
}


int main()
{
    std::cout<<"Thread Pool"<<std::endl;
    assignerTest();
    return 0;
}
