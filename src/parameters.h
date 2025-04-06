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
    const short generations = 20000;
    // Use '0' for a random seed
    const unsigned int seed = 0;
    // Forest
    // ======
    const short n_pop = 10;
    // Tree
    // ====
    const short maturity = 4;
    // Make sure to balance this with maturity otherwise you wont have enough memory
    const unsigned short gene_activation_length = 3;
    const float core_gene_substitution_chance = 0.5;
    const unsigned short collision_precision = 1000;
    const float rotation_angle = M_PI / 6;
    // TODO: decide if this should be true of false (i dont think it should be a parameter but maybe).
    // True leads to faster runtimes but lower fitness (maybe also tends to look cooler?).
    const bool seed_skips = false;
    // Genome
    // ======
    const unsigned short start_genome_size = 5;
    // ATTENTION: THIS HAS TO BE LESS THAN MAX_CHAR - 48!!!
    const unsigned short max_genome_size = 10;
    const float mut_sub_rate = 0.01;
    const float mut_dup_rate = 0.001;
    const float mut_del_rate = 0.001;
};

#endif //L_SYSTEMS_PARAMETERS_H
