#pragma once

// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../controls/xbox.h"


int to_ash()
{
    std::cout << "Hello Ash!\n";
    std::cout << "TEST2\n";
    return 0;
}


int eden_prime_main(int section = 0)
{
    if (section == 0) {
        to_ash();
    }
    return 0;
}