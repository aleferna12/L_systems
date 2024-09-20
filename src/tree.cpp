//
// Created by aleferna on 03/06/24.
//

#include <utility>
#include <stdexcept>
#include <unordered_set>
#include "tree.h"

Tree::Tree(
    const std::vector<std::string> &seedling,
    Genome genome,
    unsigned int maturity
) : genome(std::move(genome)),
    seedling(seedling),
    body(seedling),
    maturity(maturity) {}

Tree::Tree(
    const Genome &genome,
    unsigned int maturity,
    std::mt19937 &rng
) : Tree(
    {genome.getRandomGene(rng)},
    genome,
    maturity
) {}

void Tree::develop(unsigned int stage) {
    std::vector<std::string> new_body;
    for (unsigned int i = 0; i < stage; i++) {
        new_body.clear();

        for (auto &gene : body) {
            auto target_genes = genome.geneActivates(gene);
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
    for (;it != body.end(); it++) {
        if (*it == "]") {
            if (nest == 0)
                return offset;
            else
                nest--;
        } else if (*it == "[")
            nest++;
        offset++;
    }
    return offset;
}

void Tree::grow() {
    DevState cur_state = {};
    std::vector<DevState> state_stack = {};
    // Position -> whether a seed that counted towards fitness was inserted at that position
    std::unordered_map<CollisionPos, bool, pos_hash> vertice_is_seed {{}};
    segments = {};
    auto it = body.begin();
    while (it != body.end()) {
        std::string &gene = *it;
        bool inside_branch = !state_stack.empty();
        if (gene == "[") {
            state_stack.push_back(cur_state);
        } else if (gene == "]") {
            if (inside_branch) {
                cur_state = state_stack.back();
                state_stack.pop_back();
            }
        } else if (gene[0] == 'x')
            cur_state.ax += gene[1] == '+' ? rotation_angle : -rotation_angle;
        else if (gene[0] == 'y')
            cur_state.ay += gene[1] == '+' ? rotation_angle : -rotation_angle;
        else {
            auto search = vertice_is_seed.find(cur_state.pos);
            if (search != vertice_is_seed.end())
                search->second = false;

            double cos_ay = cos(cur_state.ay);
            cur_state.pos.x += int(collision_precision * sin(cur_state.ax) * cos_ay);
            cur_state.pos.y += int(collision_precision * cos(cur_state.ax) * cos_ay);
            cur_state.pos.z += int(collision_precision * sin(cur_state.ay));

            // Prevents branches growing downwards
            // TODO: replace with excessive torque breaking branches
            if (cur_state.pos.y < search->first.y) {
                it = !inside_branch ? body.end() : it + endOfBranch(it);
                continue;
            }

            vertice_is_seed.insert({cur_state.pos, gene == "*"});
            segments.emplace_back(
                Pos(search->first, collision_precision),
                Pos(cur_state.pos, collision_precision)
            );
        }
        if (seed_skips && (gene == "*"))
            it = !inside_branch ? body.end() : it + endOfBranch(it);
        else
            it++;
    }

    seeds = {};
    seeds.reserve(vertice_is_seed.size());
    for (const auto &pos : vertice_is_seed) {
        if (pos.second)
            seeds.emplace_back(pos.first, collision_precision);
    }
}

Tree Tree::germinate() const {
    return {seedling, genome, maturity};
}

std::string Tree::segmentsAsOBJ() const {
    std::vector<std::string> vertices;
    std::vector<std::string> lines;
    for (const auto &[v1, v2] : segments) {
        vertices.push_back(
            "v " +
            std::to_string(v1.x) + " " +
            std::to_string(v1.y) + " " +
            std::to_string(v1.z)
        );
        vertices.push_back(
            "v " +
            std::to_string(v2.x) + " " +
            std::to_string(v2.y) + " " +
            std::to_string(v2.z)
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
            std::to_string(seed.y) + " " +
            std::to_string(seed.z)
        );
    }
    return vecToStr(vertices, "\n") + "\n";
}

// TODO: introduce other factors such as verticality, distance from base etc
double Tree::fitness() const {
    return (double) seeds.size();
}
