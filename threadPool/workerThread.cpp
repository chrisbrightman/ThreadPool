
#include "workerThread.h"

namespace tp {
	void workerThread::markDone() {
		isFinished = true;
	}
	
	void workerThread::setName(unsigned anId) {
		workerId = anId;
	}

	workerThread::~workerThread() {
	    while (!isFinished) {
		    std::this_thread::sleep_for(std::chrono::milliseconds(10));
	    }
        #ifdef THREAD_POOL_DEBUG
            std::cout << "Worker id " << workerId << " has finished work." << std::endl;
        #endif //THREAD_POOL_DEBUG
	}
}