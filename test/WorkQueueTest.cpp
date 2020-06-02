//
// Created by chris on 5/26/20.
//

#include "gtest/gtest.h"
#include "workQueue.h"
#include <functional>

using namespace tp;

bool foo(int i, char j) {
    return false;
}

TEST(ThreadPoolWorkQueue, addWork) {
    workQueue<bool> test = workQueue<bool>();
    bool g = false;
    bool *bar = &g;
    test.addWork([] { return foo(1, 'a'); });
    EXPECT_EQ(test.workLeftToDo(), 1);
    EXPECT_EQ(test.isWorkDone(), false);
}

int bar(int i, int j) {
    return i + j;
}

TEST(ThreadPoolWorkQueue, testWork) {
    workQueue<int> test = workQueue<int>();
    for (int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            int g = 2;
            int* foo = &g;
            test.addWork([i, j] () { return bar(i, j); });
        }
    }
    for (int i = 0; i < 10; i++) {
        for(int j = 0; j > 10; j++) {
            task_s<int> *output = test.dequeueWork();
            EXPECT_EQ(output->function(), i + j);
            delete output;
        }
    }
}

