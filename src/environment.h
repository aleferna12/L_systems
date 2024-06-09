//
// Created by alefe on 09/06/2024.
//

#ifndef L_SYSTEMS_ENVIRONMENT_H
#define L_SYSTEMS_ENVIRONMENT_H

#include <vector>
#include "plant.h"

class Environment {
public:
    Environment(unsigned int n,
                unsigned int genome_size,
                unsigned int maturity,
                double mut_sub,
                double mut_dup,
                double mut_del);

    //! Evolutionary step.
    void evolve();

    //! Develops all plants to this stage. Useful for visualizing intermediate stages of development before evolution.
    void develop(unsigned int stage);

    //! Random weighted selection of a plant based on fitness.
    Plant &select_plant();

    void print_stats();

    std::vector<Plant> population;
    unsigned int maturity;
    double mut_sub;
    double mut_dup;
    double mut_del;
    unsigned int total_fitness = 0;
    Plant fittest_ever;
    Plant fittest_currently;
};


#endif //L_SYSTEMS_ENVIRONMENT_H
