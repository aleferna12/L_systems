//
// Created by alefe on 09/06/2024.
//

#include <regex>
#include <iostream>
#include "forest.h"

Forest::Forest(
    uint16_t n,
    uint16_t maturity,
    uint16_t genome_size,
    uint16_t max_genome_size,
    float mut_sub,
    float mut_dup,
    float mut_del,
    uint16_t gene_activation_length,
    std::mt19937 &rng
) {
    population.reserve(n);
    for (uint16_t i = 0; i < n; i++) {
        population.emplace_back(
            Genome(
                genome_size,
                max_genome_size,
                mut_sub,
                mut_dup,
                mut_del,
                gene_activation_length,
                rng
            ),
            maturity,
            rng
        );
    }
}

void Forest::evolve(std::mt19937 &rng) {
    for (auto &tree : population) {
        tree.develop(tree.maturity);
        tree.grow();
        tree.reset_development();
    }

    total_fitness = 0.;
    for (const auto &tree : population) {
        total_fitness += tree.fitness;
    }

    std::vector<Tree> new_population;
    for (size_t _ = 0; _ < population.size(); _++) {
        new_population.push_back(randomFitTree(rng));
        auto &tree = new_population.back();

        if (!fittest_ever.has_value() || tree.fitness > fittest_ever.value().fitness)
            fittest_ever = tree;
        tree.genome.mutate(rng);
    }
    population = new_population;
    for (auto &tree : population) {
        tree.fitness = 0;
    }
}

Tree &Forest::randomTree(std::mt19937 &rng) {
    return population[std::uniform_int_distribution<>(0, (int16_t) population.size() - 1)(rng)];
}

Tree &Forest::randomFitTree(std::mt19937 &rng) {
    float rnd = total_fitness * uniform_random(rng);
    for (auto &tree: population) {
        if (rnd <= tree.fitness) {
            return tree;
        }
        rnd -= tree.fitness;
    }
    // All plants have 0 fitness, just pick a random one
    return randomTree(rng);
}

void Forest::printStats() {
    uint16_t tot_gen_size = 0;
    for (const auto &tree : population) {
        tot_gen_size += tree.genome.size();
    }

    std::cout << "Mean genome size: " << tot_gen_size / (float) population.size() << "\n";
    std::cout << "Mean get_fitness: " << total_fitness / (float) population.size() << "\n";
    if (fittest_ever.has_value())
        std::cout << "Best get_fitness: " << fittest_ever.value().fitness<< "\n";
}

//void Forest::saveFittest(const std::string &outdir) const {
//    if (!fittest_ever.has_value())
//        throw std::runtime_error("Forest does not have a fittest plant, "
//                                 "did you evolve the population at least once?");
//
//    auto fittest = fittest_ever.value();
//    fittest.develop(fittest.maturity);
//    fittest.grow();
//    std::ofstream file;
//
//    file.open(outdir + "/fittest_tree.tree");
//    file << fittest.asTREE();
//    file.close();
//
//    std::cout << "Saved information about fittest tree "
//                 "(get_fitness = " << fittest.fitness << ") to: '" << outdir << "'\n";
//}
//
//void Forest::saveForest(const std::string &outdir) const {
//    for (size_t i = 0; i < population.size(); i++) {
//        auto tree = population[i];
//        tree.develop(tree.maturity - tree.development_stage);
//        tree.grow();
//
//        std::ofstream file;
//        std::string basename = outdir + "/" + std::to_string(i) + "_";
//
//        file.open(basename + "tree.tree");
//        file << tree.asTREE();
//        file.close();
//    }
//    std::cout << "Saved forest to: '" << outdir << "'\n";
//}
