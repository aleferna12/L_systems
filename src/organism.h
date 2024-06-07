//
// Created by aleferna on 03/06/24.
//

#ifndef L_SYSTEMS_ORGANISM_H
#define L_SYSTEMS_ORGANISM_H

#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include "utility.h"

const unsigned int GROWTH_FACTOR = 100;

const std::array ROTATE_GENES = {"x+", "x-", "y+", "y-"};
const std::array DIR_GENES = {"x+", "x-", "y+", "y-", "s"};
const std::array CORE_GENES = {"x+", "x-", "y+", "y-", "s", "[", "]"};

using Genome = std::unordered_map<std::string, std::vector<std::string>>;

struct BranchProperties {
    int rotx;
    int roty;
    int cumroty;
    bool has_grown;
    bool last_seed;
};

class Organism {
public:
    Organism(const std::vector<std::string> &seedling, Genome genome);

    void grow(unsigned int nsteps = 1);

    unsigned int count_seeds();

    unsigned int get_fitness();

    std::vector<std::vector<std::string>> get_branches();

    bool valid_branch(const std::vector<std::string> &branch);

    std::vector<unsigned int> branch_lengths();

    unsigned int count_rotations();

    void mutate(double sub_rate, double dup_rate, double del_rate);

    void mut_dup(double dup_rate);

    void mut_sub(double sub_rate);

    void mut_del(double del_rate);

    std::string translated_body();

    static std::string translate_gene(const std::string &gene, const std::string &next_gene);

    void print_genome();

    std::vector<std::string> body;
    std::vector<std::string> seedling;
    Genome genome;

    unsigned int rotational_variance();

    std::string body_as_string();

    static bool is_growth_gene(const std::string &gene);
};

#endif //L_SYSTEMS_ORGANISM_H
