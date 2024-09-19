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
#include <memory>
#include <random>
#include "parameters.h"

using ActivationMap = std::unordered_map<std::string, std::vector<std::string>>;

class Genome {
public:
    //! Creates a randomized genome of size 'size'.
    Genome(unsigned int size, unsigned int max_size, double mut_sub, double mut_dup, double mut_del,
           unsigned int gene_activation_length, std::mt19937 &rng);

    size_t size() const {
        return activation_map.size();
    }

    const std::string &getRandomGene(std::mt19937 &rng) const;

    //! Forgive me, gods, for I have used pointers (there is no std::optional(&T) though, so not my fault)
    const std::vector<std::string> *geneActivates(const std::string &gene) const {
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

    std::string stringRepresentation() const;

    //! Returns the gene back if its a core gene and "F" otherwise.
    static std::string translateGene(const std::string &gene);

    static bool isGrowthGene(const std::string &gene) {
        return std::find(core_genes.begin(), core_genes.end(), gene) == core_genes.end();
    }

    unsigned int max_size;
    double mut_sub;
    double mut_dup;
    double mut_del;
    unsigned int gene_activation_length;

    double core_gene_substitution_chance = 0.5;
    //TODO: should this be an unordered_set? benchmark
    static constexpr std::array core_genes = {"x+", "x-", "y+", "y-", "*", "[", "]"};

private:
    void mutDup(std::mt19937 &rng);

    void mutSub(std::mt19937 &rng);

    void mutDel(std::mt19937 &rng);

    static std::string geneIdToGeneString(unsigned int i);

    unsigned int used_genes = 0;
    ActivationMap activation_map;
};


#endif //L_SYSTEMS_GENOME_H
