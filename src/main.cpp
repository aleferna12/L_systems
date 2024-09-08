#include <iostream>
#include <fstream>
#include <filesystem>
#include "parameters.h"
#include "model.h"

int main() {
    Model model(Parameters {});
    model.run();
    model.forest.save_fittest(model.parameters.outdir);
}
