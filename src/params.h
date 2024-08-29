//
// Created by alefe on 24/08/2024.
//

#ifndef L_SYSTEMS_PARAMS_H
#define L_SYSTEMS_PARAMS_H

#include <cmath>

const int GROWTH_STEPS = 13;
const int N_POP = 500;
const int GENS = 1000;
// TODO: decide if this should be true of false (i dont think it should be a parameter but maybe).
// True leads to faster runtimes but lower fitness (maybe also tends to look cooler?).
const bool SEED_SKIPS = false;
const unsigned int MAX_GENOME_SIZE = 100;
// Make sure to balance this with GROWTH_STEPS otherwise you wont have enough memory
const unsigned int GENE_ACTIVATION_LENGTH = 2;  // Usually 2 or 3
const double CORE_GENE_SUBSTITUTION = 0.5;
const unsigned int COLLISION_PRECISION = 1000;
const double ROTATION_ANGLE = M_PI / 6;

#endif //L_SYSTEMS_PARAMS_H
