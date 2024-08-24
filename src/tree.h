//
// Created by aleferna on 03/06/24.
//

#ifndef L_SYSTEMS_TREE_H
#define L_SYSTEMS_TREE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include "utility.h"
#include "params.h"

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
    double ay = 0;
};

struct pos_hash {
    std::size_t operator()(const Pos &pos) const {
        return pos.x ^ pos.y ^ pos.z;
    }
};

class Tree {
public:
    Tree(const std::vector<std::string> &seedling, Genome genome, unsigned int maturity);

    Tree(unsigned int genome_size, unsigned int maturity);

    Tree() = default;

    //! Gets a clone of this tree before any growth took place.
    Tree germinate() const;

    //! Tree growth in space (updates segments and seeds).
    void grow();

    //! Tree body plan development.
    void develop(unsigned int stage);

    void mutate(double sub_rate, double dup_rate, double del_rate);

    double fitness() const;

    std::vector<std::string> translated_body() const;

    std::string genome_as_string() const;

    const auto &random_gene() const;

    void randomize_genome(unsigned int size);

    static bool is_growth_gene(const std::string &gene);

    //! Returns the gene back if its a core gene and "F" otherwise.
    static std::string translate_gene(const std::string &gene);

    std::string segments_as_OBJ() const;

    std::string seeds_as_OBJ() const;

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
};

#endif //L_SYSTEMS_TREE_H
