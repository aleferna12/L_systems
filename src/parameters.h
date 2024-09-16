//
// Created by alefe on 24/08/2024.
//

#ifndef L_SYSTEMS_PARAMETERS_H
#define L_SYSTEMS_PARAMETERS_H

#include <cmath>

struct Parameters {
    // Model
    // =====
    const std::string outdir = "out";
    // TODO: set to false once I implement a way to read parameters from the terminal
    const bool replace_dir = true;
    const int generations = 500;
    // Use '0' for a random seed
    const unsigned int seed = 89234796;
    // Forest
    // ======
    const int n_pop = 500;
    // Tree
    // ====
    const int maturity = 12;
    // Make sure to balance this with maturity otherwise you wont have enough memory
    const unsigned int gene_activation_length = 2;  // Usually 2 or 3
    const double core_gene_substitution_chance = 0.5;
    const unsigned int collision_precision = 1000;
    const double rotation_angle = M_PI / 6;
    // TODO: decide if this should be true of false (i dont think it should be a parameter but maybe).
    // True leads to faster runtimes but lower fitness (maybe also tends to look cooler?).
    const bool seed_skips = false;
    // Genome
    // ======
    const unsigned int start_genome_size = 10;
    const unsigned int max_genome_size = 100;
    const double mut_sub_rate = 0.05;
    const double mut_dup_rate = 0.005;
    const double mut_del_rate = 0.005;
};

#endif //L_SYSTEMS_PARAMETERS_H
