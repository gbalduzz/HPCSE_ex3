cmake_minimum_required(VERSION 3.1)
project(ex3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3")

add_executable(ex3 main.cpp)
add_executable(mytest test.cpp)
