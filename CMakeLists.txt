cmake_minimum_required(VERSION 3.1)
project(ex3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_definitions(-D_GLIBCXX_PARALLEL)
add_executable(ex3 main.cpp)
add_executable(mytest test.cpp)
