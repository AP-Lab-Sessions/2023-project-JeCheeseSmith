//
// Created by watson on 11/26/23.
//

#ifndef PACMAN_ENTITYVIEW_H
#define PACMAN_ENTITYVIEW_H

#include "../../logic/Observer.h"
#include "../../logic/EntityModel.h"

#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

namespace view {
    /**
     * "Abstract" Base Class
     */
    class EntityView : public obs::Observer {
    public:
        /**
         * Function to fetch the updated data form the Model and call projection/execution by the camera
         * @param et EntityModel to be viewed
         */
        void update(const em::EntityModel &et) const override;

        /**
         * Simple Destructor
         */
        ~EntityView() override = default;
    protected:
        /**
         * Keeps track of the 4 different options of sprites (for each direction)
         */
        std::vector<std::shared_ptr<sf::IntRect>> sprites;
    };

    /**
     * Class in between Pacman & Ghost since they share some functionality
     */
    class Mob: public EntityView{
    protected:
        /**
         * Bool deciding which of the 2 animation should be used
         */
        mutable bool animated{false};

        /**
         * Helper variable
         */
        mutable unsigned long duration{0};

        /**
         * Function to fetch the updated data form the Model and call projection/execution by the camera
         * @param et EntityModel to be viewed
         */
        void update(const em::EntityModel &et) const override;

        /**
         * Simple Destructor
         */
        ~Mob() override = default;
    };

    /**
     * Specialised viewer for Pacman
     */
    class Pacman : public Mob {
    public:
        /**
         * Constructor to initialise variables correctly
         */
        Pacman();

        /**
        * Function to fetch the updated data form the Model and call projection/execution by the camera. It also decides which animation should be used.
        * @param et Entity to fetch data from
        */
        void update(const em::EntityModel &et) const override;

        /**
         * Simple Destructor
         */
        ~Pacman() override = default;
    };

    /**
     * Specialised Class for the Ghost Entity
     */
    class Ghost : public Mob {
    public:
        /**
         * Constructor to initialise variables correctly
         */
        Ghost();

        /**
         * Function to fetch the updated data form the Model and call projection/execution by the camera. It also decides which animation should be used.
         * @param et Entity to fetch data from
         */
        void update(const em::EntityModel &et) const override;

        /**
         * Simple Destructor
         */
        ~Ghost() override = default;
    };

    /**
     * Specialised Class for the Fruit Collectable
     */
    class Fruit : public EntityView {
    public:
        /**
         * Constructor to initialise variables correctly
         */
        Fruit();

        /**
         * Simple Destructor
         */
        ~Fruit() override = default;
    };

    /**
     * Specialised Class for the Coin Collectable
     */
    class Coin : public EntityView {
    public:
        /**
         * Constructor to initialise variables correctly
         */
        Coin();

        /**
         * Simple Destructor
         */
        ~Coin() override = default;
    };

    /**
     * Specialised Class for the Wall Collectable
     */
    class Wall : public EntityView {
    public:
        /**
         * Constructor to initialise variables correctly
         */
        Wall();

        /**
         * Function to fetch the updated data form the Model and call projection/execution by the camera. Exaggerates the blue component harder.
         * @param et Entity to fetch data from
         */
        void update(const em::EntityModel &et) const override;

        /**
         * Simple Destructor
         */
        ~Wall() override = default;
    };
}

#endif //PACMAN_ENTITYVIEW_H
