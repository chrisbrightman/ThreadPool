// workerThread.cpp
// author: christopher Brightman

#include "workerThread.h"
#include <iostream>

namespace tp {

    workerThread::~workerThread() {
        while (!isFinished) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << "workerThread: " << workerId << " exiting." << std::endl;
    }

    void workerThread::setName(unsigned anId) {
        workerId = anId;
    }

    void workerThread::markDone() {
        isFinished = true;
    }

}

