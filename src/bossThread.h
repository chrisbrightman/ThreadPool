// author: christopher Brightman
#ifndef THREAD_POOL_BOSS_THREAD
#define THREAD_POOL_BOSS_THREAD

#include <stack>
#include <memory>
#include <atomic>
#include <iostream>

#include "workQueue.h"
#include "workerThread.h"

namespace tp {

    template<class T>
	class bossThread : public std::thread {

		std::stack<std::shared_ptr<workerThread<T>>> workers;

		std::atomic<int> numberWorkers{};

		std::shared_ptr<workQueue<T>> work;

		unsigned maxWorkers;

		bool isFinished;

	public:

		explicit bossThread(std::shared_ptr<workQueue<T>>& aWork) : bossThread(std::thread::hardware_concurrency() * 4, aWork) {
		}

		explicit bossThread(unsigned threadMax, std::shared_ptr<workQueue<T>>& someWork) : std::thread([this] () { this->operate(); }) {
		    this->work = someWork;
            maxWorkers = std::thread::hardware_concurrency() * 4;
            isFinished = false;
            numberWorkers = 1;
            workers = std::stack<std::shared_ptr<workerThread<T>>>();
			workers.push(std::shared_ptr<workerThread<T>>(new workerThread<T>(0, someWork)));
		}

		~bossThread() {
			if (!isFinished) {
				stopWork();
			}
		}

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