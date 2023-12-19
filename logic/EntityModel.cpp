//
// Created by watson on 11/22/23.
//

#include "EntityModel.h"

namespace em {

    void em::EntityModel::addObserver(const std::shared_ptr<obs::Observer>& observer) {
        observers.push_back(observer);
        update(); //Fetch the data to all observers
    }

    void em::EntityModel::update() const {
        for (const auto &obs: observers) {
            obs->update(*this);
        }
    }

    short em::EntityModel::getPower() const {
        return power;
    }

    em::EntityModel::EntityModel(const short &power, const em::Coordinate &pos) : power(power),dir(R),pos(pos) {
        addObserver( s::Singleton<obs::Score>::get() ); //Add Score
        update(); //Notify observers that the element is created and ready to be "viewed"
    }

    const em::Coordinate& em::EntityModel::getLocation() const {
        return pos;
    }

    void em::EntityModel::move(const em::Coordinate &p) {
        pos=p;
        //update(); //Notify Observers
    }

    int em::EntityModel::getDir() const {
        return dir;
    }

    void em::EntityModel::setDir(int Dir) {
        dir = Dir;
    }

    em::EntityModel::~EntityModel() {
        update(); //Give the latest update to observers
    }

    bool em::Ghost::isChasing() const {
        return chasing;
    }

    em::Ghost::Ghost(const em::Coordinate &pos,const long &fearTime,const long &delay) : EntityModel(G, pos),chasing(true),delay(delay), fearTime(fearTime) {}

    void Ghost::move(const Coordinate &p) {
        if( delay - (int) s::Singleton<sec::Stopwatch>::get()->getDuration() <0 ){ //Adjust waiting delay
            EntityModel::move(p);
        }
        if(s::Singleton<sec::Stopwatch>::get()->getDuration() - fearStart >= fearTime){ //Check if fearTime has passed
           Chasing(); //Get back to Chasing
        }
    }

    void Ghost::Feared() {
        chasing = false;
        power = fG;
        fearStart = s::Singleton<sec::Stopwatch>::get()->getDuration(); //Initialise fearStart
    }

    void Ghost::Chasing() {
        chasing = true;
        power = G;
    }

    em::Fruit::Fruit(const em::Coordinate &pos) : EntityModel(F, pos) {}

    em::Coin::Coin(const em::Coordinate &pos) : EntityModel(C, pos) {}

    em::Pacman::Pacman(const em::Coordinate &pos) : EntityModel(P, pos) {}

    em::Wall::Wall(const em::Coordinate &pos) : EntityModel(W, pos) {}

    em::Coordinate::Coordinate(const double &x,const double &y) : x(x), y(y) {}

    em::Coordinate::Coordinate() : x(0.0), y(0.0) {}

    Coordinate::Coordinate(const int &dx,const int &dy) {
        x = (double) dx/21; //Divide the index by the nr of indices
        y = (double) dy/11;
    }
}