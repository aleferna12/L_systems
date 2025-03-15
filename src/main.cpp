#include <iostream>
#include <fstream>
#include <filesystem>
#include "parameters.h"
#include "model.h"

// TODO: make genes their own struct to simplify the mess with gene ids etc
//       add a second terminal gene representing a leaf that determines how fast the plant develops (get rid of synchronous calls to 'tree.develop')

int main() {
    Model model(Parameters {});
    model.run();
    model.saveData();
}
