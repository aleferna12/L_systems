//
// Created by aleferna on 03/06/24.
//

#include "tree.h"
#include <utility>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <sstream>

Tree::Tree(
    const std::vector<std::string> &seedling,
    Genome genome,
    unsigned int maturity
) : body(seedling),
    seedling(seedling),
    genome(std::move(genome)),
    maturity(maturity) {
}

const auto &Tree::random_gene() const {
    std::uniform_int_distribution<> uniform_genome(0, (int) genome.size() - 1);
    return *std::next(std::begin(genome), uniform_genome(rng));
}

void Tree::randomize_genome(unsigned int size) {
    if (size > 30)
        throw std::runtime_error("Max 'size' is 30");

    genome = {};
    genome["A"] = {"A", "A"};
    for (int i = 0; i < size; i++) {
        auto &gene = random_gene();
        genome[std::string{char(i + 66)}] = gene.second;
    }
    mut_sub(1);  // One time per target gene. Ideally we would make sure to mutate both genes instead...
    mut_sub(1);

    for (auto &gene : seedling)
        gene = random_gene().first;
}

void Tree::develop(unsigned int stage) {
    for (int i = 0; i < stage; i++) {
        std::vector<std::string> new_body;
        for (auto &gene: body) {
            auto search = genome.find(gene);
            if (search == genome.end()) {
                new_body.push_back(gene);
                continue;
            }

            auto target_genes = search->second;
            for (auto &target_gene: target_genes) {
                if (!target_gene.empty())
                    new_body.push_back(target_gene);
            }
        }
        body = new_body;
        development_stage++;
    }
}

std::string Tree::translate_gene(const std::string &gene) {
    if (is_growth_gene(gene)) {
        return "F";
    }
    return gene;
}

std::vector<std::string> Tree::translated_body() const {
    std::vector<std::string> ret;
    for (const auto &gene : body) {
        ret.push_back(translate_gene(gene));
    }
    return ret;
}

void Tree::mutate(double sub_rate, double dup_rate, double del_rate) {
    mut_sub(sub_rate);
    mut_del(del_rate);
    mut_dup(dup_rate);
}

void Tree::mut_dup(double dup_rate) {
    Genome to_add;
    for (auto &gene : genome) {
        if (uniform_random(rng) > dup_rate)
            continue;

        for (char c = 65; c <= 90; c++) {
            auto s = std::string{c};
            if (genome.find(s) == genome.end()) {
                to_add[s] = gene.second;
                break;
            }
        }
    }

    for (auto &gene : to_add) {
        genome.insert(gene);
    }
}

void Tree::mut_sub(double sub_rate) {
    for (auto &gene : genome) {
        if (uniform_random(rng) > sub_rate)
            continue;

        std::string sub_gene;
        if (uniform_random(rng) < 0.5) {
            std::uniform_int_distribution<> uniform_dir(0, CORE_GENES.size() - 1);
            sub_gene = CORE_GENES[uniform_dir(rng)];
        } else {
            sub_gene = random_gene().first;
        }

        gene.second.at(int(uniform_random(rng) * 2)) = sub_gene;
    }
}

void Tree::mut_del(double del_rate) {
    if (genome.size() == 1)
        return;

    std::vector<std::string> to_remove;
    for (auto &gene : genome) {
        if (uniform_random(rng) > del_rate)
            continue;

        to_remove.push_back(gene.first);

        for (auto &gene2 : genome) {
            for (auto &target_gene : gene2.second) {
                if (target_gene == gene.first)
                    target_gene = "";
            }
        }
    }

    for (auto &gene : to_remove)
        genome.erase(gene);
}

std::string Tree::genome_as_string() const {
    std::stringstream gen_ss;
    for (auto &gene : genome) {
        gen_ss << gene.first << " -> ";
        gen_ss << gene.second[0] << " | " << gene.second[1];
        gen_ss << "\n";
    }
    return gen_ss.str();
}

unsigned int Tree::end_of_branch(std::vector<std::string>::iterator it) {
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
            it = state_stack.empty() ? body.end() : it + end_of_branch(it);
        else
            it++;
    }

    seeds = {};
    for (const auto &pos : vertice_is_seed) {
        if (pos.second)
            seeds.push_back(pos.first);
    }
}

bool Tree::is_growth_gene(const std::string &gene) {
    return std::find(CORE_GENES.begin(), CORE_GENES.end(), gene) == CORE_GENES.end();
}

Tree::Tree(unsigned int genome_size, unsigned int maturity) : maturity(maturity) {
    randomize_genome(genome_size);
    seedling.push_back(random_gene().first);
    body = seedling;
}

Tree Tree::germinate() const {
    return {seedling, genome, maturity};
}

std::string Tree::segments_as_OBJ() const {
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
    return vec_to_str(vertices, "\n") + "\n" + vec_to_str(lines, "\n") + "\n";
}

std::string Tree::seeds_as_OBJ() const {
    std::vector<std::string> vertices;
    for (auto const &seed : seeds) {
        vertices.push_back(
            "v " +
            std::to_string(seed.x / (double) COLLISION_PRECISION) + " " +
            std::to_string(seed.y / (double) COLLISION_PRECISION) + " " +
            std::to_string(seed.z / (double) COLLISION_PRECISION)
        );
    }
    return vec_to_str(vertices, "\n") + "\n";
}

// TODO: introduce other factors such as verticality, distance from base etc
double Tree::fitness() const {
    return (double) seeds.size();
}
