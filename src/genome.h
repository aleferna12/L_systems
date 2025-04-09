//
// Created by alefe on 29/08/2024.
//

#ifndef L_SYSTEMS_GENOME_H
#define L_SYSTEMS_GENOME_H

#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include "parameters.h"

const char FIRST_GROWTH_GENE = 48;

using ActivationMap = std::unordered_map<char, std::vector<char>>;

class Genome {
public:
    //! Creates a randomized genome of size 'size'.
    Genome(char size, uint16_t max_size, float mut_sub, float mut_dup, float mut_del,
           uint16_t gene_activation_length, std::mt19937 &rng);

    size_t size() const {
        return activation_map.size();
    }

    const char &getRandomGene(std::mt19937 &rng) const;

    //! Forgive me, gods, for I have used pointers (there is no std::optional(&T) though, so not my fault)
    const std::vector<char> *geneActivates(const char gene) const {
        auto search = activation_map.find(gene);
        if (search == activation_map.end())
            return nullptr;
        return &search->second;
    }

    void mutate(std::mt19937 &rng) {
        mutSub(rng);
        mutDel(rng);
        mutDup(rng);
    }

    uint16_t max_size;
    float mut_sub;
    float mut_dup;
    float mut_del;
    uint16_t gene_activation_length;

    float core_gene_substitution_chance = 0.5;
    //TODO: should this be an unordered_set? benchmark
    static constexpr std::array core_genes = {'+', '-', '*', '[', ']'};

private:
    void mutDup(std::mt19937 &rng);

    void mutSub(std::mt19937 &rng);

    void mutDel(std::mt19937 &rng);

    char used_genes = 0;
    ActivationMap activation_map;
};


#endif //L_SYSTEMS_GENOME_H
