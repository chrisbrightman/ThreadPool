//
// Created by chris on 5/25/20.
//

#ifndef MATRIXOPERATIONS_WORKQUEUE_H
#define MATRIXOPERATIONS_WORKQUEUE_H

#include <queue>
#include <functional>
#include <mutex>
#include <memory>

namespace tp{

    static std::mutex lock;

    template<class T>
    struct task_s {
        std::function<T()> function;
        T returnValue;
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
            std::shared_ptr<task_s<T>> task(new task_s<T>);
            task->function = function;
            task->isComplete = false;
            lock.lock();
            toDo.push(task);
            size++;
            lock.unlock();
            return std::shared_ptr<task_s<T>>(task);
        }

        virtual std::shared_ptr<task_s<T>> dequeueWork() {
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

        virtual int workLeftToDo() {
            return size;
        }

        virtual bool isWorkDone() {
            return size == 0;
        }

    };

}

#endif //MATRIXOPERATIONS_WORKQUEUE_H
