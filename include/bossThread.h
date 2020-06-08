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
	class bossThread {

		std::shared_ptr<workQueue<T>> work;

		std::stack<workerThread> workers;

		std::atomic<int> numberWorkers;

		unsigned hardwareConcurency;

		unsigned maxThreads;

	public:

		bossThread(unsigned threadMax = std::thread::hardware_concurrency() * 100) {
			maxThreads = threadMax;
			hardwareConcurency = std::thread::hardware_concurrency();
			work = std::shared_ptr<workQueue<T>>(new workQueue<T>());
			workerThread start(0, work);
			numberWorkers = 0;
		}

		~bossThread() {
			stopWork();
		}

		void stopWork() {
			for (int i = 0; i < numberWorkers; i++) {
				try {
					workerThread worker = workers.top();
					wokers.pop();
					worker.markDone();
					worker.join();
				}
				catch (std::exception ex) {
					std::cout << "Exception thrown by joining woker thread " << i << " " << ex.what() << std::endl;
				}
			}
		}

		std::shared_ptr<task_s<T>> addWork(std::function<T()> func) {
			std::shared_ptr<task_s<T>> work->addWork(func);
			if (work->size() > )
		}

	private:

		void increaseThreads(int numToMake) {
			if (numberWorkers = maxThreads) {
				return;
			}
			if (numberWorkers + numToMake > maxThreads) {
				#ifdef THREAD_POOL_DEBUG
					std::cout << "Creating " << maxThreads - numberWorkers << " more workers." << std::endl;
				#endif
				for (int i = 0; i < maxThreads - numberWorkers; i++) {
					workers.push(workerThread(numberWorkers + i, work));
				}
				numberWorkers = maxThreads;
			}
			else {
				#ifdef THREAD_POOL_DEBUG
					std::cout << "Creating " << numToMake<< " more workers." << std::endl;
				#endif
				for (int i = 0; i < numToMake; i++) {
					workers.push(workerThread(numberWorkers + i, work));
				}
				numberWorkers += numToMake;
			}
		}


	};
}


#endif // THREAD_POOL_BOSS_THREAD