//
// author: christopher Brightman
// this is the main thread pool object
// represents a fully managed thread pool for which work can be added
// must be initialized with a template that is for the return type of the
// function that is being added to the pool
//

#ifndef MATRIXOPERATIONS_THREADPOOL_H
#define MATRIXOPERATIONS_THREADPOOL_H


#include <thread>
#include <queue>
#include <functional>
#include <stack>
#include <unordered_map>

#ifdef THREAD_POOL_DEBUG
#include <iostream>
#endif

#include "workQueue.h"
#include "workerThread.h"
#include "bossThread.h"


namespace tp {

    template<class T>
    class threadPool {

        /// the work left to do
        std::shared_ptr<workQueue<T>> work;

        /// the boss threads
        std::stack<std::shared_ptr<bossThread<T>>> bosses;

        unsigned maxThreads{};

        /// is the thread done
        bool isDone;

    public:

        /// if no thread limit is given
        threadPool () : threadPool(std::thread::hardware_concurrency() * 4) {
        }

        /// if a thread limit is given
        /// @param maxThreads the maximum threads to make
        threadPool(unsigned maxThreads) {
            isDone = false;
            bosses = std::stack<std::shared_ptr<bossThread<T>>>();
            #ifdef THREAD_POOL_DEBUG
                std::cout << "initializing workQueue\n";
            #endif // THREAD_POOL_DEBUG
            work = std::shared_ptr<workQueue<T>>(new workQueue<T>());
            #ifdef THREAD_POOL_DEBUG
                std::cout << "initializing bossThread\n";
            #endif // THREAD_POOL_DEBUG
            bosses.push(std::shared_ptr<bossThread<T>>(new bossThread<T>(maxThreads, work)));
        }

        /// waits until the work is done
        ~threadPool() {
            waitUntilDone();
        }

        /// this adds work to the 
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
                auto doneQ = std::stack<std::shared_ptr<bossThread<T>>>();
                while (!bosses.empty()) {
                    std::shared_ptr<bossThread<T>> boss = bosses.top();
                    boss->stopWork();
                    doneQ.push(boss);
                    bosses.pop();
                }
                while (!doneQ.empty()) {
                    std::shared_ptr<bossThread<T>> boss = doneQ.top();
                    doneQ.pop();
                    boss->join();
                }
            }
            catch (const std::exception& ex) {
                std::cout << "Thrown by threadPool wait: " << ex.what() << std::endl;
            }
        }

    };

}


#endif //MATRIXOPERATIONS_THREADPOOL_H
