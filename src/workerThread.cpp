// workerThread.cpp
// author: christopher Brightman

#include "workerThread.h"
#include <iostream>

namespace tp {

    template<class T>
    workerThread<T>::~workerThread() {
        while (!isFinished) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        #ifdef THREAD_POOL_DEBUG
        std::cout << "workerThread: " << workerId << " exiting." << std::endl;
        #endif //  THREAD_POOL_DEBUG
    }

    template<class T>
    inline void workerThread<T>::setName(unsigned anId) {
        workerId = anId;
    }

    template<class T>
    inline void workerThread<T>::markDone() {
        isFinished = true;
    }

}

