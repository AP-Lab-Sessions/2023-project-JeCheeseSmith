//
// Created by watson on 11/26/23.
//

#include "ViewFactory.h"

namespace fact {
    std::unique_ptr<em::Pacman>ViewFactory::createPacman(const em::Coordinate &rect) const {
        auto temp = std::make_unique<em::Pacman>(rect);
        temp->addObserver(std::make_shared<view::Pacman>()); //Add the observer to the list
        return temp;
    }

    std::unique_ptr<em::Ghost> ViewFactory::createGhost(const em::Coordinate &rect,const long &fearTime,const long &delay) const {
        auto temp = std::make_unique<em::Ghost>(rect,fearTime,delay);
        temp->addObserver(std::make_shared<view::Ghost>()); //Add the observer to the list
        return temp;
    }

    std::unique_ptr<em::Coin> ViewFactory::createCoin(const em::Coordinate &rect) const {
        auto temp = std::make_unique<em::Coin>(rect);
        temp->addObserver(std::make_shared<view::Coin>()); //Add the observer to the list
        return temp;
    }

    std::unique_ptr<em::Fruit> ViewFactory::createFruit(const em::Coordinate &rect) const {
        auto temp = std::make_unique<em::Fruit>(rect);
        temp->addObserver(std::make_shared<view::Fruit>()); //Add the observer to the list
        return temp;
    }

    std::unique_ptr<em::Wall> ViewFactory::createWall(const em::Coordinate &rect) const {
        auto temp = std::make_unique<em::Wall>(rect);
        temp->addObserver(std::make_shared<view::Wall>()); //Add the observer to the list
        return temp;
    }
}