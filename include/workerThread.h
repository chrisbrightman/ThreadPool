#ifndef THREAD_POOL_WORKER_THREAD
#define THREAD_POOL_WORKER_THREAD

#include <thread>
#include <string>
#include <functional>

namespace tp {

	class workerThread {

	std::string name;
	std::thread thread;

	public:

		template<class T, class Args...>
		workerThread(std::function<T> function, Args args...) {
			thread = std::thread(function, args...);
		}

		void setName(std::string);

		~workerThread();

	private:
		



	};

}



#endif // THREAD_POOL_WORKER_THREAD