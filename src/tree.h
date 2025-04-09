//
// Created by aleferna on 03/06/24.
//

#ifndef L_SYSTEMS_TREE_H
#define L_SYSTEMS_TREE_H

#include <string>
#include <vector>

#include "generaltree.h"
#include "utility.h"
#include "pos.h"
#include "parameters.h"
#include "genome.h"

enum Phenotype {
    SEED,
    GROWTH,
    ROOT
};

struct PhenotypeData {
    Phenotype phen = ROOT;
    CollisionPos pos;
    float mass = 0;
    float torque = 0;
};

struct DevState {
    GeneralTree<PhenotypeData> *node = nullptr;
    float angle = 0;
};

class Tree {
public:
    Tree(const std::vector<char> &seedling, Genome genome, uint16_t maturity);

    Tree(const Genome &genome, uint16_t maturity, std::mt19937 &rng);

    std::string asTREE() const;

    //! Tree growth in space (updates segments and seeds).
    void grow();

    void breakBranches(GeneralTree<PhenotypeData> &tree, CollisionPos parent_pos, float branch_length, float max_torque);

    static void setMass(GeneralTree<PhenotypeData> &tree);

    //! Tree body plan development.
    void develop(uint16_t stage);

    Genome genome;
    std::vector<char> seedling;  // Needs to be initialized by all constructors
    std::vector<char> body;  // Needs to be initialized by all constructors
    uint16_t maturity;

    // TODO: add parameters
    uint16_t collision_precision = 1000;
    float rotation_angle = M_PI / 6;
    bool seed_skips = false;
    std::vector<std::pair<Pos, Pos>> segments;
    std::vector<Pos> seeds;

    uint16_t development_stage = 0;
    float fitness = 0;

    void reset_development();

private:
    uint16_t endOfBranch(std::vector<char>::iterator it);

    float get_fitness() const;

    void find_seeds(GeneralTree<PhenotypeData> &tree);
};

#endif //L_SYSTEMS_TREE_H
