//
// Created by watson on 11/26/23.
//

#include "EntityView.h"
#include "Camera.h"

namespace view {
    void EntityView::update(const em::EntityModel &et) const {
        const auto ptr = sprites[0]; //Default case; there's online 1 corresponding image
        s::Singleton<Camera>::get()->project(et.getLocation(),ptr);
    }

    Pacman::Pacman() {
        //Initialises sprites
        sprites.push_back( std::make_shared<sf::IntRect>(850, 54, 40, 40)); //Right
        sprites.push_back( std::make_shared<sf::IntRect>(850, 204, 40, 40)); //Down
        sprites.push_back( std::make_shared<sf::IntRect>(850, 354, 40, 40)); //Left
        sprites.push_back( std::make_shared<sf::IntRect>(850, 504, 40, 40));//Up
        sprites.push_back(std::make_shared<sf::IntRect>(850,4,40,40)); //To make the animation, you need the Full one
    }

    void Pacman::update(const em::EntityModel &et) const {
        std::shared_ptr<sf::IntRect> ptr;
        Mob::update(et); //Decide the move animation

        if(animated){
            ptr = sprites[et.getDir()];
        }else{
            ptr = sprites[4];
        }
        s::Singleton<Camera>::get()->project(et.getLocation(),ptr); //Call Projection/draw to window
    }

    Ghost::Ghost() {
        //Order of sprites: right down left up right down left up
        sprites.push_back( std::make_shared<sf::IntRect>(50, 4, 40, 40));
        sprites.push_back( std::make_shared<sf::IntRect>(50, 54, 40, 40));
        sprites.push_back( std::make_shared<sf::IntRect>(50, 104, 40, 40));
        sprites.push_back( std::make_shared<sf::IntRect>(50, 4, 40, 40));

        sprites.push_back( std::make_shared<sf::IntRect>(50, 154, 40, 40));
        sprites.push_back( std::make_shared<sf::IntRect>(50, 204, 40, 40));
        sprites.push_back( std::make_shared<sf::IntRect>(50, 254, 40, 40));
        sprites.push_back( std::make_shared<sf::IntRect>(50, 304, 40, 40));

        //Feared Sprites
        sprites.push_back( std::make_shared<sf::IntRect>(0, 854, 40, 40));
        sprites.push_back( std::make_shared<sf::IntRect>(0, 804, 40, 40));
    }

    void Ghost::update(const em::EntityModel &et) const{
        std::shared_ptr<sf::IntRect> ptr;
        Mob::update(et);

        //Decide the move animation
        if(et.getPower()==em::fG){ //Feared
            ptr = sprites[8 + animated*1];
        }else{
            ptr = sprites[et.getDir() + animated*4];
        }
        s::Singleton<Camera>::get()->project(et.getLocation(),ptr); //Call projection/draw to window
    }

    Fruit::Fruit() {
        sprites.push_back(std::make_shared<sf::IntRect>(600, 4, 40, 40)); //Cherry
    }

    Coin::Coin() {
        sprites.push_back(std::make_shared<sf::IntRect>(400, 304, 40, 40)); //Coin somewhere
    }

    Wall::Wall() {
        sprites.push_back(std::make_shared<sf::IntRect>(114, 5, 4, 4)); //Random Blue Part of a Ghost to make walls
    }

    void Wall::update(const em::EntityModel &et) const {
        const auto ptr = sprites[0]; //Default case; there's online 1 corresponding image
        const auto col = std::make_unique<sf::Color>(0,0,255);
        s::Singleton<Camera>::get()->project(et.getLocation(),ptr,col);
    }

    void Mob::update(const em::EntityModel &et) const {
        if(s::Singleton<sec::Stopwatch>::get()->getDuration() - duration == 1){
            animated = !animated; //Switch animation
            ++duration;
        }else if( s::Singleton<sec::Stopwatch>::get()->getDuration() - duration > 1){ //If it went out of sync, adjust
            duration = s::Singleton<sec::Stopwatch>::get()->getDuration() - 1;
        }
    }
}