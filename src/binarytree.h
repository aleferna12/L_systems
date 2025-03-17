//
// Created by alefe on 15/03/2025.
//

#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <memory>
#include <vector>
#include "pos.h"

enum Phenotype {
    SEED,
    GROWTH,
    ROOT
};

class BinaryTree {
public:
    BinaryTree() = default;

    BinaryTree(const Phenotype phen, const CollisionPos &pos) : phen(phen), pos(pos) {};

    ~BinaryTree();

    unsigned int nChildren() const {return children.size();};

    BinaryTree &getChild(const unsigned int pos) const {
        return *children.at(pos);
    }

    void removeChild(unsigned int pos);

    BinaryTree &insertChild(Phenotype phen, const CollisionPos &pos);

    // This could be a vector of reference_wrappers but that is just more complicated for no reason
    std::vector<BinaryTree *> traverse();

    Phenotype phen = ROOT;
    CollisionPos pos;
private:
    std::vector<std::unique_ptr<BinaryTree>> children;
};

#endif //BINARYTREE_H
