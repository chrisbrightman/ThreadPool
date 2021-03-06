//
// Created by chris on 5/28/20.
//

#include <memory>
#include <iostream>
#include <chrono>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "threadPool.h"

using namespace tp;

#define MAXTHREADS (std::thread::hardware_concurrency() * 4) 
#define TOMAKE 24 

double foo(int a, int b, int c) {
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            double squared = pow(b, 2) - (4 * a * c);
            if (squared >= 0) {
                double top = (double) (-1 * b) + (double) sqrt(squared);
                double g = top / (2 * a);
            }
        }
    }
    return 0;
}

TEST(ThreadPoolTest, addWork) {
    threadPool<double> test = threadPool<double>(MAXTHREADS);
    auto valuse = new std::shared_ptr<task_s<double>>[TOMAKE][TOMAKE][TOMAKE];
    auto now = std::chrono::steady_clock::now();
    for (int i = 1; i < TOMAKE + 1; i++) {
        for (int j = 0; j < TOMAKE; j++) {
            for (int k = 0; k < TOMAKE; k++) {
                valuse[i - 1][j][k] = test.addWork([i, j, k] () { return foo(i, j, k); });
            }
        }
    }
    test.waitUntilDone();
    auto then = std::chrono::steady_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(then - now);
    for (int i = 1; i < TOMAKE + 1; i++) {
        for (int j = 0; j < TOMAKE; j++) {
            for(int k = 0; k < TOMAKE; k++) {
                //EXPECT_EQ(valuse[i - 1][j][k]->returnValue, foo(i, j, k));
            }
        }
    }
    std::cout << "ThreadPool.addWork time to complete: " << time.count() << std::endl;
}
