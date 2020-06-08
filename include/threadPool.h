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

    class threadPool {

        std::unordered_map<std::type_info, std::shared_ptr<bossThread<class T>>> bosses;

        unsigned maxThreads;

        bool isDone;

    public:

        explicit threadPool () {
            isDone = false;
            bosses = std::unordered_map<std::type_info, std::shared_ptr<bossThread<class T>>>();
        }

        ~threadPool() {
            waitUntilDone();
        }

        template<class T>
        std::shared_ptr<task_s<T>> addWork(std::function<T()> work) {
            if (bosses[typeid(T)]) {
                bosses[typeid(T)] = std::shared_ptr<bossThread<T>>(new bossThread<T>());
            }
            return bosses[typeid(T)]->addWork(work);
        }

        template<class T> 
        std::shared_ptr<task_s<T>> addWork(std::function<T()> work, unsigned maxThreads) {
            if (bosses[typeid(T)]) {
                bosses[typeid(T)] = std::shared_ptr<bossThread<T>>(new bossThread<T>(maxThreads));
            }
            return bosses[typeid(T)]->addWork(work);
        }

        void waitUntilDone() {
            isDone = true;
            try {
                for(auto i = bosses.begin(); i != bosses.end(); i++) {
                    std::shared_ptr<bossThread<class T>> boss = bosses[i->first];
                    boss->stopWork();
                    bosses.erase(i->first);
                }
            }
            catch (const std::exception& ex) {
                std::cout << "Thrown by threadPool wait: " << ex.what() << std::endl;
            }
        }

    };


}


#endif //MATRIXOPERATIONS_THREADPOOL_H
