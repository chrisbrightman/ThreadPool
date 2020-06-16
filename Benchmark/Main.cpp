//
// Created by chris on 6/15/20.
//

#include <cstdlib>
#include <iostream>
#include <functional>
#include <string>

#include "timer.h"
#include "tpmain.h"

template<typename T>
void time(const std::function<T()>& func, unsigned long numToDo, const std::string& msg, unsigned threadMax = 0) {
    tp::threadPool<T> pool;
    if (threadMax <= 0) {
        pool = tp::threadPool<T>();
    }
    else {
        pool = tp::threadPool<T>(threadMax);
    }
    std::cout << msg << std::endl;
    timer stopWatch = timer();
    for (unsigned long i = 0; i < numToDo; i++) {
        pool.addWork(func());
    }
    pool.waitUntilDone();
}

int main() {
    timer time = timer();
    long j = 0;
    for (long i = 0; i < 1000000000; i++) {
        j += i;
    }
    std::cout << j << std::endl;
    return EXIT_SUCCESS;
}

