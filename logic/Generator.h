//
// Created by watson on 11/22/23.
//

#ifndef PACMAN_RANDOM_H
#define PACMAN_RANDOM_H

#include "Stopwatch.h"

#include <memory>
#include <random>
#include <chrono>
#include <iostream>

/**
 * Namespace for the random(-generator) part of the game
 */
namespace rad {
    /**
     * Class to call random function on; Implemented as a Singleton so there is 1 unique generator for each game AND its easily accessible (getInstance function)
     */
    class Generator{
    public:
        /**
         * Delete other constructors
         */
        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        /**
         * Generate a random between lbound and rbound
         * @param lbound
         * @param rbound
         * @return
         */
        int generate(int lbound,int rbound);

        /**
         * Simple Destructor
         */
        virtual ~Generator() = default;

        /**
         * Make sure the Singleton has acces to the private constructor
         */
        friend class s::Singleton<Generator>;
    private:
        /**
         * Generator to be used
         */
        std::mt19937 generator;

        /**
         * Simple Constructor: Private since it is a Singleton
         */
        Generator();
    };
}

#endif //PACMAN_RANDOM_H
