//
// Created by alefe on 29/08/2024.
//

#include <sstream>
#include <iostream>
#include "genome.h"
#include "utility.h"

Genome::Genome(
    char size,
    unsigned short max_size,
    float mut_sub,
    float mut_dup,
    float mut_del,
    unsigned short gene_activation_length,
    std::mt19937 &rng) :
    max_size(max_size),
    mut_sub(mut_sub),
    mut_dup(mut_dup),
    mut_del(mut_del),
    gene_activation_length(gene_activation_length) {
    if (size > max_size) {
        std::cerr << "Starting genome size larger than maximum genome size \n";
        exit(EXIT_FAILURE);
    }

    for (char i = FIRST_GROWTH_GENE; i < FIRST_GROWTH_GENE + size; i++)
        activation_map[i] = {};
    used_genes = size;
    for (auto &gene : activation_map) {
        for (unsigned short _ = 0; _ < gene_activation_length; _++)
            gene.second.push_back(getRandomGene(rng));
    }
}

void Genome::mutDup(std::mt19937 &rng) {
    if (size() >= max_size)
        return;

    for (auto &gene : std::unordered_map(activation_map)) {
        if (uniform_random(rng) > mut_dup)
            continue;

        char new_gene = '/';
        for (char c = FIRST_GROWTH_GENE; c < FIRST_GROWTH_GENE + used_genes; c++) {  //Recycles deleted genes (inefficient implementation)
            if (activation_map.find(c) == activation_map.end()) {
                new_gene = c;
                break;
            }
        }
        if (new_gene == '/')
            new_gene = used_genes++;
        activation_map.insert({new_gene, gene.second});
    }
}

//TODO: change so that sub_rate is applied per target gene
void Genome::mutSub(std::mt19937 &rng) {
    for (auto &gene : activation_map) {
        if (uniform_random(rng) > mut_sub)
            continue;

        char sub_gene;
        if (uniform_random(rng) < core_gene_substitution_chance) {
            std::uniform_int_distribution<> uniform_dir(0, core_genes.size() - 1);
            sub_gene = core_genes[uniform_dir(rng)];
        } else {
            sub_gene = getRandomGene(rng);
        }

        gene.second.at(short(uniform_random(rng) * gene_activation_length)) = sub_gene;
    }
}

void Genome::mutDel(std::mt19937 &rng) {
    if (activation_map.size() == 1)
        return;

    std::vector<char> to_remove;
    for (auto &gene : activation_map) {
        if (uniform_random(rng) > mut_del)
            continue;

        to_remove.push_back(gene.first);

        for (auto &gene2 : activation_map) {
            for (auto &target_gene : gene2.second) {
                if (target_gene == gene.first)
                    target_gene = '/';
            }
        }
    }

    for (auto &gene : to_remove)
        activation_map.erase(gene);
}

const char &Genome::getRandomGene(std::mt19937 &rng) const {
    std::uniform_int_distribution<> uniform_genome(0, (short) activation_map.size() - 1);
    return std::next(std::begin(activation_map), uniform_genome(rng))->first;
}
