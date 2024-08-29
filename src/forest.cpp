//
// Created by alefe on 09/06/2024.
//

#include <iostream>
#include "forest.h"

Forest::Forest(
    unsigned int n,
    unsigned int genome_size,
    unsigned int maturity,
    double mut_sub,
    double mut_dup,
    double mut_del
) : maturity(maturity),
    mut_sub(mut_sub),
    mut_dup(mut_dup),
    mut_del(mut_del) {
    population.reserve(n);
    for (int i = 0; i < n; i++) {
        population.emplace_back(genome_size, maturity);
    }
    fittest_currently = population.front();
    fittest_ever = fittest_currently;
}

void Forest::develop(unsigned int stage) {
    for (auto &tree : population)
        tree.develop(stage);
}

void Forest::grow() {
    for (auto &tree : population)
        tree.grow();
}

void Forest::evolve() {
    develop(maturity);
    grow();
    for (auto &tree : population)
        tree.genome.mutate(mut_sub, mut_dup, mut_del);

    total_fitness = 0.;
    for (const auto &tree : population)
        total_fitness += tree.fitness();

    std::vector<Tree> new_population;
    for (int _ = 0; _ < population.size(); _++) {
        auto &tree = selectPlant();
        new_population.push_back(tree.germinate());

        if (tree.seeds.size() > fittest_currently.seeds.size())
            fittest_currently = tree;
        if (tree.seeds.size() > fittest_ever.seeds.size())
            fittest_ever = tree;
    }
    population = new_population;
}

Tree &Forest::selectPlant() {
    double rnd = total_fitness * uniform_random(rng);
    for (auto &tree: population) {
        if (rnd <= tree.fitness()) {
            return tree;
        }
        rnd -= tree.fitness();
    }
    // All plants have 0 fitness, just pick a random one
    return population[std::uniform_int_distribution<>(0, (int) population.size() - 1)(rng)];
}

void Forest::printStats() {
    unsigned int tot_gen_size = 0;
    for (const auto &tree : population)
        tot_gen_size += tree.genome.size();

    std::cout << "Mean fitness: " << total_fitness / (double) population.size() << "\n";
    std::cout << "Mean genome size: " << tot_gen_size / (double) population.size() << "\n";
}
