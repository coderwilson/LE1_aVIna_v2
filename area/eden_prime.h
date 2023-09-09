#pragma once
#include <iostream>
#include "../config/global_objects.h"
// memory_class memory;


int to_ash()
{
    int forward = 1;
    int checkpoint = 0;
    while (checkpoint < 28) { // Replace with story progress later.
        if (memory.user_control()) {
            switch (checkpoint) {
            case 1: case 2: case 5: case 9: case 10: case 14: case 15: case 21: case 26:
                xbox.run();
                forward = 0;
                break;
            default:
                xbox.walk();
                forward = 1;
                break;
            }
            if (path.engage(checkpoint, forward) == 0) {
                std::cout << "Checkpoint reached: " << checkpoint << std::endl;
                checkpoint += 1;
            }
        }
        else {
            if (checkpoint == 26 || checkpoint == 27) {
                xbox.dialog();
            }
            else {
                xbox.neutral();
                xbox.tap_a();
            }
        }
    }
    std::cout << "Hello Ash!\n";
    return 0;
}


int eden_prime_main(int section = 0)
{
    if (section == 0) {

        section = 1;
    }
    if (section == 1) {
        to_ash();
    }
    return 0;
}
