//
// Created by alefe on 09/06/2024.
//

#ifndef L_SYSTEMS_FOREST_H
#define L_SYSTEMS_FOREST_H

#include <vector>
#include "tree.h"

class Forest {
public:
    Forest(unsigned int n,
           unsigned int genome_size,
           unsigned int maturity,
           double mut_sub,
           double mut_dup,
           double mut_del);

    //! Evolutionary step.
    void evolve();

    void grow();

    void develop(unsigned int stage);

    //! Random weighted selection of a plant based on fitness.
    Tree &select_plant();

    void print_stats();

    std::vector<Tree> population;
    unsigned int maturity;
    double mut_sub;
    double mut_dup;
    double mut_del;
    double total_fitness = 0;
    Tree fittest_ever;
    Tree fittest_currently;
};


#endif //L_SYSTEMS_FOREST_H
