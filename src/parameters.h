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
    const int generations = 5000;
    // Use '0' for a random seed
    const unsigned int seed = 0;
    // Forest
    // ======
    const int n_pop = 100;
    // Tree
    // ====
    const int maturity = 4;
    // Make sure to balance this with maturity otherwise you wont have enough memory
    const unsigned int gene_activation_length = 3;
    const double core_gene_substitution_chance = 0.5;
    const unsigned int collision_precision = 1000;
    const double rotation_angle = M_PI / 6;
    // TODO: decide if this should be true of false (i dont think it should be a parameter but maybe).
    // True leads to faster runtimes but lower fitness (maybe also tends to look cooler?).
    const bool seed_skips = false;
    // Genome
    // ======
    const unsigned int start_genome_size = 10;
    const unsigned int max_genome_size = 30;
    const double mut_sub_rate = 0.02;
    const double mut_dup_rate = 0.002;
    const double mut_del_rate = 0.002;
};

#endif //L_SYSTEMS_PARAMETERS_H
