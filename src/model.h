//
// Created by alefe on 08/09/2024.
//

#ifndef L_SYSTEMS_MODEL_H
#define L_SYSTEMS_MODEL_H

#include <cmath>
#include "pico/rand.h"
#include "parameters.h"
#include "forest.h"
#include "graphics.h"

class Model {
public:
    explicit Model(const Parameters &parameters);

    void run(uint16_t generations);

    void run() { run(parameters.generations); }

    void draw_tree(const Tree &tree, float scale);

//    void saveData() const;

    Parameters parameters;
    std::mt19937 rng;
    Forest forest;
    Graphics graphics;
};


#endif //L_SYSTEMS_MODEL_H
