#include <iostream>
#include <utility>
#include <cmath>
#include "organism.h"

int main() {
    std::vector<std::string> seedling = {"A"};
    Genome genome;
    genome["A"] = {"[", "B", "]"};
    genome["B"] = {"y+", "x+", "A"};

    std::vector<Organism> pop;
    pop.reserve(1000);
    for (int i = 0; i < 1000; i++) {
            pop.emplace_back(seedling, genome);
    }

    Organism best = pop[0];
    double best_fitness = 0;
    for (int i = 0; i < 2000; i++) {
        std::cout << "Generation: " << i << std::endl;

        std::vector<std::pair<double, int>> fitness_organism;
        double tot_fitness = 0;
        for (int j = 0; j < pop.size(); j++) {
            auto &o = pop.at(j);
            o.grow(6);

            auto fitness = 1. + o.get_fitness();
            o.body = o.seedling;

            tot_fitness += fitness;
            fitness_organism.emplace_back(fitness, j);

            if (fitness > best_fitness) {
                best_fitness = fitness;
                best = Organism(o.seedling, o.genome);
            }
        }

        std::cout << "Mean get_fitness: " << tot_fitness / (double) fitness_organism.size() << "\n";

        std::vector<Organism> new_pop;
        for (int j = 0; j < pop.size(); j++) {
            bool picked_cell = false;
            double rnd = tot_fitness * uniform_random(rng);
            for (auto &f: fitness_organism) {
                if (rnd < f.first) {
                    auto &o = pop[f.second];
                    new_pop.emplace_back(o.seedling, o.genome);
                    picked_cell = true;
                    break;
                }
                rnd -= f.first;
            }

            if (!picked_cell)
                throw std::runtime_error("Failed to select organism based on get_fitness");
        }

        for (auto &o : new_pop) {
            o.mutate(0.05, 0.001, 0.0008);
        }

        pop = new_pop;
    }

    best.grow(6);
    best.get_fitness();
    std::cout << best.body_as_string() << "\n";
    std::cout << best.translated_body() << "\n";
    best.print_genome();
}
