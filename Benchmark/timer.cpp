/// author: Christopher Brightman

#include <iostream>

#include "timer.h"

// constructor 
timer::timer() {
    start = std::chrono::system_clock::now();
}

// constructor
timer::~timer() {
    end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Completed in " << duration.count() << " milliseconds." << std::endl;
}
