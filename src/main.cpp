#include <iostream>
#include <fstream>
#include <filesystem>
#include "parameters.h"
#include "model.h"

// TODO: fixing growth logic broke the whole model, we need to make sure tree cant grow downwards
//       rewrite in rust for fun
//       make genes their own struct to simplify the mess with gene ids etc
//       add a second terminal gene representing a leaf that determines how fast the plant develops (get rid of synchronous calls to 'tree.develop')
//       add grid structure to forest and rework IO to get OBJ files of the whole forest

int main() {
    Model model(Parameters {});
    model.run();
    model.saveData();
}
