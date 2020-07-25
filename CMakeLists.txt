# author: Christopher Brightman

cmake_minimum_required(VERSION 3.10)
project(threadPool)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(Threads)

include_directories(threadPool)

# if to build benchmark
option(BUILD_TP_BENCHMARK "build the threadPool benchmark" NO)
# if to build tests
option(BUILD_TP_TESTS "build the threadPool tests" NO)

if (BUILD_TP_BENCHMARK)
add_subdirectory(Benchmark)
endif()

if (BUILD_TP_TESTS)
add_subdirectory(tests)
endif()

# main library
add_subdirectory(threadPool)

option(INSTALL_TP YES)

if (INSTALL_TP)
	install(TARGETS tp
		    EXPORT tp)
endif()
