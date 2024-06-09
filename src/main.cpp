#include <iostream>
#include <fstream>
#include "plant.h"
#include "environment.h"

const int GROWTH_STEPS = 7;
const int N_POP = 1000;
const int GENS = 500;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect number of arguments. Provide an output file for the plant body plan.\n";
        return EXIT_FAILURE;
    }

    Environment env(
        N_POP,
        10,
        GROWTH_STEPS,
        0.05,
        0.005,
        0.005
    );

    for (int i = 0; i < GENS; i++) {
        env.evolve();

        if (i % 100 != 0)
            continue;

        std::cout << "Generation :" << i << "\n";
        env.print_stats();
        std::cout << "\n";
    }

    env.fittest_ever.develop(env.fittest_ever.maturity);
    std::cout << vec_to_str(env.fittest_ever.body, std::string()) << "\n";

    std::ofstream file;
    file.open(argv[1]);
    file << vec_to_str(env.fittest_ever.translated_body(), ",") << "\n";

    env.fittest_ever.print_genome();
    std::cout << env.fittest_ever.body_as_OBJ() << "\n";
    std::cout << env.fittest_ever.seeds.size() << "\n";
}
