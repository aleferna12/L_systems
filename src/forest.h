//
// Created by alefe on 09/06/2024.
//

#ifndef L_SYSTEMS_FOREST_H
#define L_SYSTEMS_FOREST_H

#include <vector>
#include "tree.h"

class Forest {
public:
    //! Creates a forest and populate it with 'n' trees.
    Forest(unsigned int n,
           unsigned int maturity,
           unsigned int genome_size,
           unsigned int max_genome_size,
           double mut_sub,
           double mut_dup,
           double mut_del,
           std::mt19937 &rng);

    //! Creates an empty forest.
    Forest() = default;

    //! Evolutionary step.
    void evolve(std::mt19937 &rng);

    //! Selects a random tree from the population.
    Tree &randomTree(std::mt19937 &rng);

    //! Random weighted selection of a plant based on fitness.
    Tree &randomFitTree(std::mt19937 &rng);

    //! Print some stats about the population.
    void printStats();

    std::vector<Tree> population;
    std::optional<Tree> fittest_ever;
    std::optional<Tree> fittest_currently;
    double total_fitness = 0;

    void save_fittest(const std::string &outdir);
};


#endif //L_SYSTEMS_FOREST_H
