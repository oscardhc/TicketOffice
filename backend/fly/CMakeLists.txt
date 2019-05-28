cmake_minimum_required(VERSION 3.10)
project(fly)

add_compile_options(-O2)
set(CMAKE_CXX_STANDARD 11)

add_executable(fly test.cpp)