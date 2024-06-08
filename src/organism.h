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
const int ROTATION_ANGLE = 30;

const std::array DIR_GENES = {"x+", "x-", "y+", "y-", "s"};
const std::array CORE_GENES = {"x+", "x-", "y+", "y-", "s", "[", "]"};

using Genome = std::unordered_map<std::string, std::vector<std::string>>;

struct DevState {
    double x;
    double y;
    double z;
    int ax;
    int ay;
};

struct key_hash {
    std::size_t operator()(const std::tuple<int, int, int> &k) const
    {
        return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
    }
};

class Organism {
public:
    Organism(const std::vector<std::string> &seedling, Genome genome);

    void grow(unsigned int nsteps = 1);

    unsigned int get_fitness();

    void mutate(double sub_rate, double dup_rate, double del_rate);

    void mut_dup(double dup_rate);

    void mut_sub(double sub_rate);

    void mut_del(double del_rate);

    static std::string translate_gene(const std::string &gene);

    std::vector<std::string> translated_body();

    void print_genome();

    static bool is_growth_gene(const std::string &gene);

    const auto &random_gene();

    void randomize_genome(unsigned int size);

    std::vector<std::string> body;
    std::vector<std::string> seedling;
    Genome genome;
};

#endif //L_SYSTEMS_ORGANISM_H
