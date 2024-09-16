#include <iostream>
#include <fstream>
#include <filesystem>
#include "parameters.h"
#include "model.h"

// TODO: add a second terminal gene representing a leaf that determines how fast the plant develops (get rid of synchronous calls to 'tree.develop')
//       rewrite in rust for fun
//       make genes their own struct to simplify the mess with gene ids etc

int main() {
    Model model(Parameters {});
    model.run();
    model.forest.save_fittest(model.parameters.outdir);
}
