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

unsigned int Plant::end_of_branch(std::vector<std::string>::iterator it) {
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

// TODO: this has to change to instead just check if there already is an object in that position and stop
//  development in that case
void Plant::grow_space() {
    DevState cur_state = {};
    std::vector<DevState> state_stack = {};
    // Position -> whether a seed that counted towards fitness was inserted at that position
    std::unordered_map<Pos, bool, key_hash> vertice_is_seed {{}};
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
            if (gene == "*") {
                if (search != vertice_is_seed.end()) {
                    search->second = true;
                }
                it = state_stack.empty() ? body.end() : it + end_of_branch(it);
                continue;
            }
            cur_state.pos.x += int(COLLISION_PRECISION * sin(cur_state.ax) * cos(cur_state.ay));
            cur_state.pos.y += int(COLLISION_PRECISION * cos(cur_state.ax) * cos(cur_state.ay));
            cur_state.pos.z += int(COLLISION_PRECISION * sin(cur_state.ay));
            // Seed got there first, but it's in the middle of branch
            if (search != vertice_is_seed.end() && search->second) {
                search->second = false;
            }

            auto inserted = vertice_is_seed.insert({cur_state.pos, false});
            if (!inserted.second) {
                it = state_stack.empty() ? body.end() : it + end_of_branch(it);
                continue;
            }
            segments.emplace_back(search->first, inserted.first->first);
        }
        it++;
    }

    seeds = {};
    for (const auto &pos : vertice_is_seed) {
        if (pos.second)
            seeds.push_back(pos.first);
    }
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
    grow_space();
}

std::string Plant::body_as_OBJ() const {
    std::vector<std::string> vertices;
    std::vector<std::string> instructions;
    for (int i = 0; i < segments.size(); i++) {
        const auto &seg = segments[i];
        vertices.push_back(
            std::string("v ") +
            std::to_string(seg.first.x) + " " +
            std::to_string(seg.first.y) + " " +
            std::to_string(seg.first.z)
        );
        vertices.push_back(
            std::string("v ") +
            std::to_string(seg.second.x) + " " +
            std::to_string(seg.second.y) + " " +
            std::to_string(seg.second.z)
        );
        instructions.push_back(std::string("l ") +
                               std::to_string(i * 2 + 1) + " " +
                               std::to_string(i * 2 + 2));
    }
    return vec_to_str(vertices, "\n") + "\n" + vec_to_str(instructions, "\n");
}
