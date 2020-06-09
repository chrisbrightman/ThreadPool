// author: christopher Brightman

#include "bossThread.h"

namespace tp {

	void bossThread::stopWork() {
		for (int i = 0; i < numberWorkers; i++) {
			try {
				std::shared_ptr<workerThread> worker = workers.top();
				workers.pop();
				worker->markDone();
				worker->join();
			}
			catch (std::exception ex) {
				std::cout << "Exception thrown by joining woker thread " << i << " " << ex.what() << std::endl;
			}
		}
	}


}