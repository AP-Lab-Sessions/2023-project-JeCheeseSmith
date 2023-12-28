//
// Created by watson on 11/22/23.
//

#ifndef PACMAN_ENTITYMODEL_H
#define PACMAN_ENTITYMODEL_H

#include "Observer.h"
#include "Stopwatch.h"
#include "Score.h"

#include <memory>
#include <vector>
#include <iostream>
#include <cmath>

/**
 * Namespace used for describing the Entity Models
 */
namespace em {
    /**
     * Preset "Magical Power Numbers" so they can easily be changed or extended
     */
    static const short C = 1; /*!< Coin */
    static const short F = 2; /*!< Fruit */
    static const short fG = 3; /*!< fearedGhost */
    static const short P = 4; /*!< Pacman */
    static const short G = 5; /*!< Ghost */
    static const short W = 6; /*!< Wall */

    /**
     * Preset "Magical Numbers" for Directions so then can be easily changed or extended
     */
    static const short R = 0; /*!< Right */
    static const short D = 1; /*!<Down */
    static const short L = 2; /*!< Left */
    static const short U = 3; /*!< Up */

    /**
     * Simple structure to keep the coordinates of an Entity
     */
    struct Coordinate{
        /**
         * x value of the coordinate
         */
        double x;

        /**
         * y value of the coordinate
         */
        double y;

        /**
         * Simple constructor
         * @param x
         * @param y
         */
        Coordinate(const double &x,const double &y);

        /**
         * "Default" constructor; initialise values to 0.0
         */
        Coordinate();

        /**
         * Index constructor; Transform an Index or 2 integers to their respectable float coordinate
         * @param dx x coordinate in integer form
         * @param dy y coordinate in integer form
         */
        Coordinate(const int &dx,const int &dy);
    };

    /**
     * Base class for all Entities
     */
    class EntityModel {
    public:
        /**
         * Add an observer to the object
         */
        void addObserver(const std::shared_ptr<obs::Observer>& observer);

        /**
         * Function to re-evaluate/supply the new data for each connected Observer
         */
        void update() const;

        /**
         * Simple getter
         * @return power
         */
        [[nodiscard]] short getPower() const;

        /**
         * Simple constructor
         * @param power
         * @param pos
         */
        explicit EntityModel(const short &power,const em::Coordinate &pos);

        /**
         * Simple Destructor
         */
        virtual ~EntityModel();

        /**
         * Simple Getter
         * @return
         */
        [[nodiscard]] const em::Coordinate &getLocation() const;

         /**
         * "Move" the Entity towards a new location // "Setter"
         * @param p
         */
         virtual void move(const em::Coordinate &p);

        /**
         * Simple getter
         * @return
         */
        [[nodiscard]] int getDir() const;

        /**
         * Simple setter
         * @param Dir
         */
        void setDir(int Dir);
    protected:
        /**
         * Contains the value for its power, ability to eat the other, starting with the weakest (1-..): Coin, Fruit, Feared Ghost, Pacman, Ghost, Wall
         */
        short power;

        /**
          * Keeps track of the direction the object is facing
          * 0 = right, 1 = down, 2 = left, 3 = up
          */
        int dir;
    private:
        /**
         * List of observers
         */
        std::vector<std::shared_ptr<obs::Observer>> observers;

        /**
         * Describes the space took by the Entity in a normalised system ([-1,1])
         */
        Coordinate pos;
    };

    /**
     * Class to handle Pacman
     */
    class Pacman : public EntityModel {
    public:

        /**
         * Simple Constructor; Sets correct power
         */
        explicit Pacman(const em::Coordinate &pos);

        /**
         * Simple Destructor
         */
        ~Pacman() override = default;
    };

    /**
     * Specialised class for the Ghost Entity
     */
    class Ghost : public EntityModel {
    public:
        /**
         * Variables to control the direction switches
         */
        long lockt{0};
        bool locked{true};

        /**
         * Simple Getter
         * @return
         */
        [[nodiscard]] bool isChasing() const;

        /**
         * Switch to Fear Mode; also adjusts their power
         */
        void Feared();

        /**
         * Switch to Chasing Mode; also adjusts their power
         */
        void Chasing();

        /**
         * Simple Constructor; initialises correct power too
         */
        explicit Ghost(const em::Coordinate &pos,const long &fearTime,const long &delay);

        /**
         * Overwritten move function to autonomously handle Fear Mode, Chasing Mode & its Delay
         * @param p
         */
        void move(const em::Coordinate &p) final;

        /**
         * Simple Destructor
         */
        ~Ghost() override = default;
    private:
        /**
         * Defines Chasing or Fear Mode
         */
        mutable bool chasing;

        /**
         * Number of seconds the Ghost need to wait before starting their move operations
         */
        const long delay;

        /**
         * The time a Ghost stays in Fear Mode
         */
        const long fearTime;

        /**
         * Helper variable which we adjust to control the Fear Mode autonomously
         */
        long fearStart{};
    };

    /**
     * Class for the Wall Entity. Even though Walls are genuinely not seen as an Entity, it is useful to implement it this way by using the inheritance structure for more generic programming.
     */
    class Wall : public EntityModel {
    public:
        /**
         * Simple Constructor; Initialises correct Power
         */
        explicit Wall(const em::Coordinate &pos);

        /**
         * Simple Destructor
         */
        ~Wall() override = default;
    };

    /**
     * Specialised class for the Fruit entity
     */
    class Fruit : public EntityModel {
    public:
        /**
         * Simple Constructor; initialises power
         */
        explicit Fruit(const em::Coordinate &pos);

        /**
         * Simple Destructor
         */
         ~Fruit() override = default;
    };

    /**
     * Specialised class for the Coin Entity
     */
    class Coin : public EntityModel {
    public:
        /**
         * Simple Constructor; initialises power
         */
        explicit Coin(const em::Coordinate &pos);

        /**
         * Simple Destructor
         */
         ~Coin() override = default;
    };
}

#endif //PACMAN_ENTITYMODEL_H
