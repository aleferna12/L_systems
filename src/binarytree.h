//
// Created by alefe on 15/03/2025.
//

#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <memory>
#include <string>
#include <vector>
#include "pos.h"

enum Phenotype {
    SEED,
    GROWTH,
    ROOT
};

class BinaryTree {
public:
    BinaryTree(const Phenotype phen, const CollisionPos &pos) : phen(phen), pos(pos) {};

    ~BinaryTree();

    unsigned int nChildren() const {return children.size();};

    BinaryTree *getChild(const unsigned int pos) const {
        return children.at(pos);
    }

    void removeChild(unsigned int pos);

    BinaryTree *insertChild(Phenotype phen, const CollisionPos &pos);

    std::vector<BinaryTree *> traverse();

    Phenotype phen;
    CollisionPos pos;
private:
    std::vector<BinaryTree *> children;
};

#endif //BINARYTREE_H
