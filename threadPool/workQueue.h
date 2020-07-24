//
// Created by chris on 5/25/20.
//

#ifndef MATRIXOPERATIONS_WORKQUEUE_H
#define MATRIXOPERATIONS_WORKQUEUE_H

#include <queue>
#include <functional>
#include <mutex>
#include <memory>
#include <iostream>

namespace tp{

    static std::mutex lock;

    template<class T>
    struct task_s {
        std::function<T()> function;
        T returnValue;
        bool isComplete;
    };

    template<>
    struct task_s<void> {
        std::function<void()> function;
        bool isComplete;
    };

    template<class T>
    class workQueue {

        std::queue<std::shared_ptr<task_s<T>>> toDo;
        int size;

    public:


        workQueue() {
            size = 0;
            toDo = std::queue<std::shared_ptr<task_s<T>>>();
        }

        virtual std::shared_ptr<task_s<T>> addWork(std::function<T()> function) {
            try {
                std::shared_ptr<task_s<T>> task(new task_s<T>);
                task->function = function;
                task->isComplete = false;
                lock.lock();
                toDo.push(task);
                size++;
                lock.unlock();
                return std::shared_ptr<task_s<T>>(task);
            }
            catch (const std::exception& ex) {
                std::cerr << "The exception: " << ex.what() << " was thrown while adding work to the work queue." << std::endl;
            }
        }

        virtual std::shared_ptr<task_s<T>> dequeueWork() {
            try {
                lock.lock();
                if (isWorkDone()) {
                    lock.unlock();
                    return nullptr;
                }
                std::shared_ptr<task_s<T>> someWork(toDo.front());
                toDo.pop();
                size--;
                lock.unlock();
                return someWork;
            }
            catch (const std::exception& ex) {
                std::cerr << "The work queue threw an exception " << ex.what() << std::endl;
            }
        }

        virtual int workLeftToDo() {
            return size;
        }

        virtual bool isWorkDone() {
            return size == 0;
        }

    };

}

#endif //MATRIXOPERATIONS_WORKQUEUE_H
