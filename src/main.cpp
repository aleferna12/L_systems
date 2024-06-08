#include <iostream>
#include <utility>
#include <cmath>
#include <fstream>
#include "organism.h"

const int GROWTH_STEPS = 10;
const int N_POP = 1000;
const int GENS = 1000;

int main(int argc, char *argv[]) {
    std::vector<std::string> seedling = {"A"};

    std::vector<Organism> pop;
    for (int i = 0; i < N_POP; i++) {
        Organism o(seedling, {});
        o.randomize_genome(10);
        pop.push_back(o);
    }

    Organism best = pop[0];
    double best_fitness = 0;
    for (int i = 0; i < GENS; i++) {
        std::cout << "Generation: " << i << std::endl;

        std::vector<std::pair<double, int>> fitness_organism;
        double tot_fitness = 0;
        int tot_genome_size = 0;
        for (int j = 0; j < pop.size(); j++) {
            auto &o = pop.at(j);
            o.grow(GROWTH_STEPS);

            auto fitness = 1. + o.get_fitness();
            fitness *= fitness;  // This helps a lot
            o.body = o.seedling;

            tot_fitness += fitness;
            tot_genome_size += (int) o.genome.size();
            fitness_organism.emplace_back(fitness, j);

            if (fitness > best_fitness) {
                best_fitness = fitness;
                best = Organism(o.seedling, o.genome);
            }
        }

        std::cout << "Mean fitness: " << tot_fitness / (double) fitness_organism.size() << "\n";
        std::cout << "Mean genome size: " << tot_genome_size / (double) fitness_organism.size() << "\n\n";

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
            o.mutate(0.05, 0.005, 0.005);
        }

        pop = new_pop;
    }

    best.grow(GROWTH_STEPS);
    best.get_fitness();
    std::cout << vec_to_str(best.body) << "\n";

    std::ofstream file;
    file.open("../out.txt");
    file << vec_to_str(best.translated_body()) << "\n";

    best.print_genome();
}
