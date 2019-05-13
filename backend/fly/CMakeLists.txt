cmake_minimum_required(VERSION 3.12)
project(fly)

set(CMAKE_CXX_STANDARD 14)

aux_source_directory(. FLYSRC)
aux_source_directory(../whj/ WHJSRC)
aux_source_directory(../dhc/ DHCSRC)
add_executable(fly ${FLYSRC} ${WHJSRC} ${DHCSRC})
