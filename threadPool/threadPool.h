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
#include "bossThread.h"

#include <iostream>

namespace tp {

    template <class T>
    class threadPool {

        std::stack<std::shared_ptr<bossThread<T>>> threads = std::stack<std::shared_ptr<bossThread<T>>>();

        std::shared_ptr<workQueue<T>> workQ;
        bool isDone;

    public:

        explicit threadPool (unsigned maxThreads = std::thread::hardware_concurrency()) {
            this->workQ = std::shared_ptr<workQueue<T>>(new workQueue<T>());
            isDone = false;
            threads.push(std::shared_ptr<bossThread<T>>(new bossThread<T>(maxThreads, workQ)));
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
                    std::shared_ptr<bossThread<T>> thread = threads.top();
                    threads.pop();
                    //if (thread->joinable()) {
                        thread->stopWork();
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
