//
// Created by alefe on 15/03/2025.
//

#include "binarytree.h"

#include <iostream>

BinaryTree::~BinaryTree() {
    while (!children.empty()) {
        delete children.back();
        children.pop_back();
    }
}

void BinaryTree::removeChild(const unsigned int pos) {
    children.erase(children.begin() + pos);
}

BinaryTree *BinaryTree::insertChild(const Phenotype phen, const CollisionPos &pos) {
    children.push_back(new BinaryTree(phen, pos));
    return children.back();
}

std::vector<BinaryTree *> BinaryTree::traverse() {
    std::vector<BinaryTree *> res = {this};
    for (BinaryTree *child : children) {
        auto child_it = child->traverse();
        res.insert(res.end(), child_it.begin(), child_it.end());
    }
    return res;
}
