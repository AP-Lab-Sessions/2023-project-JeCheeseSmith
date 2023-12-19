//
// Created by watson on 11/22/23.
//

#ifndef PACMAN_CAMERA_H
#define PACMAN_CAMERA_H

#include "View.h"
#include "../../logic/World.h"

#include <SFML/Graphics.hpp>
#include <memory>

/**
 * Forward Declaration
 */
namespace em{
    struct Coordinate;
}

/**
 * Namespace used for everything related to displaying entities
 */
namespace view {
    /**
     * Implemented as Singleton
     */
    class Camera {
    public:
        /**
         * Deleted Constructors
         */
        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;

        /**
         * Project members to the view/window
         */
        void project(const em::Coordinate &rect,const std::shared_ptr<sf::IntRect> &IntRect, const std::unique_ptr<sf::Color>& col= nullptr);

        /**
         * Simple Destructor
         */
        virtual ~Camera() = default;

        /**
         * Make sure the Singleton has acces to the Constructor
         */
        friend class s::Singleton<Camera>;
    private:
        /**
         * Used texture
         */
        mutable sf::Texture text;

        /**
         * Private constructor to enforce singleton
         */
        explicit Camera();
    };
}

#endif //PACMAN_CAMERA_H
