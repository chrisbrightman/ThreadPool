#ifndef THREAD_POOL_WORKER_THREAD
#define THREAD_POOL_WORKER_THREAD

#include <thread>
#include <string>
#include <functional>
#include <iostream>

#include "workQueue.h"

namespace tp {

    template<class T>
	class workerThread  : public std::thread {

	unsigned workerId;
	bool isFinished;

	std::shared_ptr<workQueue<T>> work;

	public:

	    /*
	    template<class _Callable, class... _Args>
	    workerThread(short anId, _Callable&& func, _Args&&... _args) : std::thread (func, _args...) {
            isFinished = false;
            workerId = anId;
	    }
	     */

	    workerThread(unsigned&& anId, std::shared_ptr<workQueue<T>>& workQ)
	            : std::thread ( [ this] () { this->operate(); } ) {
            workerId = anId;
            isFinished = false;
            work = workQ;
        }

	    inline void markDone();

		void setName(unsigned anId);

		~workerThread();

    protected:

	    void operate() {
            try {
                while (!isFinished || !work->isWorkDone()) {
                    if (work->isWorkDone()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        continue;
                    }
                    std::shared_ptr<task_s<T>> toDo = work->dequeueWork();
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