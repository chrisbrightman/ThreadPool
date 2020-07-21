
add_library(tp INTERFACE)
target_include_directories(tp INTERFACE bossThread.h workerThread.cpp workerThread.h workQueue.h)


