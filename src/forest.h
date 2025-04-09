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
    Forest(uint16_t n,
           uint16_t maturity,
           uint16_t genome_size,
           uint16_t max_genome_size,
           float mut_sub,
           float mut_dup,
           float mut_del,
           uint16_t gene_activation_length,
           std::mt19937 &rng);

    //! Creates an empty forest.
    Forest() = default;

    //! Evolutionary step.
    void evolve(std::mt19937 &rng);

    //! Selects a random tree from the population.
    Tree &randomTree(std::mt19937 &rng);

    //! Random weighted selection of a plant based on get_fitness.
    Tree &randomFitTree(std::mt19937 &rng);

    //! Print some stats about the population.
    void printStats();

    std::vector<Tree> population;
    std::optional<Tree> fittest_ever;
    float total_fitness = 0;

//    void saveFittest(const std::string &outdir) const;
//
//    void saveForest(const std::string &outdir) const;
};


#endif //L_SYSTEMS_FOREST_H
