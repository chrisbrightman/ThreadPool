/// author: Christopher Brightman 
/// runs a number of benchmarks on the thread pool

#include <cstdlib>
#include <iostream>
#include <functional>
#include <string>
#include <cmath>
#include <thread>
#include <memory>

#include "timer.h"
#include "tpmain.h"

#define MAX_FOR_QUAD (std::thread::hardware_concurrency() * 20000)
#define NUM_TASKS (std::thread::hardware_concurrency() * 1000)

/// <summary>
/// this creates a timer and runs the given function a given number of times
/// </summary>
/// <typeparam name="T">
/// the return type for the function given
/// </typeparam>
/// <param name="func"> 
/// a fucntion 
/// </param>
/// <param name="numToDo"> 
/// how many times to do the function
/// </param>
/// <param name="msg">
/// a message to print to the console before running the benchmark
/// </param>
/// <param name="threadMax">
/// the maximum number of threads for the pool
/// </param>
template<typename T>
void toTime(std::function<T()> func, unsigned long numToDo, std::string&& msg, const unsigned& threadMax = 0) {
    std::cout << msg << std::endl;
    tp::threadPool<T> pool = tp::threadPool<T>(threadMax);
    timer stopWatch = timer();
    for (unsigned long i = 0; i < numToDo; i++) {
        pool.addWork([func] () { return func();});
    }
    pool.waitUntilDone();
}

/// <summary>
/// does the quadradic formula MAX_FOR_QUAD times
/// </summary>
/// <returns></returns>
double quadratic_formula() {
    std::unique_ptr<double[]> returnArray(new double[MAX_FOR_QUAD]);
    for (int i = 1; i < MAX_FOR_QUAD; i++) {
        double squared = pow(i, 2) - (4 * i * i);
        if (squared >= 0) {
            double top = (double) (-1 * i) + (double) sqrt(squared);
            returnArray[i] = top / (2 * i);
        }
    }
    return returnArray[0];
}

int main() {
    const unsigned topThreads = std::thread::hardware_concurrency() * 15;
    toTime<double>(quadratic_formula, NUM_TASKS, "Testing Quadratic Formula test.", topThreads);
    return EXIT_SUCCESS;
}

