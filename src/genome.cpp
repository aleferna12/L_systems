//
// Created by alefe on 29/08/2024.
//

#include <stdexcept>
#include <sstream>
#include "genome.h"
#include "utility.h"

Genome::Genome(unsigned int size) {
    if (size > MAX_GENOME_SIZE)
        throw std::runtime_error("Max 'size' is " + std::to_string(MAX_GENOME_SIZE));

    for (int i = 0; i < size; i++)
        activation_map[geneIdToGeneString(used_genes++)] = {};
    for (auto &gene : activation_map) {
        for (int _ = 0; _ < GENE_ACTIVATION_LENGTH; _++)
            gene.second.push_back(getRandomGene());
    }
}

void Genome::mutDup(double dup_rate) {
    if (size() >= MAX_GENOME_SIZE)
        return;

    ActivationMap to_add;
    for (auto &gene : activation_map) {
        if (uniform_random(rng) > dup_rate)
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
void Genome::mutSub(double sub_rate) {
    for (auto &gene : activation_map) {
        if (uniform_random(rng) > sub_rate)
            continue;

        std::string sub_gene;
        if (uniform_random(rng) < CORE_GENE_SUBSTITUTION) {
            std::uniform_int_distribution<> uniform_dir(0, core_genes.size() - 1);
            sub_gene = core_genes[uniform_dir(rng)];
        } else {
            sub_gene = getRandomGene();
        }

        gene.second.at(int(uniform_random(rng) * GENE_ACTIVATION_LENGTH)) = sub_gene;
    }
}

void Genome::mutDel(double del_rate) {
    if (activation_map.size() == 1)
        return;

    std::vector<std::string> to_remove;
    for (auto &gene : activation_map) {
        if (uniform_random(rng) > del_rate)
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

const std::string &Genome::getRandomGene() const {
    std::uniform_int_distribution<> uniform_genome(0, (int) activation_map.size() - 1);
    return std::next(std::begin(activation_map), uniform_genome(rng))->first;
}

std::string Genome::stringRepresentation() const {
    std::stringstream gen_ss;
    for (auto &gene : activation_map) {
        gen_ss << gene.first << " -> ";
        for (int i = 0; i < GENE_ACTIVATION_LENGTH - 1; i++)
            gen_ss << gene.second.at(i) << " | ";
        gen_ss << gene.second.at(GENE_ACTIVATION_LENGTH - 1);
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