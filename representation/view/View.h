//
// Created by watson on 12/14/23.
//

#ifndef PACMAN_VIEW_H
#define PACMAN_VIEW_H

#include "../../logic/Stopwatch.h"

#include <SFML/Graphics.hpp>
#include <memory>

namespace view{
    /**
     * Window class inherited from sf::RenderWindow so. Implemented as a Singleton for easy acces.
     */
    class View: public sf::RenderWindow {
    public:
        /**
         * Delete other Constructors
         */
        View(const View&) = delete;
        View& operator=(const View&) = delete;

        /**
         * Simple Destructor
         */
        ~View() override = default;

        /**
         * Make sure the Singleton has acces to the Constructor
         */
        friend class s::Singleton<View>;
    private:
        /**
         * Private Constructor to implement Singleton
         * @param mode sf::VideoMode
         * @param title The title of the window
         */
        View(const sf::VideoMode &mode, const sf::String &title);

        /**
         * Private Default Constructor with no parameters
         */
        View();
    };
}

#endif //PACMAN_VIEW_H
