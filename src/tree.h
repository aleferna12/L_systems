//
// Created by aleferna on 03/06/24.
//

#ifndef L_SYSTEMS_TREE_H
#define L_SYSTEMS_TREE_H

#include <string>
#include <vector>
#include "utility.h"
#include "params.h"
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

    Tree(unsigned int genome_size, unsigned int maturity);

    Tree() = default;

    //! Gets a clone of this tree before any growth took place.
    Tree germinate() const;

    //! Tree growth in space (updates segments and seeds).
    void grow();

    //! Tree body plan development.
    void develop(unsigned int stage);

    double fitness() const;

    std::vector<std::string> translatedBody() const;

    std::string segmentsAsOBJ() const;

    std::string seedsAsOBJ() const;

    std::vector<std::string> body;
    std::vector<std::string> seedling;
    Genome genome;
    std::vector<std::pair<Pos, Pos>> segments;
    std::vector<Pos> seeds;
    unsigned int development_stage = 0;
    unsigned int maturity = 0;

private:
    unsigned int endOfBranch(std::vector<std::string>::iterator it);
};

#endif //L_SYSTEMS_TREE_H
