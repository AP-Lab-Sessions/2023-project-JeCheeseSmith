//
// Created by watson on 11/22/23.
//

#ifndef PACMAN_WORLD_H
#define PACMAN_WORLD_H

#include "EntityModel.h"
#include "AbstractFactory.h"
#include "Stopwatch.h"
#include "Generator.h"
#include "Score.h"

#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <memory>

/**
 * Namespace for everything related to the World (Controller - (Mostly) Model)
 */
namespace w{
    /**
     * Width and height of the board, used for movements. Made static for extendability & convenience.
     */
    static float w = 21.f;
    static float h = 11.f;

    /**
     * A couple of typedefs to keep the data easy accessible
     */
    template<typename T> using Board = std::vector<std::vector<T>>;

    /**
     * Main Class to manage the entities
     */
    class World {
    public:
        /**
         * Move pacman and the Ghosts into the desired direction given on input
         * @param dir 0 = right, 1 = down, 2 = left, 3 = up
         */
        void move(const int &dir);

        /**
         * Retrigger a general update of all observers
         */
        void update() const;

        /**
         * Construct the world and initiates its Entities
         * @param factory used to instantiate the entities
         * @param difficulty
         * @param hearts
         */
        World(const fact::AbstractFactory &factory, long difficulty, short hearts);

        /**
         * Gives true if we ran out of lives; GameOver
         * @return bool deciding the state
         */
        bool gameOver() const;

        /**
         * Simple getter: Get the current amount of lives
         * @return Number of leftover lives
         */
        short getHearts() const;

        /**
         * Check if all collectables are deleted
         * @return bool deciding the state
         */
        bool isCompleted() const;

        /**
         * Toggle game paused state
         * @param Paused
         */
        void setPaused(bool Paused);

        /**
         * Simple Destructor
         */
        virtual ~World();
    private:
        /**
         * Defines the game difficulty; indirectly used for the speed of Ghost, duration of the Fruity Effect (Fear Mode), ...
         */
        const unsigned long difficulty;

        /**
         * The playground of every entity
         */
        Board<std::unique_ptr<em::EntityModel>> board; //20 x 10

        /**
         * Position of our l'ille pacy!
         */
        Index pac;

        /**
         * Vector of used Ghosts
         */
        std::vector<Sprite> sprites;

        /**
         * Number of lives of Pacman
         */
        short hearts;

        /**
         * Variable which determines if all coins & fruits are eaten
         */
        mutable bool completed;

        /**
         * Bool which determines if the game should (not) do anything
         */
        bool paused;

        /**
        * Simple Helper function to remove redundant code, adds a row of Walls
        * @param factory used to create entities
        */
        void createWalls(const fact::AbstractFactory &factory);

        /**
         * Helper Function; Add rows of entities with walls at the first and last spot
         * @param factory used to create entities
         */
        void createBoard(const fact::AbstractFactory &factory);

        /**
         * Fill all spots with coins
         * @param factory used to create entities
         */
        void fillBoard(const fact::AbstractFactory &factory);

        /**
         * Helper Function to minimise redundant code
         * @param factory used to create entities
         * @param offset used to determine upper of down part of the maze
         */
        void addWalls(const fact::AbstractFactory &factory, bool offset);

        /**
         * Helper function; Returns true if a Ghost needs to change it's direction
         * @return
         */
        static bool changeDir(const std::vector<std::pair<Index,int>> &options);

        /**
         * Helper function; calculates the Manhattan distance between a Ghost & pacman
         * @param options Possible locations & direction to go to
         * @param chasing Determines if the distance needs to be minimised or maximised
         * @return
         */
        std::pair<Index,int> Manhattan(std::vector<std::pair<Index,  int>> &options, bool chasing) const;

        /**
         * Retrieves the possible positions & directions a ghost can go to
         * @param ghost start location & direction
         * @return
         */
        std::vector<std::pair<Index, int>> getIndices(Sprite &ghost) const;

        /**
         * Helper function to calculate the normal, real coordinates for an Entity given a specified direction
         * @param curr current Coordinate of the Entity
         * @param dir direction the Entity will go to / is facing
         * @param power strenght of the Entity
         * @return
         */
        std::pair<em::Coordinate,Index> computeCoordinate(const em::Coordinate &curr, int dir,short power) const;

        /**
         * General function to move all Ghosts
         */
        void moveGhosts();
    };
}

#endif //PACMAN_WORLD_H
