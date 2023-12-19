//
// Created by watson on 12/14/23.
//

#include "View.h"

namespace view{
    View::View(const sf::VideoMode &mode, const sf::String &title) : RenderWindow(mode, title) {}

    View::View(): RenderWindow(sf::VideoMode(1600, 900), "APman") {}
}