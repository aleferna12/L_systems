//
// Created by aleferna on 03/06/24.
//

#include <utility>
#include <stdexcept>
#include <unordered_set>
#include <iostream>
#include "tree.h"

Tree::Tree(
    const std::vector<std::string> &seedling,
    Genome genome,
    unsigned int maturity
) : body(seedling),
    seedling(seedling),
    genome(std::move(genome)),
    maturity(maturity) {
}

Tree::Tree(
    unsigned int genome_size,
    unsigned int maturity
) : maturity(maturity),
    genome(genome_size) {
    seedling = {genome.getRandomGene()};
    body = seedling;
}

void Tree::develop(unsigned int stage) {
    for (int i = 0; i < stage; i++) {
        std::vector<std::string> new_body;
        for (auto &gene: body) {
            auto target_genes = genome.geneActivates(gene);
            if (target_genes == nullptr) {  // Either core gene or gene left behind by deletion
                new_body.push_back(gene);
                continue;
            }
            for (auto &target_gene: *target_genes) {
                if (!target_gene.empty())
                    new_body.push_back(target_gene);
            }
        }
        body = new_body;
        development_stage++;
    }
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
    std::unordered_map<Pos, bool, pos_hash> vertice_is_seed {{}};
    segments = {};
    auto it = body.begin();
    while (it != body.end()) {
        std::string &gene = *it;
        if (gene == "[") {
            state_stack.push_back(cur_state);
            cur_state = {};
        } else if (gene == "]") {
            if (!state_stack.empty()) {
                cur_state = state_stack.back();
                state_stack.pop_back();
            }
        } else if (gene[0] == 'x')
            cur_state.ax += gene[1] == '+' ? ROTATION_ANGLE : -ROTATION_ANGLE;
        else if (gene[0] == 'y')
            cur_state.ay += gene[1] == '+' ? ROTATION_ANGLE : -ROTATION_ANGLE;
        else {
            auto search = vertice_is_seed.find(cur_state.pos);
            if (search != vertice_is_seed.end())
                search->second = false;

            cur_state.pos.x += int(COLLISION_PRECISION * sin(cur_state.ax) * cos(cur_state.ay));
            cur_state.pos.y += int(COLLISION_PRECISION * cos(cur_state.ax) * cos(cur_state.ay));
            cur_state.pos.z += int(COLLISION_PRECISION * sin(cur_state.ay));

            vertice_is_seed.insert({cur_state.pos, gene == "*"});
            segments.emplace_back(search->first, cur_state.pos);
        }
        if (SEED_SKIPS & gene == "*")
            it = state_stack.empty() ? body.end() : it + endOfBranch(it);
        else
            it++;
    }

    seeds = {};
    for (const auto &pos : vertice_is_seed) {
        if (pos.second)
            seeds.push_back(pos.first);
    }
}

Tree Tree::germinate() const {
    return {seedling, genome, maturity};
}

std::string Tree::segmentsAsOBJ() const {
    std::vector<std::string> vertices;
    std::vector<std::string> lines;
    for (auto const &[v1, v2] : segments) {
        vertices.push_back(
            "v " +
            std::to_string(v1.x / (double) COLLISION_PRECISION) + " " +
            std::to_string(v1.y / (double) COLLISION_PRECISION) + " " +
            std::to_string(v1.z / (double) COLLISION_PRECISION)
        );
        vertices.push_back(
            "v " +
            std::to_string(v2.x / (double) COLLISION_PRECISION) + " " +
            std::to_string(v2.y / (double) COLLISION_PRECISION) + " " +
            std::to_string(v2.z / (double) COLLISION_PRECISION)
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
    for (auto const &seed : seeds) {
        vertices.push_back(
            "v " +
            std::to_string(seed.x / (double) COLLISION_PRECISION) + " " +
            std::to_string(seed.y / (double) COLLISION_PRECISION) + " " +
            std::to_string(seed.z / (double) COLLISION_PRECISION)
        );
    }
    return vecToStr(vertices, "\n") + "\n";
}

// TODO: introduce other factors such as verticality, distance from base etc
double Tree::fitness() const {
    return (double) seeds.size();
}
