//
// Created by alefe on 09/06/2024.
//

#include <stdexcept>
#include <iostream>
#include "environment.h"

Environment::Environment(
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

void Environment::develop(unsigned int stage) {
    for (auto &org : population)
        org.develop(stage);
}

void Environment::evolve() {
    develop(maturity);
    for (auto &org : population)
        org.mutate(mut_sub, mut_dup, mut_del);

    total_fitness = 0;
    for (const auto &org : population)
        total_fitness += org.fitness;

    std::vector<Plant> new_population;
    for (int _ = 0; _ < population.size(); _++) {
        auto &org = select_plant();
        new_population.push_back(org.germinate());

        if (org.fitness > fittest_currently.fitness)
            fittest_currently = org;
        if (org.fitness > fittest_ever.fitness)
            fittest_ever = org;
    }
    population = new_population;
}

Plant &Environment::select_plant() {
    double rnd = total_fitness * uniform_random(rng);
    for (auto &org: population) {
        auto fitness = org.fitness + 1;  // 0 causes problems with the weights
        if (rnd < fitness) {
            return org;
        }
        rnd -= fitness;
    }
    throw std::runtime_error("Selection criteria failed to pick plant");
}

void Environment::print_stats() {
    unsigned int tot_gen_size = 0;
    for (const auto &org : population)
        tot_gen_size += org.genome.size();

    std::cout << "Mean fitness: " << total_fitness / (double) population.size() << "\n";
    std::cout << "Mean genome size: " << tot_gen_size / (double) population.size() << "\n";
}
