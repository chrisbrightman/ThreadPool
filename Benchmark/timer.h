//
// Created by chris on 6/15/20.
//

#ifndef THREADPOOL_TIMER_H
#define THREADPOOL_TIMER_H

#include <chrono>


class timer {

    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;

public:
    timer();

    ~timer();


};


#endif //THREADPOOL_TIMER_H
