
project(tp)

option(THREAD_POOL_DEBUG "print debug info for the thread pool" NO)

if(THREAD_POOL_DEBUG) 
	add_definitions(-DTHREAD_POOL_DEBUG)
endif()

add_library(tp INTERFACE)
target_include_directories(tp INTERFACE bossThread.h workerThread.cpp workerThread.h workQueue.h threadPool.h)

option(INSTALL_TP YES)

if (INSTALL_TP)
	include(CMakePackageConfigHelpers)
	install(TARGETS tp
		EXPORT TPTargets
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib 
		RUNTIME DESTINATION bin
		INCLUDES DESTINATION include 
		)
endif()

