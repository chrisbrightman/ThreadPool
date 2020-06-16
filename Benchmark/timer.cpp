//
// Created by chris on 6/15/20.
//

#include <iostream>

#include "timer.h"

timer::timer() {
    start = std::chrono::system_clock::now();
}

timer::~timer() {
    end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Completed in " << duration.count() << " milliseconds." << std::endl;
}
