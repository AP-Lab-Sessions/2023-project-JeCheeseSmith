//
// Created by watson on 11/22/23.
//

#include "States.h"

namespace s {
    void StateManager::pressed()  {
        stateStack.top()->pressed(curr); //Delegate the task to the corresponding state
    }

    void StateManager::push(std::unique_ptr<State> s) {
         stateStack.push(std::move(s)); //Always move it; unique ptr
    }

    void StateManager::pop(int times) {
        for (int i = 0; i < times; ++i) {
            stateStack.pop();
        }
        //stateStack.top()->update();
    }

    long StateManager::getDifficulty() const {
        return difficulty;
    }

    void StateManager::incDifficulty() {
        ++difficulty;
    }

    StateManager::~StateManager() {
        while(!stateStack.empty()){
            stateStack.pop();
        }
    }

    short StateManager::getHearts() const {
        return hearts;
    }

    void StateManager::setHearts(short nHearts) {
        StateManager::hearts = nHearts;
    }

    void StateManager::replace(std::unique_ptr<State> s) {
        this->pop(2);
        stateStack.push(std::move(s));
    }

    void StateManager::setCurr(sf::Keyboard::Key Curr) {
        StateManager::curr = Curr;
    }

    const sf::Font &StateManager::getFont() const {
        return font;
    }

    StateManager::StateManager() {
        if (!font.loadFromFile("representation/data/BlueStyle.ttf")) { //Load Font
            throw std::runtime_error("Couldn't load font!");
        }
    }

    void StateManager::setDifficulty(long diff) {
        StateManager::difficulty = diff;
    }

    State::State(StateManager &manager): manager(manager) {}

    void State::loadScreen(const std::string &path,bool clear) {
        sf::Texture bg; //Load texture
        if(!bg.loadFromFile(path))
        {
            throw std::runtime_error("Could not open this particular loading screen");
        }

        sf::Sprite Sprite;
        Sprite.setTexture(bg); //set texture on sprite
        Sprite.setPosition(0,0);
        Sprite.scale((float) s::Singleton<view::View>::get()->getSize().x / Sprite.getLocalBounds().width, (float) s::Singleton<view::View>::get()->getSize().y / Sprite.getLocalBounds().height ); //Scale texture depending on screen resolution
        if(clear){
            s::Singleton<view::View>::get()->clear(); //Clear current bs
        }
        s::Singleton<view::View>::get()->draw(Sprite); //Draw it
        s::Singleton<view::View>::get()->display();
    }

    void State::loadText(const std::string &str, bool center) {
        sf::Text text;
        text.setFont(manager.getFont());
        text.setString(str);
        text.setCharacterSize( std::floor(s::Singleton<view::View>::get()->getSize().y / 44));
        text.setFillColor(sf::Color::Yellow);

        if (center) { //Center the data if requested
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width / 2.0f,
                           textRect.top + textRect.height / 2.0f);
            text.setPosition(s::Singleton<view::View>::get()->getView().getCenter());
        }
        s::Singleton<view::View>::get()->draw(text);
    }

    MenuState::MenuState(StateManager &manager): State(manager) {
        update();
    }

    void MenuState::pressed(sf::Keyboard::Key &curr)  {
        if(curr==sf::Keyboard::Key::Enter){
            s::Singleton<obs::Score>::get()->setScore(obs::maxScore); //Reset the value
            manager.push(std::make_unique<s::LevelState>(manager)); //Push back
        }else{
            update();
        }
    }

    void MenuState::update() {
        s::Singleton<view::View>::get()->clear(); //Clear current stuff

        //Create String
        std::string menu = "Highscores:\n";
        std::shared_ptr<obs::Score> s = s::Singleton<obs::Score>::get(); //Initialise it here, so it will not go out of scope for all the levels
        for(auto it = --(s->getHighscores().end()); it!=s->getHighscores().begin(); --it){
            menu+= std::to_string(*it)+"\n";
        }
        menu+= std::to_string(*s->getHighscores().begin())+"\n"; //Add the last/First one

        loadText(menu); //Load Scores
        loadText("Click here to start!",true); //Create Play-Button
        s::Singleton<view::View>::get()->display(); //Display newly drawn items
    }

    LevelState::LevelState(StateManager &manager) : State(
            manager), world(fact::ViewFactory(),manager.getDifficulty(),manager.getHearts()) {
        s::Singleton<view::View>::get()->clear(); //Clear current stuff

        //Create World in view

        //Set score on screen
        loadText( {"Score\n"+std::to_string(s::Singleton<obs::Score>::get()->getScore())} );

        s::Singleton<view::View>::get()->display(); //Display newly drawn items
    }

    void LevelState::pressed(sf::Keyboard::Key &curr) { //0 = right, 1 = down, 2 = left, 3 = up
        s::Singleton<view::View>::get()->clear();

        loadText("Score:\n" + std::to_string(s::Singleton<obs::Score>::get()->getScore()) + "\nLifes: " +
                         std::to_string(world.getHearts()));

        world.setPaused(false); //This is not so pretty to call each time, but it works
        switch (curr) {
            case sf::Keyboard::Key::Escape: {
                world.setPaused(true);
                manager.push(std::make_unique<s::PausedState>(manager)); //Push back Paused State
                return; //Don't go further
            }
            case sf::Keyboard::Key::Left:
                world.move(em::L);
                break;
            case sf::Keyboard::Key::Right:
                world.move(em::R);
                break;
            case sf::Keyboard::Key::Up:
                world.move(em::U);
                break;
            case sf::Keyboard::Key::Down:
                world.move(em::D);
                break;
            default:
                update(); //We need to update all data, otherwise will get a black screen
                break;
        }
        s::Singleton<view::View>::get()->display(); //Display newly drawn items

        if(world.gameOver()) {
            world.setPaused(true); //Pause the game so the score will be preserved
            manager.push(std::make_unique<s::GameOverState>(manager)); //Push back
        }else if (world.isCompleted()) {
            world.setPaused(true); //Pause the game so the score will be preserved
            manager.setHearts(world.getHearts());
            manager.push(std::make_unique<s::VictoryState>(manager)); //Push back
        }
    }

    void LevelState::update() {
        world.update();
    }

    PausedState::PausedState(StateManager &manager)
            : State(manager) {
        update();
    }

    void PausedState::pressed(sf::Keyboard::Key &curr)  {
        switch(curr){
            case sf::Keyboard::Key::Enter: //Pop Itself if a click or enter is registered to continue
                manager.pop();
                break;
            case sf::Keyboard::Key::BackSpace:
                manager.pop(2); //Pop the level and itsself
                break;
            default:
                break;
        }
    }

    void PausedState::update() {
        loadScreen("representation/data/Paused.png"); //Create Pause Menu
        loadText("Press Esc to go to Menu\n Press any other key to return to game"); //Display some text
    }

    VictoryState::VictoryState(StateManager &manager)
            : State(manager) {
        update();
    }

    void VictoryState::pressed(sf::Keyboard::Key &curr)  {
        if(curr == sf::Keyboard::Key::BackSpace){ //If we want to continue
            manager.incDifficulty(); //Make the game a little harder
            manager.replace( std::make_unique<s::LevelState>(manager) ); //Push back, pop 2 times (level & this) //Create new lvl state
        }
    }

    void VictoryState::update() {
        loadScreen("representation/data/Victory.png"); //Window.load Victory Menu
    }

    GameOverState::GameOverState(StateManager &manager)
            : State(manager) {
        s::Singleton<obs::Score>::get()->exporter(); //Score may only be exported on game Ending
        loadScreen("representation/data/GameOver.jpg"); //Load GameOver Screen
    }

    void GameOverState::pressed(sf::Keyboard::Key &curr)  {
        if(curr == sf::Keyboard::Key::Escape){ //If we want to leave/continue
            manager.setHearts(3); //Reset lives
            manager.setDifficulty(0); //Reset diff
            manager.pop(2); //Pop the failed level & itself
            loadScreen("representation/data/Menu.png"); //Load Menu
        }
    }

    void GameOverState::update() {
        loadScreen("representation/data/Menu.png"); //Load Menu
    }
}