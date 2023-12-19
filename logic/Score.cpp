//
// Created by watson on 11/22/23.
//

#include "Score.h"

namespace obs{
    const std::string Score::location = "logic/output/score.txt"; //Static variables must be initialized outside the class

    void Score::exporter() const {
        std::sort(highscores.begin(), highscores.end());
        if(highscores[0]<score){ //Replace lowest highscore
            highscores[0]=score;
        }
        std::sort(highscores.begin(), highscores.end()); //Sort them again

        std::ofstream file(location);
        if(file.is_open()){ //Save to file
            for(const auto &point: highscores){
                file << std::to_string(point)+"\n";
            }
            file.close();
        }else{
            throw std::runtime_error("Couldn't write to "+location+" !");
        }
    }

    void Score::update(const em::EntityModel &et) const {
        if(et.getLocation().x<0){ //et is not used anymore and ready to be deleted
            switch(et.getPower()){ //Depending on the pow, the score is determined
                case em::C :{ //Coin
                    if(s::Singleton<sec::Stopwatch>::get()->getDuration() - lastCoinEaten<=0){
                        score+= 50;
                    }else{
                        score+= 50 / (int) (s::Singleton<sec::Stopwatch>::get()->getDuration() - lastCoinEaten);
                    }
                    lastCoinEaten = s::Singleton<sec::Stopwatch>::get()->getDuration(); //Refresh the time
                    break;
                }
                case em::F: //Fruit
                    score+=100;
                    break;
                case em::fG: //Feared Ghost
                    score+=200;
                    break;
                default: //Adding a default label for safety
                    break;
            }
            if(score>maxScore){
                score = maxScore;
            }
        }

        if(s::Singleton<sec::Stopwatch>::get()->getDuration() - duration == 1){ //Decrease the score every timespan
            score-=50;
            ++duration;
        }else if(s::Singleton<sec::Stopwatch>::get()->getDuration() - duration >=1){ //For some reason got out of sync; for example, after pausing
            duration = s::Singleton<sec::Stopwatch>::get()->getDuration()-1;
        }

        if(score<0){ //We don't want a negative score, spare our friends =)
            score = 0;
        }
    }

    Score::Score(): score(maxScore), duration(0), lastCoinEaten(0){
        REQUIRE(sos::FileExists(location), "score.txt could not be found!"); //Check up

        std::ifstream load(location); //Load saved scores
        std::string line;

        if(load.is_open()){
            while ( getline (load,line) ){
                highscores.push_back(stoi(line));
            }
            load.close();
        }else{
            throw std::runtime_error("Couldn't write to "+location+" !");
        }
    }

    const std::vector<int> &Score::getHighscores() const {
        std::sort(highscores.begin(), highscores.end()); //Make sure to sort it; "logical const"
        return highscores;
    }

    int Score::getScore() const {
        return score;
    }

    void Score::setScore(int scor) {
        Score::score = scor;
    }

    void Score::clearedLevel() const {
        score+=200;
        if(score>obs::maxScore){
            score = maxScore;
        }
    }
}