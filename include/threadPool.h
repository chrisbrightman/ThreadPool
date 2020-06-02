//
// Created by chris on 5/25/20.
//

#ifndef MATRIXOPERATIONS_THREADPOOL_H
#define MATRIXOPERATIONS_THREADPOOL_H


#include <thread>
#include <queue>
#include <stack>
#include <functional>
#include <atomic>
#include "workQueue.h"

#include <iostream>

namespace tp {

    template <class T>
    class threadPool {
        std::stack<std::shared_ptr<std::thread>> threads;
        workQueue<T> *workQ;
        bool isDone;

    public:

        threadPool(unsigned int maxThreads = std::thread::hardware_concurrency()) {
            this->workQ = new workQueue<T>();
            isDone = false;
            threads = std::stack<std::shared_ptr<std::thread>>();
            for (int i = 0; i < maxThreads; i++) {
                threads.push( std::shared_ptr<std::thread>(new std::thread(&threadPool::operate, this)));
            }
        }

        ~threadPool() {
            isDone = true;
            for (int i = 0; i < threads.size(); i++) {
                std::shared_ptr<std::thread> thread = threads.top();
                if (thread->joinable()) {
                    thread->join();
                }
                threads.pop();
            }
            delete workQ;
        }

        std::shared_ptr<task_s<T>> addWork(std::function<T()> work) {
            return workQ->addWork( work );
        }

        void waitUntilDone() {
            isDone = true;
            for (int i = 0; i < threads.size(); i++) {
                try {
                    std::shared_ptr<std::thread> thread = threads.top();
                    if (thread->joinable()) {
                        thread->join();
                    }
                }
                catch (const std::exception& ex) {
                    std::cout << ex.what();
                }
            }
        }

    private:

        void operate() {
            try {
                while (!isDone || !workQ->isWorkDone()) {
                    if (workQ->isWorkDone()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }
                    task_s<T> *toDo = workQ->dequeueWork();
                    if (toDo) {
                        toDo->returnValue = toDo->function();
                        toDo->isComplete = true;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            } catch (const std::exception& ex) {
                std::cout << ex.what();
                std::cout << "I'm here";

            }
        }

    };


}


#endif //MATRIXOPERATIONS_THREADPOOL_H
