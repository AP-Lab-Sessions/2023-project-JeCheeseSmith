//
// Created by watson on 11/26/23.
//

#ifndef PACMAN_VIEWFACTORY_H
#define PACMAN_VIEWFACTORY_H

#include "../../logic/AbstractFactory.h"
#include "../../logic//Score.h"
#include "EntityView.h"

#include <memory>

namespace fact {
    /**
     * Concrete Factory to create entities and assign a corresponding view
     */
    class ViewFactory : public AbstractFactory {
    public:
        /**
         * Creates the entity: Pacman Attaches an corresponding view
         * \param rect Initial Coordinate of the Entity
         * @return Pacman object
         */
        [[nodiscard]] std::unique_ptr<em::Pacman> createPacman(const em::Coordinate &rect) const override;

        /**
         * Creates the entity: Ghost Attaches an corresponding view
         * @param rect Initial Coordinate of the Entity
         * @param fearTime Time a Ghost will stay in fear-mode
         * @param delay Number of seconds the Ghost will wait before starting to chase or move
         * @return Ghost Object
         */
        [[nodiscard]] std::unique_ptr<em::Ghost> createGhost(const em::Coordinate &rect, const long &fearTime, const long &delay) const override;

        /**
         * Creates the entity: Coin Attaches an corresponding view
         * @param rect Initial Coordinate of the Entity
         * @return Coin Object
         */
        [[nodiscard]] std::unique_ptr<em::Coin> createCoin(const em::Coordinate &rect) const override;

        /**
         * Creates the entity: Fruit Attaches an corresponding view
         * @param rect Initial Coordinate of the Entity
         * @return Fruit Object
         */
        [[nodiscard]] std::unique_ptr<em::Fruit> createFruit(const em::Coordinate &rect) const override;

        /**
         * Creates the entity: Wall Attaches an corresponding view
         * @param rect Initial Coordinate of the Entity
         * @return Wall Entity
         */
        [[nodiscard]] std::unique_ptr<em::Wall> createWall(const em::Coordinate &rect) const override;

        /**
         * Simple Destructor
         */
        ~ViewFactory() override = default;
    };
}

#endif //PACMAN_VIEWFACTORY_H
