/// author: Christopher Brightman

#ifndef THREADPOOL_TIMER_H
#define THREADPOOL_TIMER_H

#include <chrono>

/// <summary>
/// a timer class
/// </summary>
class timer {

    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;

public:

    /// <summary>
    /// creates a timer
    /// </summary>
    timer();

    /// <summary> 
    /// destroys and prints out the time that the object existed for 
    /// <\summary>
    ~timer();

};


#endif //THREADPOOL_TIMER_H
