//
// Created by alefe on 08/09/2024.
//

#include <algorithm>
#include <iostream>
#include "pico/rand.h"
#include "model.h"

Model::Model(const Parameters &parameters) :
    parameters(parameters),
    rng(parameters.seed == 0 ? get_rand_32() : parameters.seed),
    forest(
        parameters.n_pop,
        parameters.maturity,
        parameters.start_genome_size,
        parameters.max_genome_size,
        parameters.mut_sub_rate,
        parameters.mut_dup_rate,
        parameters.mut_del_rate,
        parameters.gene_activation_length,
        rng
    ),
    graphics(
        320,
        240,
        pimoroni::ROTATE_90
    ) {
//    if (!std::filesystem::create_directory(parameters.outdir)) {
//        if (!parameters.replace_dir)
//            throw std::runtime_error("Directory " + parameters.outdir + " already exists.");
//        std::cerr << "WARNING: replacing files in output directory.\n";
//    }

    // Handle optional parameters of trees and genomes
    for (auto &tree : forest.population) {
        tree.collision_precision = parameters.collision_precision;
        tree.rotation_angle = parameters.rotation_angle;
        tree.seed_skips = parameters.seed_skips;

        tree.genome.gene_activation_length = parameters.gene_activation_length;
        tree.genome.core_gene_substitution_chance = parameters.core_gene_substitution_chance;
    }

    // TODO: make parameter
    graphics.screen.set_backlight(255);
}

void Model::run(const uint16_t generations) {
    for (uint16_t i = 0; i < generations; i++) {
        if (i % 100 == 0) {
            std::cout << "Generation: " << i << "\n";
            forest.printStats();
            std::cout << "\n";
            print_memory_info();
            std::cout << "\n";

            if (i == 0)
                continue;
            // TODO: remove from here
            graphics.clear();
            Tree t = forest.fittest_ever.value();
            t.develop(std::max(0, t.maturity - t.development_stage));
            t.grow();
            draw_tree(t, 10);
            graphics.update();
        }

        forest.evolve(rng);
    }
    std::cout << "Finished on generation " << generations << "\n";
    std::cout << "Printing info about last generation:\n";
    forest.printStats();
}

// TODO: add proper parameters
void Model::draw_tree(const Tree &tree, float scale) {
    for (auto &seg : tree.segments) {
        graphics.line(
            {seg.first.x * scale + 120, seg.first.y * scale + 120},
            {seg.second.x * scale + 120, seg.second.y * scale + 120},
            2
        );
    }
}

//void Model::saveData() const {
//    forest.saveFittest(parameters.outdir);
//
//    const auto forestdir = parameters.outdir + "/forest";
//    std::filesystem::create_directory(forestdir);
//    forest.saveForest(forestdir);
//}
