//
// Created by watson on 11/22/23.
//

#include "Camera.h"
#include "EntityView.h"

namespace view {
    view::Camera::Camera() {
        if (!text.loadFromFile("representation/data/Sprites.png")) //Load the texture file
        {
            throw std::runtime_error("Sprites.png could not be loaded or found!");
        }
    }

    void view::Camera::project(const em::Coordinate &rect, const std::shared_ptr<sf::IntRect> &IntRect,
                               const std::unique_ptr<sf::Color> &col) {
        //Create Sprite
        sf::Sprite sprite;
        sprite.setTexture(text);
        sprite.setTextureRect(*IntRect);
        if (col != nullptr) {
            sprite.setColor(*col);
        }

        //Transform Sprite
        const float t = ((float) s::Singleton<view::View>::get()->getSize().x / w::w) / sprite.getLocalBounds().width ; //Normalised bord is 21x11 entities
        const float t1 = ((float) s::Singleton<view::View>::get()->getSize().y / w::h) / sprite.getLocalBounds().height;
        const float x1 = (float) rect.x * (float) s::Singleton<view::View>::get()->getSize().x;
        const float y1 = (float) rect.y * (float) s::Singleton<view::View>::get()->getSize().y;

        sprite.scale(t, t1); //Scale to normalized pixel format: 21x11
        sprite.setPosition(x1,y1); //Scale the normalized coordinate to the right pixel

        //Draw to the window
        s::Singleton<view::View>::get()->draw(sprite);
    }
}