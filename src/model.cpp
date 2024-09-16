//
// Created by alefe on 08/09/2024.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include "model.h"

Model::Model(const Parameters &parameters) :
    parameters(parameters),
    forest(
        parameters.n_pop,
        parameters.maturity,
        parameters.start_genome_size,
        parameters.max_genome_size,
        parameters.mut_sub_rate,
        parameters.mut_dup_rate,
        parameters.mut_del_rate,
        rng
    ) {
    if (!std::filesystem::create_directory(parameters.outdir)) {
        if (!parameters.replace_dir)
            throw std::runtime_error("Directory " + parameters.outdir + " already exists.");
        std::cerr << "WARNING: replacing files in output directory.\n";
    }

    // Handle optional parameters of trees and genomes
    for (auto &tree : forest.population) {
        tree.collision_precision = parameters.collision_precision;
        tree.rotation_angle = parameters.rotation_angle;
        tree.seed_skips = parameters.seed_skips;

        tree.genome.gene_activation_length = parameters.gene_activation_length;
        tree.genome.core_gene_substitution_chance = parameters.core_gene_substitution_chance;
    }
}

void Model::run(unsigned int generations) {
    for (unsigned int i = 0; i < generations; i++) {
        if (i % 100 == 0) {
            std::cout << "Generation: " << i << "\n";
            forest.printStats();
            std::cout << "\n";
        }

        forest.evolve(rng);
    }
}