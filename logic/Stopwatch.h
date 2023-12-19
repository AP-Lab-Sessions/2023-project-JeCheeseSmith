//
// Created by watson on 11/22/23.
//

#ifndef PACMAN_STOPWATCH_H
#define PACMAN_STOPWATCH_H

#include "../lib/Singleton.h"

#include <memory>
#include <chrono>
#include <iostream>
#include <cmath>

/**
 * Namespace used for anything time related
 */
namespace sec {
    /**
     * Clock Class to keep track and manage time utilities; Implemented as Singleton
     */
    class Stopwatch {
    public:
        /**
         * Delete other constructors because of the Singleton implementation
         */
        Stopwatch(const Stopwatch&) = delete;
        Stopwatch& operator=(const Stopwatch&) = delete;

        /**
         * Get the framerate, capped at 60 FPS
         * @return
         */
        float restart();

        /**
         * Retrieve the current duration of the game in seconds
         * @return
         */
        [[nodiscard]] long getDuration() const;

        /**
         * Simple Destructor
         */
        virtual ~Stopwatch() = default;

        /**
         * Make sure the Singleton has acces to the Constructor
         */
        friend class s::Singleton<Stopwatch>;
    private:
        /**
         * Start value
         */
        std::clock_t c_start;

        /**
         * Start value
         */
        const std::time_t d_start;

        /**
         * Private Constructor due to the Singleton Design Pattern
         */
        Stopwatch();
    };
}

#endif //PACMAN_STOPWATCH_H
