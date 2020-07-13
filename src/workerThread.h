/// author: Christopher Brightman
/// class for the worker threads
/// they take work of the work queue and do it

#ifndef THREAD_POOL_WORKER_THREAD
#define THREAD_POOL_WORKER_THREAD

#include <thread>
#include <string>
#include <functional>

#ifdef THREAD_POOL_DEBUG
#include <iostream>
#endif

#include "workQueue.h"

namespace tp {

    template<class T>
	class workerThread  : public std::thread {

	/// the workers id  
	unsigned workerId;

	/// no more work is to be added to the queue complete what's left
	bool isFinished;

	/// the work queue  
	std::shared_ptr<workQueue<T>> work;

	public:

	    /// <summary>
	    /// makes a work thread
	    /// </summary>
	    /// <param name="anId"> the worker id </param>
	    /// <param name="workQ"> the work queue to complete</param>
	    workerThread(unsigned&& anId, std::shared_ptr<workQueue<T>>& workQ)
	            : std::thread ( [ this] () { this->operate(); } ) {
            workerId = anId;
            isFinished = false;
            work = workQ;
        }

        /// <summary>
        /// tells the worker that no more work is to be added to the queue
        /// finish what is left
        /// </summary>
        inline void markDone() {
            isFinished = true;
        }

        /// <summary>
        /// sets an id for the worker
        /// </summary>
        /// <param name="anId"></param>
        inline void setId(unsigned anId) {
            workerId = anId;
        }

        ~workerThread() {
            while (!isFinished) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            #ifdef THREAD_POOL_DEBUG
                    std::cout << "workerThread: " << workerId << " exiting." << std::endl;
            #endif
            }

    protected:

	    /// <summary>
	    /// main run for the worker 
        /// grabs work off the queue and completes it otherwise it waits
	    /// </summary>
	    void operate() {
            try {
                while (!isFinished || !work->isWorkDone()) {
                    if (work->isWorkDone()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }
                    std::shared_ptr<task_s<T>> toDo = work->dequeueWork();
                    #ifdef THREAD_POOL_DEBUG
                        std::cout << "Worker Thread " << id << " grabed work of type " << type_info(T) << std::endl;
                    #endif
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



#endif // THREAD_POOL_WORKER_THREAD