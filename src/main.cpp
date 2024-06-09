#include <iostream>
#include <fstream>
#include "plant.h"
#include "environment.h"

const int GROWTH_STEPS = 7;
const int N_POP = 1000;
const int GENS = 3000;

int main(int argc, char *argv[]) {
    Environment env(
        N_POP,
        5,
        GROWTH_STEPS,
        0.05,
        0.005,
        0.
    );

    for (int i = 0; i < GENS; i++) {
        env.evolve();

        std::cout << "Generation :" << i << "\n";
        env.print_stats();
        std::cout << "\n";
    }

    env.fittest_ever.develop(env.fittest_ever.maturity);
    std::cout << vec_to_str(env.fittest_ever.body) << "\n";

    std::ofstream file;
    file.open("./out.txt");
    file << vec_to_str(env.fittest_ever.translated_body()) << "\n";

    env.fittest_ever.print_genome();
}
