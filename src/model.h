//
// Created by alefe on 08/09/2024.
//

#ifndef L_SYSTEMS_MODEL_H
#define L_SYSTEMS_MODEL_H

#include <cmath>
#include "pico/rand.h"
#include "parameters.h"
#include "forest.h"

class Model {
public:
    explicit Model(const Parameters &parameters);

    void run(unsigned short generations);

    void run() { run(parameters.generations); }

//    void saveData() const;

    Parameters parameters;
    std::mt19937 rng;
    Forest forest;
};


#endif //L_SYSTEMS_MODEL_H
