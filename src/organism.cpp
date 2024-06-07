//
// Created by aleferna on 03/06/24.
//

#include "organism.h"

#include <utility>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>

Organism::Organism(
    const std::vector<std::string> &seedling,
    Genome genome
) : body(seedling),
    seedling(seedling),
    genome(std::move(genome)) {
}

void Organism::grow(unsigned int nsteps) {
    for (int i = 0; i < nsteps; i++) {
        std::vector<std::string> new_body;
        for (auto &gene: body) {
            auto search = genome.find(gene);
            if (search == genome.end()) {
                new_body.push_back(gene);
                continue;
            }

            auto target_genes = search->second;
            if (target_genes.size() != 3) {
                // TODO: This should point to an 3-long array instead of a vector
                throw std::runtime_error("Malformed genome rule");
            }
            for (auto &target_gene: target_genes) {
                if (!target_gene.empty())
                    new_body.push_back(target_gene);
            }
        }
        body = new_body;
    }
}

std::string Organism::body_as_string() {
    std::ostringstream result;
    for (auto it = body.begin(); it != body.end() - 1; it++) {
        result << *it;
        result << ',';
    }
    result << body.back();
    return result.str();
}

std::string Organism::translated_body() {
    std::ostringstream result;
    for (auto it = body.begin(); it != body.end() - 1; it++) {
        result << translate_gene(*it, *(it + 1));
        result << ',';
    }
    result << translate_gene(body.back(), "");
    return result.str();
}

std::string Organism::translate_gene(const std::string &gene, const std::string &next_gene) {
    if (is_growth_gene(gene)) {
        return "F";
    }
    return gene;
}

void Organism::mutate(double sub_rate, double dup_rate, double del_rate) {
    mut_sub(sub_rate);
    mut_del(del_rate);
    mut_dup(dup_rate);
}

void Organism::mut_dup(double dup_rate) {
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

void Organism::mut_sub(double sub_rate) {
    for (auto &gene : genome) {
        if (uniform_random(rng) > sub_rate)
            continue;

        if (uniform_random(rng) < 0.1) {
            gene.second[0] = "[";
            gene.second[1] = "]";
            continue;
        }

        std::string sub_gene;
        if (uniform_random(rng) < 0.1) {
            std::uniform_int_distribution<> uniform_dir(0, DIR_GENES.size() - 1);
            sub_gene = DIR_GENES[uniform_dir(rng)];
        } else {
            std::uniform_int_distribution<> uniform_genome(0, (int) genome.size() - 1);
            sub_gene = std::next(std::begin(genome), uniform_genome(rng))->first;
        }

        std::uniform_int_distribution<> uniform_3(0, 2);
        int target_i = uniform_3(rng);
        gene.second[target_i] = sub_gene;
    }
}

void Organism::mut_del(double del_rate) {
    if (genome.size() == 1)
        return;

    std::vector<std::string> to_remove;
    for (auto &gene : genome) {
        if (uniform_random(rng) > del_rate)
            continue;

        to_remove.push_back(gene.first);

        for (auto &gene2 : genome) {
            for (int target_gene = 0; target_gene < 3; target_gene++) {
                if (gene2.second[target_gene] == gene.first)
                    gene2.second[target_gene] = "";
            }
        }
    }

    for (auto &gene : to_remove)
        genome.erase(gene);
}

void Organism::print_genome() {
    for (auto &gene : genome) {
        std::cout << gene.first << ": ";
        for (int i = 0; i < 2; i++) {
            std::cout << gene.second[i] << " - ";
        }
        std::cout << gene.second[2];
        std::cout << "\n";
    }
}

unsigned int Organism::count_seeds() {
    unsigned int c = 0;
    for (auto &gene : body) {
        if (gene == "s")
            c++;
    }
    return c;
}

std::vector<unsigned int> Organism::branch_lengths() {
    // Size of the branch and whether it contains both directional genes and growth genes
    std::vector<std::pair<unsigned int, std::pair<bool, bool>>> bl_stack;
    std::vector<unsigned int> branch_ls;
    for (auto &gene : body) {
        if (gene == "[") {
            bl_stack.emplace_back();
            continue;
        }
        if (bl_stack.empty())
            continue;

        auto &x = bl_stack.back();
        if (gene == "]") {
            if (x.second.first && x.second.second)
                branch_ls.push_back(x.first);
            bl_stack.pop_back();
        } else {
            if (std::find(ROTATE_GENES.begin(), ROTATE_GENES.end(), gene) != ROTATE_GENES.end())
                x.second.first = true;
            else
                x.second.second = true;
            x.first++;
        }
    }
    return branch_ls;
}

unsigned int Organism::count_rotations() {
    unsigned int c = 0;
    for (auto &gene : body) {
        if (std::find(ROTATE_GENES.begin(), ROTATE_GENES.end(), gene) != ROTATE_GENES.end())
            c++;
    }
    return c;
}

unsigned int Organism::rotational_variance() {
    std::unordered_set<std::string> rot_genes;
    for (auto &gene : body) {
        if (std::find(ROTATE_GENES.begin(), ROTATE_GENES.end(), gene) != ROTATE_GENES.end()) {
            rot_genes.insert(gene);
        }
    }
    return rot_genes.size();
}

// TODO: this has to change to instead just check if there already is an object in that position and stop
//  development in that case
unsigned int Organism::get_fitness() {
    unsigned int fit_counter = 0;
    std::vector<BranchProperties> valid {{}};

    for (const auto &gene : body) {
        if (gene == "[") {
            valid.emplace_back();
            valid.back().cumroty = valid[valid.size() - 2].cumroty;
            continue;
        }
        auto &cur_valid = valid.back();
        if (gene == "]") {
            if (valid.size() <= 1) {
                continue;
            }

            if (
                cur_valid.last_seed &&
                cur_valid.has_grown &&
                abs(cur_valid.cumroty) < 4
            )
                fit_counter++;

            valid.pop_back();
            continue;
        }

        if (gene[0] == 'y') {
            auto add = gene[1] == '+' ? 1 : -1;
            cur_valid.roty += add;
            cur_valid.cumroty += add;
        }
        else if (gene[0] == 'x')
            cur_valid.rotx += gene[1] == '+' ? 1 : -1;
        else if (gene == "s")
            cur_valid.last_seed = true;
        else {
            const auto &prev_valid = valid[valid.size() - 2];
            if (
                !cur_valid.has_grown && (
                    cur_valid.rotx != 0 &&
                    cur_valid.roty != prev_valid.roty
                )
            )
                cur_valid.has_grown = true;
            cur_valid.last_seed = false;
        }
    }

    return fit_counter + valid[0].last_seed;
}

bool Organism::is_growth_gene(const std::string &gene) {
    return std::find(CORE_GENES.begin(), CORE_GENES.end(), gene) == CORE_GENES.end();
}

//unsigned int Organism::get_fitness() {
//    unsigned int fitness = 0;
//    auto branches = get_branches();
//    for (const auto &branch : branches) {
//        if (!branch.empty() || branch.back() != "s") {
//            fitness++;
//        }
//    }
//    return fitness;
//}
