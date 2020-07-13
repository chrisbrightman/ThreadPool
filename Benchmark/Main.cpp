//
// Created by chris on 6/15/20.
//

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

