// author: christopher Brightman

#ifndef THREAD_POOL_BOSS_THREAD
#define THREAD_POOL_BOSS_THREAD

#include <stack>
#include <memory>
#include <atomic>

#ifdef THREAD_POOL_DEBUG
#include <iostream>
#endif

#include "workQueue.h"
#include "workerThread.h"

namespace tp {

    /// <summary>
	/// repersents a manager thread to manage all the workers
    /// </summary>
    /// <typeparam name="T">
	/// the return type for the work to be completed
	/// </typeparam>
    template<class T>
	class bossThread : public std::thread {

		// the worker threads
		std::stack<std::shared_ptr<workerThread<T>>> workers;

		// the number of workers
		std::atomic<int> numberWorkers{};

		// the work queue for the workers 
		std::shared_ptr<workQueue<T>> work;

		// the maximum number of workers the boss can manage
		unsigned maxWorkers;

		// is the boss done
		bool isFinished;

	public:

		/// <summary>
		/// if no maximum workers is given
		/// </summary>
		/// <param name="aWork"> the work queue to use</param>
		explicit bossThread(std::shared_ptr<workQueue<T>>& aWork) : bossThread(std::thread::hardware_concurrency() * 4, aWork) {
		}

		/// <summary>
		/// if a maximum workers is given
		/// creates only one worker initially 
		/// </summary>
		/// <param name="threadMax"> the maximum number of workers</param>
		/// <param name="someWork"> the work queuue to assign to the workers</param>
		explicit bossThread(unsigned threadMax, std::shared_ptr<workQueue<T>>& someWork) : std::thread([this] () { this->operate(); }) {
		    this->work = someWork;
            maxWorkers = threadMax;
            isFinished = false;
            numberWorkers = 1;
            workers = std::stack<std::shared_ptr<workerThread<T>>>();
			workers.push(std::shared_ptr<workerThread<T>>(new workerThread<T>(0, someWork)));
		}

		/// <summery> 
		/// waits for the workers to be done before returning
		/// </summery>
		~bossThread() {
			if (!isFinished) {
				stopWork();
			}
		}

        /// <summary>
        /// stops all the workers that the boss is managing
        /// </summary>
        void stopWork() {
            for (int i = 0; i < numberWorkers; i++) {
                try {
                    std::shared_ptr<workerThread<T>> worker = workers.top();
                    workers.pop();
                    worker->markDone();
                    worker->join();
                }
                catch (std::exception ex) {
                    std::cout << "Exception thrown by joining worker thread " << i << " " << ex.what() << std::endl;
                }
            }
			isFinished = true;
        }

	private:

		/// <summary>
		/// adds a given number of workers to the bosses work stack 
		/// </summary>
		/// <param name="numToMake">
		/// the number of workers to make
		/// </param>
		void increaseThreads(int numToMake) {
			if (numberWorkers == maxWorkers) {
				return;
			}
			if (numberWorkers + numToMake > maxWorkers) {
				#ifdef THREAD_POOL_DEBUG
					std::cout << "Creating " << maxWorkers - numberWorkers << " more workers." << std::endl;
				#endif // THREAD_POOL_DEBUG
				for (int i = 0; i < maxWorkers - numberWorkers; i++) {
					workers.push(std::shared_ptr<workerThread<T>>(new workerThread<T>(numberWorkers + i, work)));
				}
				numberWorkers = maxWorkers;
			}
			else {
				#ifdef THREAD_POOL_DEBUG
					std::cout << "Creating " << numToMake<< " more workers." << std::endl;
				#endif // THREAD_POOL_DEBUG
				for (int i = 0; i < numToMake; i++) {
					workers.push(std::shared_ptr<workerThread<T>>(new workerThread<T>(numberWorkers + i, work)));
				}
				numberWorkers += numToMake;
			}
		}
		 
		/// <summary>
		/// the main operation function for the boss 
		/// ocasionally checks to see the size of the work queue
		/// if the work queue is too big make more workers
		/// </summary>
		void operate() {
			while (!isFinished) {
				if (work->workLeftToDo() > numberWorkers * 4) {
					increaseThreads(numberWorkers);
				}
				std::this_thread::sleep_for(std::chrono::microseconds(100));
			}
		}


	};
}


#endif // THREAD_POOL_BOSS_THREAD