//
// Created by aleferna on 03/06/24.
//

#ifndef L_SYSTEMS_TREE_H
#define L_SYSTEMS_TREE_H

#include <string>
#include <vector>
#include "utility.h"
#include "parameters.h"
#include "genome.h"

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

    Tree(const Genome &genome, unsigned int maturity, std::mt19937 &rng);

    //! Gets a clone of this tree before any growth took place.
    Tree germinate() const;

    //! Tree growth in space (updates segments and seeds).
    void grow();

    //! Tree body plan development.
    void develop(unsigned int stage);

    //! Tree body plan development to maturity.
    void develop() { develop(maturity); }

    double fitness() const;

    std::vector<std::string> translatedBody() const;

    std::string segmentsAsOBJ() const;

    std::string seedsAsOBJ() const;

    Genome genome;
    std::vector<std::string> seedling;  // Needs to be initialized by all constructors
    std::vector<std::string> body;  // Needs to be initialized by all constructors
    unsigned int maturity;

    unsigned int collision_precision = 1000;
    double rotation_angle = M_PI / 6;
    bool seed_skips = false;

private:
    unsigned int endOfBranch(std::vector<std::string>::iterator it);

    unsigned int development_stage = 0;
    std::vector<std::pair<Pos, Pos>> segments;
    std::vector<Pos> seeds;
};

#endif //L_SYSTEMS_TREE_H
