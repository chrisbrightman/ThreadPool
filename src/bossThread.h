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

	class bossThread : public std::thread{

		std::stack<std::shared_ptr<workerThread>> workers;

		template<class T>
		using word = std::shared_ptr<workQueue<T>>;

		std::atomic<int> numberWorkers;

		unsigned hardwareConcurency;

		unsigned maxThreads;

		bool isFinished;

	public:

		template<class T>
		bossThread(workQueue<T>& work) : std::thread( [this, work] () { this->operate(work); }) {

		}

		bossThread(unsigned threadMax = std::thread::hardware_concurrency() * 100) {
			maxThreads = threadMax;
			hardwareConcurency = std::thread::hardware_concurrency();
			numberWorkers = 0;
			isFinished = false;
			workers = std::stack<std::shared_ptr<workerThread>>();
		}

		~bossThread() {
			stopWork();
		}

		void stopWork();

		template<class T>
		std::shared_ptr<task_s<T>> addWork(std::function<T()> func) {
			std::shared_ptr<task_s<T>> task = work->addWork(func);
		}

	private:

		template<class T>
		void increaseThreads(int numToMake, workQueue<T>& work) {
			if (numberWorkers = maxThreads) {
				return;
			}
			if (numberWorkers + numToMake > maxThreads) {
				#ifdef THREAD_POOL_DEBUG
					std::cout << "Creating " << maxThreads - numberWorkers << " more workers." << std::endl;
				#endif // THREAD_POOL_DEBUG
				for (int i = 0; i < maxThreads - numberWorkers; i++) {
					workers.push(std::shared_ptr<workerThread>(new workerThread(numberWorkers + i, work)));
				}
				numberWorkers = maxThreads;
			}
			else {
				#ifdef THREAD_POOL_DEBUG
					std::cout << "Creating " << numToMake<< " more workers." << std::endl;
				#endif // THREAD_POOL_DEBUG
				for (int i = 0; i < numToMake; i++) {
					workers.push(std::shared_ptr<workerThread>(new workerThread(numberWorkers + i, work)));
				}
				numberWorkers += numToMake;
			}
	}
		 
		template<class T>
		void operate(workQueue<T>& work) {
			while (!isFinished) {
				if (work->size > numberWorkers * 4) {
					increaseThreads(numberWorkers, work);
				}
				std::this_thread::sleep_for(std::chrono::microseconds(100));
			}
		}


	};
}


#endif // THREAD_POOL_BOSS_THREAD