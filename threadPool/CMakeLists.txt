
project(tp)

option(THREAD_POOL_DEBUG "print debug info for the thread pool" NO)

if(THREAD_POOL_DEBUG) 
	add_definitions(-DTHREAD_POOL_DEBUG)
endif()

set(tp_templates bossThread.h workerThread.h workQueue.h threadPool.h include/tpmain.h)

add_library(tp STATIC workerThread.cpp ${tp_templates})

option(INSTALL_TP YES)

if (INSTALL_TP)
	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
		TPConfigVersion.cmake
		VERSION 1.0 
		COMPATIBILITY AnyNewerVersion
		)
	install(TARGETS tp
		EXPORT TPTargets
		DESTINATION lib
		)
	install(EXPORT TPTargets 
		FILE TPTargets.cmake 
		NAMESPACE TP::
		DESTINATION lib/cmake/TP
	    )
endif()

