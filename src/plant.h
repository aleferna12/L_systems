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

const unsigned int COLLISION_PRECISION = 1000;
const double ROTATION_ANGLE = M_PI / 6;

const std::array CORE_GENES = {"x+", "x-", "y+", "y-", "*", "[", "]"};

using Genome = std::unordered_map<std::string, std::vector<std::string>>;

struct Pos {
    int x = 0;
    int y = 0;
    int z = 0;

    bool operator==(const Pos& other) const = default;
};

struct DevState {
    Pos pos = {};
    double ax = 0.;
    double ay = M_PI / 2;
};

struct key_hash {
    std::size_t operator()(const Pos &pos) const {
        return pos.x ^ pos.y ^ pos.z;
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

    //! Grows body and then develops spatially (updates seeds and segments).
    void develop(unsigned int stage);

    void mutate(double sub_rate, double dup_rate, double del_rate);

    std::vector<std::string> translated_body() const;

    void print_genome() const;

    const auto &random_gene() const;

    void randomize_genome(unsigned int size);

    static bool is_growth_gene(const std::string &gene);

    //! Returns the gene back if its a core gene and "F" otherwise.
    static std::string translate_gene(const std::string &gene);

    std::string body_as_OBJ() const;

    std::vector<std::string> body;
    std::vector<std::string> seedling;
    Genome genome;
    std::vector<std::pair<Pos, Pos>> segments;
    std::vector<Pos> seeds;
    unsigned int development_stage = 0;
    unsigned int maturity = 0;

private:
    void mut_dup(double dup_rate);

    void mut_sub(double sub_rate);

    void mut_del(double del_rate);

    unsigned int end_of_branch(std::vector<std::string>::iterator it);

    void grow_space();
};

#endif //L_SYSTEMS_PLANT_H
