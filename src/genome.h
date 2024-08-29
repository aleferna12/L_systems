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
#include "params.h"

using ActivationMap = std::unordered_map<std::string, std::vector<std::string>>;

class Genome {
public:
    Genome() = default;

    //! Creates a randomized genome of size 'size'.
    explicit Genome(unsigned int size);

    size_t size() const {
        return activation_map.size();
    }

    const std::string &getRandomGene() const;

    //! Forgive me, gods, for I have used pointers (const pointer though, maybe that's fine)
    const std::vector<std::string> *geneActivates(const std::string &gene) const {
        auto search = activation_map.find(gene);
        if (search == activation_map.end())
            return nullptr;
        return &search->second;
    }

    void mutate(double sub_rate, double dup_rate, double del_rate) {
        mutSub(sub_rate);
        mutDel(del_rate);
        mutDup(dup_rate);
    }

    std::string stringRepresentation() const;

    //! Returns the gene back if its a core gene and "F" otherwise.
    static std::string translateGene(const std::string &gene);

    static bool isGrowthGene(const std::string &gene) {
        return std::find(core_genes.begin(), core_genes.end(), gene) == core_genes.end();
    }

    //TODO: should this be an unordered_set? benchmark
    static constexpr std::array core_genes = {"x+", "x-", "y+", "y-", "*", "[", "]"};

private:
    static std::string geneIdToGeneString(unsigned int i);

    void mutDup(double dup_rate);

    void mutSub(double sub_rate);

    void mutDel(double del_rate);

    unsigned int used_genes = 0;

    ActivationMap activation_map;

    //TODO: should this be an unordered_set? benchmark
    static constexpr std::array gene_chars = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                                              'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
};


#endif //L_SYSTEMS_GENOME_H
