#pragma once
#include "../memory/core.h"
memory_class memory;
#include "../controls/xbox.h"
xbox_class xbox;
#include "state.h"
tas_vars main_vars;
#include "../pathing/core.h"
path_class path;

void memory_start() {
    int wait_counter = 0;
    while (memory.connect() != 0) {
        std::cout << "Memory did not initialize. Please make sure the game is running. - ";
        std::cout << wait_counter << std::endl;
        wait_counter += 1;
        Sleep(1000);
    }
}

void memory_end() {
    memory.disconnect();
}