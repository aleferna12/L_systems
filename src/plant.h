//
// Created by aleferna on 03/06/24.
//

#ifndef L_SYSTEMS_PLANT_H
#define L_SYSTEMS_PLANT_H

#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include "utility.h"

const unsigned int GROWTH_FACTOR = 100;
const int ROTATION_ANGLE = 30;

const std::array CORE_GENES = {"x+", "x-", "y+", "y-", "*", "[", "]"};

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

class Plant {
public:
    Plant(const std::vector<std::string> &seedling, Genome genome, unsigned int maturity);

    Plant(unsigned int genome_size, unsigned int maturity);

    Plant() = default;

    //! Gets a clone of this plant before any growth took place.
    Plant germinate() const;

    void grow(unsigned int stage);

    //! Grows and then updates fitness.
    void develop(unsigned int stage);

    void update_fitness();

    void mutate(double sub_rate, double dup_rate, double del_rate);

    std::vector<std::string> translated_body() const;

    void print_genome() const;

    const auto &random_gene() const;

    void randomize_genome(unsigned int size);

    static bool is_growth_gene(const std::string &gene);

    //! Returns the gene back if its a core gene and "F" otherwise.
    static std::string translate_gene(const std::string &gene);

    std::vector<std::string> body;
    std::vector<std::string> seedling;
    Genome genome;
    unsigned int fitness = 0;
    unsigned int development_stage = 0;
    unsigned int maturity = 0;

private:
    void mut_dup(double dup_rate);

    void mut_sub(double sub_rate);

    void mut_del(double del_rate);
};

#endif //L_SYSTEMS_PLANT_H
