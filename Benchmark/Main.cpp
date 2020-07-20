/// author: Christopher Brightman 
/// runs a number of benchmarks on the thread pool

#include <cstdlib>
#include <iostream>
#include <functional>
#include <string>
#include <cmath>
#include <thread>
#include <memory>
#include <mutex>

#include "timer.h"
#include "tpmain.h"

#define MAX_FOR_QUAD (std::thread::hardware_concurrency() * 20000)
#define NUM_TASKS (std::thread::hardware_concurrency() * 500)
#define MAX_FOR_FACT 20

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
void toTime(std::function<T()> func, unsigned long numToDo, std::string&& msg, const unsigned& threadMax = std::thread::hardware_concurrency() * 15) {
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

std::mutex lock;

void factorial(unsigned long numToFact) {
    unsigned long output = numToFact;
    unsigned long next = numToFact - 1;
    if (numToFact != 0) {
        for (int i = 0; i < numToFact - 1; i++) {
            output = output * next;
            next--;
        }
    }
    lock.lock();
    // if you think that this function is being optimized out then uncomment this
    //std::cout << numToFact << " factorial is " << output << std::endl;
    lock.unlock();
}

void factorialTest() {
    for (int i = 0; i < MAX_FOR_FACT; i++) {
        factorial(i);
    }
}

int main() {
    toTime<double>(quadratic_formula, NUM_TASKS, "Testing Quadratic Formula test.");
    toTime<void>(factorialTest, NUM_TASKS, "Testing Factorial Test.", std::thread::hardware_concurrency());
    return EXIT_SUCCESS;
}

