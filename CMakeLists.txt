cmake_minimum_required(VERSION 3.20)
project(untitled25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

add_executable(untitled25 main.cpp)
