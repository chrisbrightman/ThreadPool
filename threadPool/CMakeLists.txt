
project(tp)

option(THREAD_POOL_DEBUG "print debug info for the thread pool" NO)

if(THREAD_POOL_DEBUG) 
	add_definitions(-DTHREAD_POOL_DEBUG)
endif()

add_library(tp STATIC bossThread.h workerThread.h workerThread.cpp workQueue.h threadPool.h tpmain.h)

option(INSTALL_TP YES)

if (INSTALL_TP)
	include(CMakePackageConfigHelpers)
	install(TARGETS tp
		EXPORT TPTargets
		DESTINATION lib
		EXPORT TPTarget
		)
	install(EXPORT TPTarget DESTINATION lib/TP)
endif()

