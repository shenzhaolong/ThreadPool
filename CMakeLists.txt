cmake_minimum_required(VERSION 3.23)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 14)

add_executable(ThreadPool thread_assigner.cpp thread_assigner.h main.cpp)
