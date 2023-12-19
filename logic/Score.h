//
// Created by watson on 11/22/23.
//

#ifndef PACMAN_SCORE_H
#define PACMAN_SCORE_H

#include "../lib/DesignByContract.h"
#include "../lib/helper.h"
#include "Observer.h"
#include "EntityModel.h"
#include "Stopwatch.h"

#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>

namespace obs {
    /**
     * Lower the use of magic numbers
     */
    static int maxScore = 1000;

    /**
     * Specialised Observer to manage the scores for each game, implemented as a singleton so the same score will be used for all games
     */
    class Score : public Observer {
    public:
        /**
         * Delete other constructors because of the Singleton implementation
         */
        Score(const Score&) = delete;
        Score& operator=(const Score&) = delete;

        /**
         * Exports the current scores to a file
         */
        void exporter() const;

        /**
         * Overloaded function from obs::update
         * @param et EntityModel
         */
        void update(const em::EntityModel &et) const final;

        /**
         * Simple getter
         * @return Current Registered Highscores
         */
        const std::vector<int> &getHighscores() const;

        /**
         * Simple getter
         * @return current Score
         */
        int getScore() const;

        /**
         * Simple setter
         * @param scor
         */
        void setScore(int scor);

        /**
         * Add bonus points when a level has been completed
         */
        void clearedLevel() const;

        /**
         * Simple Destructor
         */
        ~Score() override = default;

        /**
         * Make sure the Singleton has acces to the Constructor
         */
        friend class s::Singleton<Score>;
    private:
        /**
         * Location to the score file
         */
        static const std::string location;

        /**
         * Current score in game
         */
        mutable int score; //Declared mutable due to the update() function being const. The const keeps to be properly inherited. "Logical const"

        /**
         * Variable to keep track of the timespan
         */
        mutable long duration;

        /**
         * Keeps the timestamp for the most recent coin which been eaten
         */
        mutable long lastCoinEaten;

        /**
         * Highscores loaded from location
         */
        mutable std::vector<int> highscores;

        /**
         * Simple constructor
         */
        explicit Score();
    };

}

#endif //PACMAN_SCORE_H
