//
// Created by watson on 11/22/23.
//

#ifndef PACMAN_STATES_H
#define PACMAN_STATES_H

#include "../view/ViewFactory.h"
#include "../../logic/World.h"
#include "../view/Camera.h"
#include "../../logic/Score.h"
#include "../view/View.h"

#include <memory>
#include <SFML/Graphics.hpp>
#include <stack>
#include <iostream>

/**
 * Namespace for the control using States
 */
namespace s {

    static const std::string spritesPNG{"representation/data/Sprites.png"};
    static const std::string victoryPNG{"representation/data/Victory.png"};
    static const std::string pausedPNG{"representation/data/Paused.png"};
    static const std::string menuPNG{"representation/data/Menu.png"};
    static const std::string gameOverJPG{"representation/data/GameOver.jpg"};
    static const std::string fontTTF{"representation/data/BlueStyle.ttf"};

    class State; //Forward Declaration

    /**
     * State manager or context; States Design Pattern
     */
    class StateManager  {
    private:
        /**
         * Stack to hold states
         */
        std::stack< std::unique_ptr<State> > stateStack;

        /**
         * Holds the difficulty of the Levels, increases depending on progress
         */
        long int difficulty{0}; //Initialise on 0

        /**
         * Holds the amount of lives we still have; must be preserved towards the new game
         */
        short hearts{3}; //We start with 3 lives

        /**
         * Keep track of the latest pressed key
         */
        sf::Keyboard::Key curr{sf::Keyboard::Key::A}; //Initialise a standard value

        /**
         * Keeps the font loaded
         */
        sf::Font font;
    public:
        /**
         * Simple getter
         * @return
         */
        [[nodiscard]] short getHearts() const;

        /**
         * Simple setter
         * @param nHearts
         */
        void setHearts(short nHearts);

        /**
         * When a keyboard input is registered, delegate to State
         */
        void pressed();

        /**
         * Add a new State to the StackL
         * @param s
         */
        void push(std::unique_ptr<State> s);

        /**
         * Remove a state from the Stack
         */
        void pop(int times = 1);

        /**
         * Pop & Push at the same time
         */
        void replace(std::unique_ptr<State> s);

        /**
         * Simple Getter
         * @return
         */
        [[nodiscard]] long getDifficulty() const;

        /**
         * Increment the difficulty  by 1
         */
         void incDifficulty();

         /**
          * Simple Destructor
          */
        virtual ~StateManager();

        /**
         * Simple setter
         * @param curr
         */
        void setCurr(sf::Keyboard::Key Curr);

        /**
         * Simple getter
         * @return
         */
        const sf::Font &getFont() const;

        /**
         * Simple setter
         * @param difficulty
         */
        void setDifficulty(long diff);

        /**
         * Simple constructor
         */
        StateManager();
    };

    /**
     * Abstract Base Class for each State
     */
    class State {
    protected:
        /**
         * Holds a reference to the StateManager
         */
        StateManager& manager;

        /**
         * Load  a certain .png in the background
         * @param path
         */
        static void loadScreen(const std::string &path, bool clear=true);

        /**
         * Load a vector of strings onto the screen
         * @param data
         * @param center , define if the text should be centered
         */
        void loadText(const std::string &str, bool center=false);

    public:
        /**
         * Simple constructor
         * @param manager , ptr to the StateManager class to acces related data
         * @param window , ptr to manage the window contents
         */
        explicit State(StateManager &manager);

        /**
         * Pure Virtual function to delegate pressed input
         * @return
         */
        virtual void pressed(sf::Keyboard::Key &curr) = 0;

        /**
         * Retrigger drawing the initial data
         */
        virtual void update() = 0;

        /**
         * Simple Destructor
         */
        virtual ~State() = default;
    };

    /**
     * Specified State to handle the Menu
     */
    class MenuState: public State {
    public:
        /**
         * Constructor to setup Menu Loading Screen
         * @param manager
         * @param window
         */
        explicit MenuState(StateManager &manager);

        /**
         * Function to handle input
         */
        void pressed(sf::Keyboard::Key &curr)  override;

        /**
         * Function to update the window
         */
        void update() override;

        /**
         * Simple Destructor
         */
        ~MenuState() override = default;
    };

    /**
     * Specified State to handle the current Level: talks to the World
     */
    class LevelState: public State {
    private:
        /**
         * Contains the World we play on; Model
         */
        w::World world;
    public:
        /**
         * Constructor to setup everything
         * @param manager , StateManager
         * @param window , Window to draw in
         */
        explicit LevelState(StateManager &manager);

        /**
         * Function to handle input
         */
        void pressed(sf::Keyboard::Key &curr) override;

        /**
         * Function to update the window
         */
        void update() override;

        /**
         * Simple Destructor
         */
        ~LevelState() override = default;
    };

    /**
     * Specified State to handle the a game pause
     */
    class PausedState: public State {
    public:
        /**
         * Constructor to setup the view
         * @param manager
         * @param window
         */
        explicit PausedState(StateManager &manager);

        /**
         * Function to handle input
         */
        void pressed(sf::Keyboard::Key &curr)  override;

        /**
         * Function to update the window
         */
        void update() override;

        /**
         * Simple Destructor
         */
        ~PausedState() override = default;
    };

    /**
     * Specified State to handle the victory event
     */
    class VictoryState: public State {
    public:
        /**
         * Constructor to setup the view
         * @param manager
         * @param window
         */
        explicit VictoryState(StateManager &manager);

        /**
         * Function to handle input
         */
        void pressed(sf::Keyboard::Key &curr)  override;

        /**
         * Function to update the window
         */
        void update() override;

        /**
         * Simple Destructor
         */
        ~VictoryState() override = default;
    };

    /**
    * Specified State to handle the GameOver event
    */
    class GameOverState: public State {
    public:
        /**
         * Constructor
         * @param manager
         * @param window
         */
        explicit GameOverState(StateManager &manager);

        /**
         * Function to handle input
         */
        void pressed(sf::Keyboard::Key &curr)  override;

        /**
         * Function to update the window
         */
        void update() override;

        /**
         * Simple Destructor
         */
        ~GameOverState() override = default;
    };
}

#endif //PACMAN_STATES_H
