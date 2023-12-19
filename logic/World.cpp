//
// Created by watson on 11/22/23.
//

#include "World.h"

namespace w {
    World::World(const fact::AbstractFactory &factory, long difficulty, short hearts) :
                                                                                             difficulty(difficulty), hearts(hearts), completed(false), paused(false) {
        board.reserve(11);
        long fearTime;
        if(7 - difficulty < 0){
            fearTime = 0;
        }else{
            fearTime = 7 - difficulty;
        }
        createWalls(factory); //Add Walls above, first row
        createBoard(factory); //Add walls on the outer, rest are coins
        addWalls(factory,false); //Add walls on the upper board

        //Add some walls in the middle
        board[5][7] = factory.createWall(em::Coordinate(7,5));
        board[5][13] = factory.createWall(em::Coordinate(13,5));
        board[6][9] = factory.createWall(em::Coordinate(9,6));
        board[6][10] = factory.createWall(em::Coordinate(10,6));
        board[6][11] = factory.createWall(em::Coordinate(11,6));

        addWalls(factory,true); //Add walls on the down half of the board
        createWalls(factory); //The Last Row of walls

        //Set Collectables: Fruits & Coins
        board[1][1] = factory.createFruit(em::Coordinate(1,1));
        board[9][1] = factory.createFruit(em::Coordinate(1,9));
        board[1][19] = factory.createFruit(em::Coordinate(19,1));
        board[9][19] = factory.createFruit(em::Coordinate(19,9));
        fillBoard(factory); //Set Coins

        //Create mobs
        Sprite clyde{factory.createGhost(em::Coordinate(8,5),fearTime,10),{5,8}}; //Orange, super cool one!
        Sprite blinky{factory.createGhost(em::Coordinate(9,5),fearTime,0),{5,9} }; //Red one
        Sprite pinky{factory.createGhost(em::Coordinate(10,5),fearTime,0), {5,10}}; //Obviously Pink One
        Sprite inky{factory.createGhost(em::Coordinate(11,5),fearTime,5), {5,11}}; //Cyan one!
        sprites.push_back(std::move(clyde));
        sprites.push_back(std::move(blinky));
        sprites.push_back(std::move(pinky));
        sprites.push_back(std::move(inky));
        
        board[9][10] = factory.createPacman(em::Coordinate(10,9));
        pac = {9,10};
        board[0][0] = nullptr; //Upper left corner is for score
    }

    void World::move(const int &dir) { //0 = right, 1 = down, 2 = left, 3 = up
        if (paused) { //If in paused state: don't give the Ghosts a chance to kill Pacman; nor let pacman continue moving
            return;
        }

        board[pac.first][pac.second]->setDir(dir); //Also for animations
        auto data = computeCoordinate(board[pac.first][pac.second]->getLocation(), dir,
                                      board[pac.first][pac.second]->getPower()); //Coordinate we want to go to
        auto moveto = data.first;
        auto ix = data.second.second;
        auto iy = data.second.first;

        auto m = [&](){
            board[iy][ix] = std::move(board[pac.first][pac.second]); //Move the pacman object to new index
            pac = {iy, ix}; //Reload indices
            board[pac.first][pac.second]->move(moveto); //set pac's new coords
        };

        if (ix < 21 && iy < 11) {
            if (board[iy][ix] != nullptr) {
                if (board[iy][ix]->getPower() <=
                    board[pac.first][pac.second]->getPower()) { //Pac is stronger; it may move
                    if (board[iy][ix]->getPower() == em::F) { //Eating a fruit
                        for (auto &sprite: sprites) {
                            sprite.first->Feared(); //Set Fear Mode
                        }
                    }

                    board[iy][ix]->move(em::Coordinate(-1, -1)); //Set out of screen; also for score updates
                    m(); //Move it!
                }
            } else { //In case it is a nullptr, just move to that location since its freely movable
                m(); //Move it!
            }
        }

        moveGhosts();

        //Check Ghost Collision
        for (auto &ghost: sprites) {
            if (ghost.first == nullptr) {
                continue;
            }
            if (ghost.second == pac) { //If they're on the same location
                if (ghost.first->getPower() > board[pac.first][pac.second]->getPower()) { //Ghost is Stronger
                    --hearts;

                    board[9][10] = std::move(board[pac.first][pac.second]); //Move the pacman object to new index
                    pac = {9, 10};
                    board[pac.first][pac.second]->move(em::Coordinate(10, 9)); //set pac's new coords

                    //Reset location for all ghosts
                    for(auto &sprite: sprites){
                        sprite.first->move(em::Coordinate(9,5));
                        sprite.second = {5,9};
                    }
                    break; //We shouldn't check for an extra collision rn
                } else { //Feared Ghost
                    ghost.first->move(em::Coordinate(-1, -1)); //"Temp move away"; update score
                    ghost.first->Chasing(); //Set back to chasing
                    ghost.first->move(em::Coordinate(10, 5)); //Bring it back to the screen
                }
            }
        }
        update(); //Also renotify the drawings of all coins
    }

    void World::update() const {
        completed = true;
        for(const auto &vec: board){
            for(const auto &et: vec){
                if(et!= nullptr){
                    if(et->getPower()==em::C){ //Found a coin which hasn't been eaten yet
                        completed = false;
                    }
                    et->update();
                }
            }
        }
        board[pac.first][pac.second]->update();
        for(const auto& sprite: sprites){
            sprite.first->update();
        }
    }

    void World::moveGhosts() {
        for(auto& ghost: sprites){
            std::pair<Index,int> togo;
            auto options = getIndices(ghost);

            if(options.empty()){ //In case it could nowhere; just return
                continue;
            }

            if(!ghost.first->isChasing()){ //If in fear mode: Ghost Move slower
                int rbound = 10 - (int) difficulty;
                if(rbound<0){
                    rbound = 0;
                }
                if(rbound == s::Singleton<rad::Generator>::get()->generate(0, rbound)){ //Slower Movement = There's a chance they don't move then
                    return;
                }
            }

            if(changeDir(options)){ //Check if we need to change the direction
                int pick = s::Singleton<rad::Generator>::get()->generate(0, (int) options.size()-1);
                togo = options[pick];
            }else{ //Move to extreme Manhattan distance
                togo = Manhattan(options,ghost.first->isChasing());
            }

            //Implement some controlled delay for the Direction change
            if(s::Singleton<sec::Stopwatch>::get()->getDuration() - ghost.first->lockt == 1){
                ghost.first->locked = !ghost.first->locked; //Switch lock
                ++ghost.first->lockt;
            }else if(s::Singleton<sec::Stopwatch>::get()->getDuration() - ghost.first->lockt >1){ //Reset the value if it went out of sync
                ghost.first->locked = !ghost.first->locked; //Switch lock
                ghost.first->lockt = s::Singleton<sec::Stopwatch>::get()->getDuration();
            }
            if(ghost.first->locked) {
                ghost.first->setDir(togo.second); //Set direction
            }

            //Move Ghost
            auto data = computeCoordinate(ghost.first->getLocation(),ghost.first->getDir(), ghost.first->getPower()); //Coordinate we want to go
            auto moveto = data.first;
            int ix = data.second.second;
            int iy = data.second.first;

            if( (board[iy][ix]!= nullptr) && (board[iy][ix]->getPower() > ghost.first->getPower())) { //If it is a wall, don't go there
                return;
            }
            ghost.second = data.second;
            ghost.first->move(moveto);
        }
    }

    std::pair<Index, int> World::Manhattan(std::vector<std::pair<Index, int>> &options, bool chasing) const {
        std::vector<int> distances{};
        std::vector<std::pair<Index,int>> new_options{};
        int extreme;

        for(const auto& i: options){
            int d = abs(pac.first - i.first.first) + abs(pac.second -   i.first.second);
            distances.emplace_back(d);
        }

        if(chasing){
            extreme = *std::min_element(distances.begin(), distances.end());
        }else{
            extreme = *std::max_element(distances.begin(), distances.end());
        }

        for(const auto &i: options) {
            int d = abs(pac.first -   i.first.first) + abs(pac.second -   i.first.second);
            if(d==extreme){
                new_options.emplace_back(i);
            }
        }

        int rbound = (int) new_options.size()-1;
        if(rbound<0){
            rbound = 0;
        }
        const int random = s::Singleton<rad::Generator>::get()->generate(0, rbound);
        return new_options[random];
    }

    std::vector<std::pair<Index, int>> World::getIndices(Sprite &ghost) const {
        std::vector<std::pair<Index, int>> options;

        for(int dir = 0; dir<4; ++dir) {
            int ix = ghost.second.second; //The First pair is curr coordinate
            int iy = ghost.second.first;

            switch (dir) {
                case em::R: //Right
                    ++ix;
                    break;
                case em::D: //Down
                    ++iy;
                    break;
                case em::L: //Left
                    --ix;
                    break;
                case em::U: //Up
                    --iy;
                    break;
                default: //Shouldn't be used; is here for safety
                    break;
            }

            if(board[iy][ix]== nullptr || (board[iy][ix]->getPower() < ghost.first->getPower())) { //If it is not a wall or fear (collectable, nullptr); free to move here
                options.push_back({{iy, ix}, dir});
            }
        }
        return options;
    }

    std::pair<em::Coordinate,Index> World::computeCoordinate(const em::Coordinate &curr, int dir, short power) const {
        long ix{0}; //Index x
        long iy{0}; //Index y
        em::Coordinate moveto;
        float deltatime = s::Singleton<sec::Stopwatch>::get()->restart();

        if(power==em::G){ //If it's a ghost, adjust the speed based on the difficulty
            deltatime += (float) difficulty * deltatime;
        }

        switch(dir) {
            case em::R: {
                ix = std::ceil( (curr.x + (1.f/w::w)* deltatime) * w );
                iy = std::lround(curr.y* h) ;
                moveto = em::Coordinate(curr.x + ((1.f / w) * deltatime), (float) iy/h);
                break;
            }
            case em::D: {
                ix = std::lround( curr.x * w) ;
                iy = std::ceil( (curr.y + (1.f/h)*deltatime) * h);
                moveto = em::Coordinate( (float) ix/w, curr.y + ((1.f/h) *deltatime));
                break;
            }
            case em::L: {
                ix = std::floor( (curr.x - (1.f/w)*deltatime) * w );
                iy = std::lround( curr.y* h);
                moveto = em::Coordinate( curr.x - ( (1.f/w)*deltatime), (float) iy/h); //Multiply by deltaTime
                break;
            }
            case em::U: {
                ix = std::lround( curr.x  * w) ;
                iy = std::floor( (curr.y - (1.f/h)* deltatime)  *h);
                moveto = em::Coordinate( (float) ix/w, curr.y - ((1.f/h) *deltatime)) ; //Correct to initial index-value, Multiply by deltaTime
                break;
            }
            default: //Should not come true; implemented for safety
                break;
        }
        return {moveto, {iy,ix}};
    }

    bool World::gameOver() const {
        if(hearts<=0){
            return true;
        }
        return false;
    }

    short World::getHearts() const {
        return hearts;
    }

    bool World::isCompleted() const {
        return completed;
    }

    void World::setPaused(bool Paused) {
        paused = Paused;
    }

    bool World::changeDir(const std::vector<std::pair<Index, int>> &options){
        if(s::Singleton<rad::Generator>::get()->generate(0,1) == 0){ //50% Chance it will NOT change its direction
            return false;
        }else{
            switch(options.size()){ //It may change its direction; check if that's possible
                case 0: //Should always have 1 option to move to
                case 1: //Reached a triple corner
                    return false; //Don't change the direction, use Manhattan Distances
                case 2: //Tunnel or Corner: Decide what to do
                    if( (options[0].second == 2 && options[1].second == 0) || (options[0].second == 0 && options[1].second == 2) || (options[0].second == 3 && options[1].second == 1) || (options[0].second == 1 && options[1].second == 3) ){ //Tunnel: {right,left} , {down,up}
                        return false;
                    }else{
                        return true;
                    }
                default: //3 or 4: intersection
                    return true;
            }
        }
    }

    void World::createWalls(const fact::AbstractFactory &factory) {
        const int x = (int) board.size();
        std::vector<std::unique_ptr<em::EntityModel>> temp; //Temp variable
        temp.reserve(21);
        for (int j = 0; j < 21; ++j) { //For the lengt of the normalized board
            temp.emplace_back(factory.createWall(em::Coordinate(j,x)));//Add a line of walls, use of std::move since we use Unique ptrs
        }
        board.emplace_back(std::move(temp));
    }

    void World::createBoard(const fact::AbstractFactory &factory){
        for (int i = 1; i < 10; ++i) { //For the height of the normalized board (minus first and last)
            std::vector<std::unique_ptr<em::EntityModel>> temp; //Temp variable
            temp.reserve(21);
            temp.emplace_back(factory.createWall(em::Coordinate(0,i))); //Each row starts with a wall
            for (int j = 1; j < 20; ++j) { //For the lengt of the normalized board
                temp.emplace_back(nullptr); //We will substitute this later with coins
            }
            temp.emplace_back(factory.createWall(em::Coordinate(20,i)));//The Last one is a wall
            board.emplace_back(std::move(temp));
        }
    }

    void World::addWalls(const fact::AbstractFactory &factory, bool offset){
        int i,j;
        if(offset){
            i = 9;
            j = -1;
        }else{
            i = 1;
            j =1;
        }

        //I add this all manually because I'm doubting if a function to minimize the lines of code would have an actual performance issue

        //Create 2nd/ 7th Row - Replace some coins
        board[i][5] = factory.createWall(em::Coordinate(5,i));
        board[i][15] = factory.createWall(em::Coordinate(15,i));

        //Create 3rd / 8th Row - Replace some coins
        i+=j;
        board[i][2] = factory.createWall(em::Coordinate(2,i));
        board[i][3] = factory.createWall(em::Coordinate(3,i));
        board[i][5] = factory.createWall(em::Coordinate(5,i));

        board[i][7] = factory.createWall(em::Coordinate(7,i));
        board[i][8] = factory.createWall(em::Coordinate(8,i));
        board[i][9] = factory.createWall(em::Coordinate(9,i));
        board[i][10] = factory.createWall(em::Coordinate(10,i));
        board[i][11] = factory.createWall(em::Coordinate(11,i));
        board[i][12] = factory.createWall(em::Coordinate(12,i));
        board[i][13] = factory.createWall(em::Coordinate(13,i));

        board[i][15] = factory.createWall(em::Coordinate(15,i));
        board[i][17] = factory.createWall(em::Coordinate(17,i));
        board[i][18] = factory.createWall(em::Coordinate(18,i));

        //Create 4th / 9th Row - Replace some coins
        i+=j;
        board[i][2] = factory.createWall(em::Coordinate(2,i));
        board[i][18] = factory.createWall(em::Coordinate(18,i));

        //Create 5th Row / 10th - Replace some coins
        i+=j;
        board[i][2] = factory.createWall(em::Coordinate(2,i));
        board[i][4] = factory.createWall(em::Coordinate(4,i));
        board[i][5] = factory.createWall(em::Coordinate(5,i));
        board[i][7] = factory.createWall(em::Coordinate(7,i));
        board[i][8] = factory.createWall(em::Coordinate(8,i));
        board[i][12] = factory.createWall(em::Coordinate(12,i));
        board[i][13] = factory.createWall(em::Coordinate(13,i));
        board[i][15] = factory.createWall(em::Coordinate(15,i));
        board[i][16] = factory.createWall(em::Coordinate(16,i));
        board[i][18] = factory.createWall(em::Coordinate(18,i));
    }

    void World::fillBoard(const fact::AbstractFactory &factory) {
        for (int i = 1; i < 10; ++i) { //For the height of the normalized board (minus first and last)
            for (int j = 1; j < 20; ++j) { //For the lengt of the normalized board
                if (board[i][j] == nullptr) {
                    if( i==5 || i==4 ){
                        switch(j) {
                            case 8: //Do nothing; needs to stay void
                            case 9: //Do nothing; needs to stay void
                            case 10: //Do nothing; needs to stay void
                            case 11: //Do nothing; needs to stay void
                            case 12: //Do nothing; needs to stay void
                                break;
                            default:
                                board[i][j] = factory.createCoin(em::Coordinate(j, i)); //Add coin
                                break;
                        }
                    }else{
                        board[i][j] = factory.createCoin( em::Coordinate(j,i) );
                    }
                }
            }
        }
    }

    World::~World() {
        if(isCompleted()){
            s::Singleton<obs::Score>::get()->clearedLevel(); //Add Bonus points
        }
    }
}
