//
// Created by alefe on 24/08/2024.
//

#ifndef L_SYSTEMS_PARAMS_H
#define L_SYSTEMS_PARAMS_H

const int GROWTH_STEPS = 13;
const int N_POP = 500;
const int GENS = 1000;
// Couldnt decide if this should be true of false.
// True leads to faster runtimes but lower fitness (maybe also tends to look cooler?).
const bool SEED_SKIPS = true;
const unsigned int COLLISION_PRECISION = 1000;
const double ROTATION_ANGLE = M_PI / 6;

#endif //L_SYSTEMS_PARAMS_H
