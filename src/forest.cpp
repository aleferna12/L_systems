//
// Created by alefe on 09/06/2024.
//

#include <iostream>
#include <fstream>
#include "forest.h"

Forest::Forest(
    unsigned int n,
    unsigned int maturity,
    unsigned int genome_size,
    unsigned int max_genome_size,
    double mut_sub,
    double mut_dup,
    double mut_del,
    std::mt19937 &rng
) {
    population.reserve(n);
    for (unsigned int i = 0; i < n; i++) {
        population.emplace_back(
            Genome(
                genome_size,
                max_genome_size,
                mut_sub,
                mut_dup,
                mut_del,
                rng
            ),
            maturity,
            rng
        );
    }
}

void Forest::evolve(std::mt19937 &rng) {
    for (auto &tree : population)
        tree.develop();
    for (auto &tree : population)
        tree.grow();
    for (auto &tree : population)
        tree.genome.mutate(rng);

    total_fitness = 0.;
    for (const auto &tree : population)
        total_fitness += tree.fitness();

    std::vector<Tree> new_population;
    for (unsigned int _ = 0; _ < population.size(); _++) {
        auto &tree = randomFitTree(rng);
        new_population.push_back(tree.germinate());

        if (!fittest_currently.has_value() || tree.fitness() > fittest_currently.value().fitness())
            fittest_currently = tree;
        if (!fittest_ever.has_value() || tree.fitness() > fittest_ever.value().fitness())
            fittest_ever = tree;
    }
    population = new_population;
}

Tree &Forest::randomTree(std::mt19937 &rng) {
    return population[std::uniform_int_distribution<>(0, (int) population.size() - 1)(rng)];
}

Tree &Forest::randomFitTree(std::mt19937 &rng) {
    double rnd = total_fitness * uniform_random(rng);
    for (auto &tree: population) {
        if (rnd <= tree.fitness()) {
            return tree;
        }
        rnd -= tree.fitness();
    }
    // All plants have 0 fitness, just pick a random one
    return randomTree(rng);
}

void Forest::printStats() {
    unsigned int tot_gen_size = 0;
    for (const auto &tree : population)
        tot_gen_size += tree.genome.size();

    std::cout << "Mean genome size: " << tot_gen_size / (double) population.size() << "\n";
    std::cout << "Mean fitness: " << total_fitness / (double) population.size() << "\n";
    if (fittest_ever.has_value())
        std::cout << "Best fitness: " << fittest_ever.value().fitness() << "\n";
}

void Forest::save_fittest(const std::string &outdir) {
    if (!fittest_ever.has_value())
        throw std::runtime_error("Forest does not have a fittest plant, "
                                 "did you run the simulation for at least one generation?");

    auto fittest = fittest_ever.value();
    fittest.develop(fittest.maturity);

    std::ofstream file;

    file.open(outdir + "/fittest_body.txt");
    file << vecToStr(fittest.body, "") << "\n";
    file.close();

    file.open(outdir + "/fittest_genome.txt");
    file << fittest.genome.stringRepresentation();
    file.close();

    file.open(outdir + "/fittest_segments.obj");
    file << fittest.segmentsAsOBJ();
    file.close();

    file.open(outdir + "/fittest_seeds.obj");
    file << fittest.seedsAsOBJ();
    file.close();

    std::cout << "Saved information about fittest tree (fitness = " <<
    fittest.fitness() << ") to: '" << outdir << "'\n";
}