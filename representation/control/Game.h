//
// Created by watson on 11/22/23.
//

#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H

#include "States.h"
#include "../../lib/helper.h"
#include "../../lib/DesignByContract.h"
#include "../../logic/Stopwatch.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>

/**
 * Namespace for everything related to starting & stopping the game
 */
namespace play {

    /**
     * Class to manage start & stop of the game, delegates task to "sub-controllers".
     */
    class Game {
    private:
        /**
         * State manager
         */
        std::shared_ptr<s::StateManager> manager;

        /**
        * Checks if all the required files are present
        */
        [[nodiscard]] static bool consistent() ;

        /**
         * Function to run/start the Game
         */
        void gameLoop();

        /**
         * Helper function to manage events correctly; keeps the code clean
         */
        void handleEvents();

    public:
        /**
         * Constructor , instantiates the class and verify consistency
         * REQUIRE(consistent(), "Program is inconsistent, abort");
         */
        Game();

        /**
         * Simple Destructor
         */
        virtual ~Game();
    };
}

#endif //PACMAN_GAME_H
