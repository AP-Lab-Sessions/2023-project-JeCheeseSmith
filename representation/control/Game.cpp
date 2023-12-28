//
// Created by watson on 11/22/23.
//

#include "Game.h"

namespace play {
    bool Game::consistent() {
        return sos::FileExists(s::spritesPNG) && sos::FileExists(s::menuPNG)
               && sos::FileExists(s::pausedPNG) && sos::FileExists(s::gameOverJPG)
               && sos::FileExists(s::victoryPNG) && sos::FileExists(s::fontTTF);
    }

    Game::Game()  {
        REQUIRE(consistent(), "Program is inconsistent, abort"); //Check consistency
        s::Singleton<view::View>::get()->setVerticalSyncEnabled(true); //"create" view
        manager = std::make_shared<s::StateManager>();
        manager->push(std::make_unique<s::MenuState>(manager)); //construct & initialise StateManager + first state
        gameLoop(); //Start Game
    }

    void Game::handleEvents() {
        sf::Event event{};
        while (s::Singleton<view::View>::get()->pollEvent(event)) {  //As long as there is interaction with the screen/Game
            switch (event.type) { // check the type of the event
                case sf::Event::Closed: // view closed, end program, destruct
                    s::Singleton<view::View>::get()->close();
                    break;
                case sf::Event::KeyPressed: //If a key is pressed: delegate interaction process to States
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){ //Go to PausedState
                        manager->setCurr(sf::Keyboard::Key::Escape);
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
                        manager->setCurr(sf::Keyboard::Key::Left);
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
                        manager->setCurr(sf::Keyboard::Key::Right);
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
                        manager->setCurr(sf::Keyboard::Key::Up);
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
                        manager->setCurr(sf::Keyboard::Key::Down);
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
                        manager->setCurr(sf::Keyboard::Key::Enter);
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace)){
                        manager->setCurr(sf::Keyboard::Key::BackSpace);
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    manager->setCurr(sf::Keyboard::Key::Enter);
                    break;
                default:
                    break;
            }
        }
    }

    void Game::gameLoop() {
        while(s::Singleton<view::View>::get()->isOpen()) { //Keep interaction
            handleEvents(); //Handle events
            s::Singleton<sec::Stopwatch>::get()->restart(); //restart the clock
            manager->pressed(); //Update function
        }
    }

    Game::~Game() {
        manager.reset();
    }
}