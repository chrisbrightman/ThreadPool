//
// Created by chris on 5/25/20.
//

#ifndef MATRIXOPERATIONS_THREADPOOL_H
#define MATRIXOPERATIONS_THREADPOOL_H


#include <thread>
#include <queue>
#include <functional>
#include <stack>
#include <unordered_map>
#include <iostream>

#include "workQueue.h"
#include "workerThread.h"
#include "bossThread.h"


namespace tp {

    template<class T>
    class threadPool {

        std::shared_ptr<workQueue<T>> work;

        std::stack<std::shared_ptr<bossThread<T>>> bosses;

        unsigned maxThreads{};

        bool isDone;

    public:

        threadPool () {
            maxThreads = std::thread::hardware_concurrency() * 4;
            isDone = false;
            bosses = std::stack<std::shared_ptr<bossThread<T>>>();
            work = std::shared_ptr<workQueue<T>>(new workQueue<T>());
            bosses.push(std::shared_ptr<bossThread<T>>(new bossThread<T>(work)));
        }

        ~threadPool() {
            waitUntilDone();
        }

        std::shared_ptr<task_s<T>> addWork(std::function<T()> someWork) {
            return work->addWork(someWork);
        }

        /*
        template<class T> 
        std::shared_ptr<task_s<T>> addWork(std::function<T()> work, unsigned maxThreads) {
            if (bosses[typeid(T)]) {
                bosses[typeid(T)] = std::shared_ptr<bossThread<T>>(new bossThread(maxThreads));
            }
            return bosses[typeid(T)]->addWork(work);
        }
        */

        void waitUntilDone() {
            isDone = true;
            try {
                while (!bosses.empty()) {
                    bosses.pop();
                }
            }
            catch (const std::exception& ex) {
                std::cout << "Thrown by threadPool wait: " << ex.what() << std::endl;
            }
        }

    };

}


#endif //MATRIXOPERATIONS_THREADPOOL_H
