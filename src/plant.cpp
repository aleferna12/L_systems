//
// Created by aleferna on 03/06/24.
//

#include "plant.h"

#include <utility>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <unordered_set>

Plant::Plant(
    const std::vector<std::string> &seedling,
    Genome genome,
    unsigned int maturity
) : body(seedling),
    seedling(seedling),
    genome(std::move(genome)),
    maturity(maturity) {
}

const auto &Plant::random_gene() const {
    std::uniform_int_distribution<> uniform_genome(0, (int) genome.size() - 1);
    return *std::next(std::begin(genome), uniform_genome(rng));
}

void Plant::randomize_genome(unsigned int size) {
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

void Plant::grow(unsigned int stage) {
    if (stage > maturity) {
        std::cerr << "Trying to grow plant post maturity stage, plant will grow to maturity instead.\n";
        stage = maturity;
    }

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

std::string Plant::translate_gene(const std::string &gene) {
    if (is_growth_gene(gene)) {
        return "F";
    }
    return gene;
}

std::vector<std::string> Plant::translated_body() const {
    std::vector<std::string> ret;
    for (const auto &gene : body) {
        ret.push_back(translate_gene(gene));
    }
    return ret;
}

void Plant::mutate(double sub_rate, double dup_rate, double del_rate) {
    mut_sub(sub_rate);
    mut_del(del_rate);
    mut_dup(dup_rate);
}

void Plant::mut_dup(double dup_rate) {
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

void Plant::mut_sub(double sub_rate) {
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

void Plant::mut_del(double del_rate) {
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

void Plant::print_genome() const {
    for (auto &gene : genome) {
        std::cout << gene.first << " -> ";
        std::cout << gene.second[0] << " | " << gene.second[1];
        std::cout << "\n";
    }
}

// TODO: this has to change to instead just check if there already is an object in that position and stop
//  development in that case
void Plant::update_fitness() {
    unsigned int fit_counter = 0;
    DevState cur_state = {};
    std::vector<DevState> state_stack = {};
    // Position -> whether a seed that counted towards fitness was inserted at that position
    std::unordered_map<std::tuple<int, int, int>, bool, key_hash> increased_fitness;
    for (auto it = body.begin(); it != body.end(); it++) {
        std::string &gene = *it;
        auto pos_tup = std::make_tuple(cur_state.x, cur_state.y, cur_state.z);
        auto inserted = increased_fitness.insert({pos_tup, false});
        if (gene == "[") {
            state_stack.push_back(cur_state);
            cur_state = {};
        } else if (gene == "]" && !state_stack.empty()) {
            cur_state = state_stack.back();
            state_stack.pop_back();
        } else if (gene[0] == 'x')
            cur_state.ax += gene[1] == '+' ? ROTATION_ANGLE : -ROTATION_ANGLE;
        else if (gene[0] == 'y')
            cur_state.ay += gene[1] == '+' ? ROTATION_ANGLE : -ROTATION_ANGLE;
        else if (gene == "*") {
            if (inserted.second) {
                auto next = it + 1;
                if (next == body.end() || (!state_stack.empty() && *next == "]")) {
                    fit_counter++;
                    inserted.first->second = true;
                }
            }
        } else {
            cur_state.x += int(COLLISION_PRECISION * sin(cur_state.ax) * cos(cur_state.ay));
            cur_state.y += int(COLLISION_PRECISION * cos(cur_state.ax) * cos(cur_state.ay));
            cur_state.z += int(COLLISION_PRECISION * sin(cur_state.ay));
            // Seed got there first, but it's in the middle of branch
            if (!inserted.second && inserted.first->second) {
                fit_counter--;
                inserted.first->second = false;
            }
        }
    }
    fitness = fit_counter;
}

bool Plant::is_growth_gene(const std::string &gene) {
    return std::find(CORE_GENES.begin(), CORE_GENES.end(), gene) == CORE_GENES.end();
}

Plant::Plant(unsigned int genome_size, unsigned int maturity) : maturity(maturity) {
    randomize_genome(genome_size);
    seedling.push_back(random_gene().first);
    body = seedling;
}

Plant Plant::germinate() const {
    return {seedling, genome, maturity};
}

void Plant::develop(unsigned int stage) {
    grow(stage);
    update_fitness();
}
