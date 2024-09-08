//
// Created by alefe on 29/08/2024.
//

#include <stdexcept>
#include <sstream>
#include "genome.h"
#include "utility.h"

Genome::Genome(
    unsigned int size,
    unsigned int max_size,
    double mut_sub,
    double mut_dup,
    double mut_del,
    std::mt19937 &rng
) : max_size(max_size),
    mut_sub(mut_sub),
    mut_dup(mut_dup),
    mut_del(mut_del) {
    if (size > max_size)
        throw std::runtime_error("Max 'size' is " + std::to_string(max_size));

    for (unsigned int i = 0; i < size; i++)
        activation_map[geneIdToGeneString(used_genes++)] = {};
    for (auto &gene : activation_map) {
        for (unsigned int _ = 0; _ < gene_activation_length; _++)
            gene.second.push_back(getRandomGene(rng));
    }
}

void Genome::mutDup(std::mt19937 &rng) {
    if (size() >= max_size)
        return;

    ActivationMap to_add;
    for (auto &gene : activation_map) {
        if (uniform_random(rng) > mut_dup)
            continue;

        std::string new_gene;
        for (unsigned int c = 0; c < used_genes; c++) {  //Recycles deleted genes (inefficient implementation)
            auto s = geneIdToGeneString(c);
            if (activation_map.find(s) == activation_map.end()) {
                new_gene = s;
                break;
            }
        }
        if (new_gene.empty())
            new_gene = geneIdToGeneString(used_genes++);
        to_add.insert({new_gene, gene.second});
    }

    for (auto &gene : to_add) {
        activation_map.insert(gene);
    }
}

//TODO: change so that sub_rate is applied per target gene
void Genome::mutSub(std::mt19937 &rng) {
    for (auto &gene : activation_map) {
        if (uniform_random(rng) > mut_sub)
            continue;

        std::string sub_gene;
        if (uniform_random(rng) < core_gene_substitution_chance) {
            std::uniform_int_distribution<> uniform_dir(0, core_genes.size() - 1);
            sub_gene = core_genes[uniform_dir(rng)];
        } else {
            sub_gene = getRandomGene(rng);
        }

        gene.second.at(int(uniform_random(rng) * gene_activation_length)) = sub_gene;
    }
}

void Genome::mutDel(std::mt19937 &rng) {
    if (activation_map.size() == 1)
        return;

    std::vector<std::string> to_remove;
    for (auto &gene : activation_map) {
        if (uniform_random(rng) > mut_del)
            continue;

        to_remove.push_back(gene.first);

        for (auto &gene2 : activation_map) {
            for (auto &target_gene : gene2.second) {
                if (target_gene == gene.first)
                    target_gene = "";
            }
        }
    }

    for (auto &gene : to_remove)
        activation_map.erase(gene);
}

std::string Genome::geneIdToGeneString(unsigned int i) {
    char buffer[36];
    int index = 0;

    i++;
    while (i > 0) {
        i--;
        buffer[index++] = char('A' + (i % 26));  // Store the character for the current base-26 digit
        i /= 26;
    }
    buffer[index] = '\0';

    return buffer;
}

const std::string &Genome::getRandomGene(std::mt19937 &rng) const {
    std::uniform_int_distribution<> uniform_genome(0, (int) activation_map.size() - 1);
    return std::next(std::begin(activation_map), uniform_genome(rng))->first;
}

std::string Genome::stringRepresentation() const {
    std::stringstream gen_ss;
    for (auto &gene : activation_map) {
        gen_ss << gene.first << " -> ";
        for (unsigned int i = 0; i < gene_activation_length - 1; i++)
            gen_ss << gene.second.at(i) << " | ";
        gen_ss << gene.second.at(gene_activation_length - 1);
        gen_ss << "\n";
    }
    return gen_ss.str();
}

std::string Genome::translateGene(const std::string &gene) {
    if (isGrowthGene(gene)) {
        return "F";
    }
    return gene;
}
