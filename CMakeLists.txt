cmake_minimum_required(VERSION 3.23)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 17)

add_executable(ThreadPool thread_assigner.cpp thread_assigner.h thread_pool.cpp thread_pool.h thread_worker.cpp thread_worker.h main.cpp)
