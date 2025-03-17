//
// Created by alefe on 15/03/2025.
//

#include "binarytree.h"

BinaryTree::~BinaryTree() {
    while (!children.empty()) {
        children.pop_back();
    }
}

void BinaryTree::removeChild(const unsigned int pos) {
    children.erase(children.begin() + pos);
}

BinaryTree &BinaryTree::insertChild(const Phenotype phen, const CollisionPos &pos) {
    children.push_back(std::make_unique<BinaryTree>(phen, pos));
    return *(children.back());
}

std::vector<BinaryTree *> BinaryTree::traverse() {
    std::vector res = {this};
    for (const auto &child : children) {
        auto child_it = child->traverse();
        res.insert(res.end(), child_it.begin(), child_it.end());
    }
    return res;
}
