//
// Created by aleferna on 03/06/24.
//

#include <utility>
#include <unordered_set>
#include <sstream>
#include "tree.h"

#include <iostream>

Tree::Tree(
    const std::vector<std::string> &seedling,
    Genome genome,
    const unsigned short maturity
) : genome(std::move(genome)),
    seedling(seedling),
    body(seedling),
    maturity(maturity) {}

Tree::Tree(
    const Genome &genome,
    const unsigned short maturity,
    std::mt19937 &rng
) : Tree(
    {genome.getRandomGene(rng)},
    genome,
    maturity
) {}

void Tree::develop(const unsigned short stage) {
    std::vector<std::string> new_body;
    for (unsigned short i = 0; i < stage; i++) {
        new_body.clear();

        for (auto &gene : body) {
            const auto target_genes = genome.geneActivates(gene);
            if (target_genes == nullptr) {
                new_body.push_back(gene);
                continue;
            }
            for (auto &target_gene : *target_genes) {
                if (!target_gene.empty())
                    new_body.push_back(target_gene);
            }
        }
        std::swap(body, new_body);
    }
    development_stage += stage;
}

unsigned short Tree::endOfBranch(std::vector<std::string>::iterator it) {
    unsigned short nest = 0;
    unsigned short offset = 0;
    for (;it != body.end(); ++it) {
        if (*it == "]") {
            if (nest == 0)
                return offset;
            nest--;
        } else if (*it == "[")
            nest++;
        offset++;
    }
    return offset;
}

void Tree::grow() {
    GeneralTree<PhenotypeData> general_tree({});
    DevState cur_state = {&general_tree, 0};
    std::vector<DevState> state_stack = {};
    segments = {};
    auto it = body.begin();
    while (it != body.end()) {
        std::string &gene = *it;
        const bool inside_branch = !state_stack.empty();
        if (gene == "[") {
            state_stack.push_back(cur_state);
        } else if (gene == "]") {
            if (inside_branch) {
                cur_state = state_stack.back();
                state_stack.pop_back();
            }
        } else if (gene == "+")
            cur_state.angle += rotation_angle;
        else if (gene == "-")
            cur_state.angle -= rotation_angle;
        else if (gene == "*") {
            if (cur_state.node->nChildren() == 0) {
                cur_state.node->data.phen = SEED;
                if (seed_skips) {
                    it = !inside_branch ? body.end() : it + endOfBranch(it);
                    continue;
                }
            }
        }
        else {
            if (cur_state.node->data.phen == SEED) {
                cur_state.node->data.phen = GROWTH;
            }

            const auto prev_pos = cur_state.node->data.pos;
            CollisionPos next_pos = {
                static_cast<short>(prev_pos.x + collision_precision * sinf(cur_state.angle)),
                static_cast<short>(prev_pos.y + collision_precision * cosf(cur_state.angle))
            };

            bool hit = false;
            for (unsigned short i = 0; i < cur_state.node->nChildren(); i++) {
                const auto &child = cur_state.node->getChild(i);
                if (child.data.pos == next_pos) {
                    hit = true;
                    break;
                }
            }
            if (hit) {
                ++it;
                continue;
            }

            auto &next_node = cur_state.node->insertChild({GROWTH, next_pos});
            cur_state.node = &next_node;

            segments.emplace_back(
                Pos(prev_pos, collision_precision),
                Pos(next_pos, collision_precision)
            );
        }
        ++it;
    }

    setMass(general_tree);
    // TODO make max_torque a parameter
    breakBranches(general_tree, {}, collision_precision, 2000);

    find_seeds(general_tree);
    fitness = get_fitness();
}

void Tree::find_seeds(GeneralTree<PhenotypeData> &tree) {
    for (unsigned short i = 0; i < tree.nChildren(); i++) {
        auto &child = tree.getChild(i);
        if (child.data.phen == SEED) {
            seeds.emplace_back(child.data.pos, collision_precision);
        }
        find_seeds(child);
    }
}

void Tree::reset_development() {
    body = seedling;
    segments.clear();
    seeds.clear();
    development_stage = 0;
}

// TODO: instead of breaking, just invalidate seeds on branches, this way we can retrieve the morphologies
void Tree::breakBranches(
    GeneralTree<PhenotypeData> &tree,
    const CollisionPos parent_pos,
    const float branch_length,
    const float max_torque
) {
    if (tree.data.phen == ROOT) {
        tree.data.torque = 0;
    } else {
        const float x = parent_pos.x - tree.data.pos.x;
        const float y = parent_pos.y - tree.data.pos.y;
        const float a = atan2(y, x);
        const float cos_a = cos(a);
        const float d = branch_length * cos_a / 2;
        tree.data.torque = tree.data.mass * d * cos_a;
    }

    for (unsigned short i = tree.nChildren(); i--;) {
        auto &child = tree.getChild(i);
        breakBranches(child, tree.data.pos, branch_length, max_torque);
        if (child.data.torque > max_torque || child.data.pos.y < tree.data.pos.y) {
            tree.removeChild(i);
        }
    }
}

void Tree::setMass(GeneralTree<PhenotypeData> &tree) {
    if (tree.nChildren() == 0) {
        tree.data.mass = 1;
        return;
    }

    float mass = 1;
    for (unsigned short i = 0; i < tree.nChildren(); i++) {
        auto &child = tree.getChild(i);
        setMass(child);
        mass += child.data.mass;
    }
    tree.data.mass = mass;
}

std::string Tree::asTREE() const {
    std::stringstream res;
    for (const auto &[v1, v2] : segments) {
        res << 's' << v1.x << ' ' << v1.y << ',';
        res << v2.x << ' ' << v2.y << '\n';
    }
    for (const auto &seed : seeds) {
        res << '*' << seed.x << ' ' << seed.y << '\n';
    }
    return res.str();
}

float Tree::get_fitness() const {
    float fit = 0;
    for (unsigned short i = 0; i < seeds.size(); i++) {
        auto &seed1 = seeds.at(i);
        fit += 10 + fabs(seed1.x) / 10;
        for (unsigned short j = 0; j < seeds.size(); j++) {
            if (i == j)
                continue;
            auto &seed2 = seeds.at(j);
            fit += fabs(seed1.x - seed2.x) / (float) seeds.size();
        }
    }
    return fit;
}
