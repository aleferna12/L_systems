#include <iostream>
#include "pico/stdlib.h"
#include "parameters.h"
#include "model.h"

// TODO:
//  ? add a second terminal gene representing a leaf that determines how fast the plant develops (get rid of synchronous calls to 'tree.develop')
//  * test memory limits, seems like we have some memory left to spare (increase population size and genome size)
//  * decide whether to mount a filesystem on the flash memory or get rid of all data saving features
int main() {
    stdio_init_all();  // Initialize serial communication
    // TODO: remove
    sleep_ms(500);  // Give it sometime to connect

    Model model(Parameters {});
    model.run();
    // model.saveData();
}
