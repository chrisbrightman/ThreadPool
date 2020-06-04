//
// Created by chris on 5/25/20.
//

#ifndef MATRIXOPERATIONS_THREADPOOL_H
#define MATRIXOPERATIONS_THREADPOOL_H


#include <thread>
#include <queue>
#include <functional>
#include <vector>
#include <array>
#include "workQueue.h"

#include <iostream>

namespace tp {

    template <class T>
    class threadPool {

        std::vector<std::shared_ptr<std::thread>> threads = std::vector<std::shared_ptr<std::thread>>();
        std::unique_ptr<workQueue<T>> workQ;
        bool isDone;

    public:

        threadPool(unsigned int maxThreads = std::thread::hardware_concurrency()) {
            this->workQ = std::unique_ptr<workQueue<T>>(new workQueue<T>());
            isDone = false;
            for (int i = 0; i < maxThreads; i++) {
                threads.push_back(std::shared_ptr<std::thread>(new std::thread(&threadPool::operate, this, i)));
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
                    std::shared_ptr<std::thread> thread = threads.back();
                    threads.pop_back();
                    if (thread->joinable()) {
                        thread->join();
                    }
                }
            }
            catch (const std::exception& ex) {
                std::cout << "Thrown by threadPool wait: " << ex.what() << std::endl;
            }
        }

    private:

        void operate(int id) {
            try {
                while (!isDone || !workQ->isWorkDone()) {
                    if (workQ->isWorkDone()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }
                    std::shared_ptr<task_s<T>> toDo = workQ->dequeueWork();
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
            threads.erase(threads.begin() + id);
        }
    };


}


#endif //MATRIXOPERATIONS_THREADPOOL_H
