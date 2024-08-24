#include <iostream>
#include <fstream>
#include <filesystem>
#include "tree.h"
#include "forest.h"
#include "params.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect number of arguments. Provide an output file for the plant body plan.\n";
        return EXIT_FAILURE;
    }

    auto outdir = std::string(argv[1]);
    if (!std::filesystem::create_directory(outdir))
        throw std::runtime_error("Directory " + outdir + " already exists.");

    Forest forest(
        N_POP,
        10,
        GROWTH_STEPS,
        0.05,
        0.005,
        0.005
    );

    for (int i = 0; i < GENS; i++) {
        forest.evolve();

        if (i % 100 != 0)
            continue;

        std::cout << "Generation: " << i << "\n";
        forest.print_stats();
        std::cout << "\n";
    }

    forest.fittest_ever.develop(forest.fittest_ever.maturity);
    std::cout << "Best fitness: " << forest.fittest_ever.fitness() << "\n";

    std::ofstream file;

    file.open(outdir + "/fittest_body.txt");
    file << vec_to_str(forest.fittest_ever.body, "") << "\n";
    file.close();

    file.open(outdir + "/fittest_genome.txt");
    file << forest.fittest_ever.genome_as_string();
    file.close();

    file.open(outdir + "/fittest_segments.obj");
    file << forest.fittest_ever.segments_as_OBJ();
    file.close();

    file.open(outdir + "/fittest_seeds.obj");
    file << forest.fittest_ever.seeds_as_OBJ();
    file.close();
}
