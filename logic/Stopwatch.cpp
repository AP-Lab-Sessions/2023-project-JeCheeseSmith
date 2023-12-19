//
// Created by watson on 11/22/23.
//

#include "Stopwatch.h"

namespace sec {
    Stopwatch::Stopwatch(): d_start(std::time(nullptr)) {
        c_start = std::clock(); //Initialise variables
    }

    float Stopwatch::restart() {
        float deltatime = 1.f / ( (float) clock() - (float) c_start);
        c_start = std::clock(); //Restart clock
        return std::min(deltatime, 1.f/60.f); //Cap at 60 Frames!
    }

    long Stopwatch::getDuration() const {
        double duration = std::difftime(std::time(nullptr),d_start);
        return std::lround(duration);
    }
}