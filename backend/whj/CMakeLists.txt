cmake_minimum_required(VERSION 3.12)
project(backend_dhc)

set(CMAKE_CXX_STANDARD 17)

add_executable(backend_whj bpcheck.cpp ../dhc/IOManager.hpp ../dhc/map.hpp ../dhc/utility.hpp ../dhc/exceptions.hpp bpt_new.hpp)
