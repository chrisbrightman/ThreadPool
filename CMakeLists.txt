cmake_minimum_required(VERSION 3.10)
project(threadPool)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)


add_subdirectory(threadPool)

find_package(Threads)

include_directories(threadPool)
add_subdirectory(Benchmark)

add_subdirectory(test)
