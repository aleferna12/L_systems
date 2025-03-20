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
    const unsigned int maturity
) : genome(std::move(genome)),
    seedling(seedling),
    body(seedling),
    maturity(maturity) {}

Tree::Tree(
    const Genome &genome,
    const unsigned int maturity,
    std::mt19937 &rng
) : Tree(
    {genome.getRandomGene(rng)},
    genome,
    maturity
) {}

void Tree::develop(const unsigned int stage) {
    std::vector<std::string> new_body;
    for (unsigned int i = 0; i < stage; i++) {
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

std::vector<std::string> Tree::translatedBody() const {
    std::vector<std::string> ret;
    for (const auto &gene : body) {
        ret.push_back(Genome::translateGene(gene));
    }
    return ret;
}

unsigned int Tree::endOfBranch(std::vector<std::string>::iterator it) {
    unsigned int nest = 0;
    unsigned int offset = 0;
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
    GeneralTree<PhenotypeData> binary_tree({});
    DevState cur_state = {&binary_tree, 0};
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
                prev_pos.x + int(collision_precision * sin(cur_state.angle)),
                prev_pos.y + int(collision_precision * cos(cur_state.angle))
            };

            bool hit = false;
            for (unsigned int i = 0; i < cur_state.node->nChildren(); i++) {
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

    setMass(binary_tree);
    // TODO make max_torque a parameter
    breakBranches(binary_tree, {}, collision_precision, 2000);

    seeds = {};
    for (const auto node : binary_tree.traverse()) {
        if (node->data.phen == SEED) {
            seeds.emplace_back(node->data.pos, collision_precision);
        }
    }
}

// TODO: instead of breaking, just invalidate seeds on branches, this way we can retrieve the morphologies
void Tree::breakBranches(
    GeneralTree<PhenotypeData> &tree,
    const CollisionPos parent_pos,
    const double branch_length,
    const double max_torque
) {
    if (tree.data.phen == ROOT) {
        tree.data.torque = 0;
    } else {
        const double x = parent_pos.x - tree.data.pos.x;
        const double y = parent_pos.y - tree.data.pos.y;
        const double a = atan2(y, x);
        const double cos_a = cos(a);
        const double d = branch_length * cos_a / 2;
        tree.data.torque = tree.data.mass * d * cos_a;
    }

    for (unsigned int i = tree.nChildren(); i--;) {
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

    double mass = 1;
    for (unsigned int i = 0; i < tree.nChildren(); i++) {
        auto &child = tree.getChild(i);
        setMass(child);
        mass += child.data.mass;
    }
    tree.data.mass = mass;
}

Tree Tree::germinate() const {
    return {seedling, genome, maturity};
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

std::string Tree::segmentsAsOBJ() const {
    std::vector<std::string> vertices;
    std::vector<std::string> lines;
    for (const auto &[v1, v2] : segments) {
        vertices.push_back(
            "v " +
            std::to_string(v1.x) + " " +
            std::to_string(v1.y)
        );
        vertices.push_back(
            "v " +
            std::to_string(v2.x) + " " +
            std::to_string(v2.y)
        );
        lines.push_back(
            "l " +
            std::to_string(vertices.size() - 1) + " " +
            std::to_string(vertices.size())
        );
    }
    return vecToStr(vertices, "\n") + "\n" + vecToStr(lines, "\n") + "\n";
}

std::string Tree::seedsAsOBJ() const {
    std::vector<std::string> vertices;
    for (const auto &seed : seeds) {
        vertices.push_back(
            "v " +
            std::to_string(seed.x) + " " +
            std::to_string(seed.y)
        );
    }
    return vecToStr(vertices, "\n") + "\n";
}

// TODO: seed worth more if farther away from other seeds
double Tree::fitness() const {
    std::unordered_set<int> fallen;
    double fit = 0;
    for (auto &seed : seeds) {
        double this_seed_fit = 1 + fabs(seed.x) * 0.2;
        int int_pos = int(round(collision_precision * seed.x));
        if (!fallen.contains(int_pos)) {
            fallen.insert(int_pos);
        } else {
            this_seed_fit /= 2;
        }
        fit += this_seed_fit;
    }
    return fit;
}
