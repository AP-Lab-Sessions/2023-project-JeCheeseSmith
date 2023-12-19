//
// Created by watson on 11/26/23.
//

#ifndef PACMAN_ABSTRACTFACTORY_H
#define PACMAN_ABSTRACTFACTORY_H

#include "EntityModel.h"

#include <memory>
#include <set>

/**
 * Namespace used by the creation patterns & aspects of the game: Factory Patterns
 */
namespace fact{
    /**
     * Class which implements the Abstract Factory Design Pattern. A concrete factory is implemented in the representation library.
     */
    class AbstractFactory {
    public:
        /**
         * Creates the entity: Pacman
         * @param rect Initial Coordinate of the Entity
         * @return
         */
        [[nodiscard]] virtual std::unique_ptr<em::Pacman> createPacman(const em::Coordinate &rect) const = 0;

        /**
         * Creates the entity: Ghost
         * @param rect Initial Coordinate of the Entity
         * @param fearTime Time a Ghost will stay in fear-mode
         * @param delay Number of seconds the Ghost will wait before starting to chase or move
         * @return
         */
        [[nodiscard]] virtual std::unique_ptr<em::Ghost> createGhost(const em::Coordinate &rect,const long &fearTime,const long &delay) const = 0;

        /**
         * Creates the entity: Coin
         * @param rect Initial Coordinate of the Entity
         * @return
         */
        [[nodiscard]] virtual std::unique_ptr<em::Coin> createCoin(const em::Coordinate &rect) const = 0;

        /**
         * Creates the entity: Fruit
         * @param rect Initial Coordinate of the Entity
         * @return
         */
        [[nodiscard]] virtual std::unique_ptr<em::Fruit> createFruit(const em::Coordinate &rect) const = 0;

        /**
         * Creates the entity: Wall
         * @param rect Initial Coordinate of the Entity
         * @return
         */
        [[nodiscard]] virtual std::unique_ptr<em::Wall> createWall(const em::Coordinate &rect) const = 0;

        /**
         * Simple Destructor
         */
        virtual ~AbstractFactory() = default;
    };
}

#endif //PACMAN_ABSTRACTFACTORY_H
