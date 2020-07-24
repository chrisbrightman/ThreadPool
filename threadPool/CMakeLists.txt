
option(THREAD_POOL_DEBUG "print debug info for the thread pool" NO)

if(THREAD_POOL_DEBUG) 
	add_definitions(-DTHREAD_POOL_DEBUG)
endif()

add_library(tp INTERFACE)
target_include_directories(tp INTERFACE bossThread.h workerThread.cpp workerThread.h workQueue.h threadPool.h)


