//
// Created by watson on 11/22/23.
//

#include "Generator.h"

namespace rad{
    int Generator::generate(int lbound, int rbound) {
        std::uniform_int_distribution distribution{ lbound, rbound }; //Numbers to choose in between off
        return distribution(generator);
    }

    Generator::Generator() : generator(std::mt19937{static_cast<std::mt19937::result_type>( std::chrono::steady_clock::now().time_since_epoch().count())}) {} //Initialise it once per run as asked by the Project Assignment so we don't get the same sequence
}

