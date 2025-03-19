//
// Created by alefe on 15/03/2025.
//

#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <memory>
#include <vector>

template <class T>
class GeneralTree {
public:
    explicit GeneralTree(const T data) : data(data) {};

    ~GeneralTree() {
        while (!children.empty()) {
            children.pop_back();
        }
    }

    unsigned int nChildren() const {return children.size();};

    GeneralTree &getChild(const unsigned int pos) const {
        return *children.at(pos);
    }

    void removeChild(unsigned int pos) {
        children.erase(children.begin() + pos);
    }

    GeneralTree &insertChild(T phen) {
        children.push_back(std::make_unique<GeneralTree>(phen));
        return *(children.back());
    }

    // This could be a vector of reference_wrappers but that is just more complicated for no reason
    std::vector<GeneralTree *> traverse() {
        std::vector<GeneralTree *> res = {this};
        for (const auto &child : children) {
            auto child_it = child->traverse();
            res.insert(res.end(), child_it.begin(), child_it.end());
        }
        return res;
    }

    T data;
private:
    std::vector<std::unique_ptr<GeneralTree>> children;
};

#endif //BINARYTREE_H
