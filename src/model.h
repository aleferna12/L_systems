//
// Created by alefe on 08/09/2024.
//

#ifndef L_SYSTEMS_MODEL_H
#define L_SYSTEMS_MODEL_H

#include <cmath>
#include "parameters.h"
#include "forest.h"

class Model {
public:
    explicit Model(const Parameters &parameters);

    void run(unsigned int generations);

    void run() { run(parameters.generations); }

    Parameters parameters;
    Forest forest;

    std::mt19937 rng {std::random_device{}()};
};


#endif //L_SYSTEMS_MODEL_H
