//
// Created by chris on 5/25/20.
//

#ifndef MATRIXOPERATIONS_THREADPOOL_H
#define MATRIXOPERATIONS_THREADPOOL_H


#include <thread>
#include <queue>
#include <functional>
#include <stack>
#include "workQueue.h"
#include "workerThread.h"

#include <iostream>

namespace tp {

    template <class T>
    class threadPool {

        std::stack<std::shared_ptr<workerThread>> threads = std::stack<std::shared_ptr<workerThread>>();

        std::shared_ptr<workQueue<T>> workQ;
        bool isDone;

    public:

        explicit threadPool (unsigned maxThreads = std::thread::hardware_concurrency()) {
            this->workQ = std::shared_ptr<workQueue<T>>(new workQueue<T>());
            isDone = false;
            for (unsigned int i = 0; i < maxThreads; i++) {
                threads.push(std::shared_ptr<workerThread>(new workerThread(std::forward<unsigned >(i), workQ)));
            }
        }

        ~threadPool() {
            waitUntilDone();
        }

        std::shared_ptr<task_s<T>> addWork(std::function<T()> work) {
            return workQ->addWork( work );
        }

        void waitUntilDone() {
            isDone = true;
            try {
                while(!threads.empty()) {
                    std::shared_ptr<workerThread> thread = threads.top();
                    threads.pop();
                    //if (thread->joinable()) {
                        thread->markDone();
                        thread->join();
                    //}
                }
            }
            catch (const std::exception& ex) {
                std::cout << "Thrown by threadPool wait: " << ex.what() << std::endl;
            }
        }

    };


}


#endif //MATRIXOPERATIONS_THREADPOOL_H
