/// author: Christopher Brightman

#ifndef MATRIXOPERATIONS_WORKQUEUE_H
#define MATRIXOPERATIONS_WORKQUEUE_H

#include <queue>
#include <functional>
#include <mutex>
#include <memory>

namespace tp{

    /// a mutex for synchronization 
    static std::mutex lock;

    /// <summary>
    /// a struct for representing a task for the thread pool
    /// </summary>
    /// <typeparam name="T"> 
    /// the return type for the function
    /// </typeparam>
    template<class T>
    struct task_s {
        std::function<T()> function;
        T returnValue;
        bool isComplete;
    };

    /// <summary>
    /// a synchronized queue for the thread pool
    /// </summary>
    /// <typeparam name="T">
    /// the return type for the functions that are being added to the work queue
    /// </typeparam>
    template<class T>
    class workQueue {

        /// the underlying queue for the tasks
        std::queue<std::shared_ptr<task_s<T>>> toDo;

        /// the current size of the queue
        int size;

    public:

       /// <summary>
       /// creates a work queue
       /// </summary>
       workQueue() {
            size = 0;
            toDo = std::queue<std::shared_ptr<task_s<T>>>();
        }

        /// <summary>
        /// synchronized enqueue for the work queue
        /// </summary>
        /// <param name="function"> the function to add </param>
        /// <returns> 
        /// a pointer to the new task that had been added to the queue
        /// </returns>
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

        /// <summary>
        /// synchronized dequeue of the work
        /// </summary>
        /// <returns> 
        /// thr pointer to the task 
        /// </returns>
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

        /// <summary>
        /// returns the size of the queue
        /// </summary>
        /// <returns>
        /// the size of the queue
        /// </returns>
        int workLeftToDo() {
            return size;
        }

        /// <summary>
        /// if the work is done
        /// </summary>
        /// <returns>
        /// if the work is done
        /// </returns> 
        bool isWorkDone() {
            return size == 0;
        }

    };

}

#endif //MATRIXOPERATIONS_WORKQUEUE_H
