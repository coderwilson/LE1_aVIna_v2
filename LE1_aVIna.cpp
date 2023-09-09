/********************************************************************************************
* Need to fill something in here about the project.
* 
* 
* 
* 
********************************************************************************************/
#pragma once
#include <iostream>
#include "config/global_objects.h"
using namespace std;
using std::string;

// Area-specific includes
#include "area/eden_prime.h"


int main()
{
    // Opening statements as appropriate
    cout << "aVIna Start!\n";
    try {
        // Initialize logging??
        // Connect to memory
        memory_start();

        // Initialize controller
        xbox.connect();

        // Connect to config
        main_vars.gamestate = 10;
        main_vars.zone_progress = 0;
        cout << "Check gamestate: " << std::dec << main_vars.gamestate << "\n";

    }
    catch (string e) {
        cout << "Initialization error: " << e << "\n";
    }

    // Run TAS based on game state. Initially from configs, updating as the TAS completes each section.
    while (main_vars.gamestate != 999) {
        try {
            cout << "Gamestate ID: " << main_vars.gamestate << " | " << main_vars.gamestate_name() << std::endl;
            switch (main_vars.gamestate) {
                case 5:

                    main_vars.gamestate = 10;
                    break;
                case 10:
                    eden_prime_main();
                    main_vars.gamestate = 999;
                    break;
                case 15:
                    cout << "Section not yet programmed." << main_vars.gamestate_name() << std::endl;
                    main_vars.gamestate = 10;
                    break;
                case 20:
                    cout << "Section not yet programmed." << main_vars.gamestate_name() << std::endl;
                    main_vars.gamestate = 10;
                    break;
                case 30:
                    cout << "Section not yet programmed." << main_vars.gamestate_name() << std::endl;
                    main_vars.gamestate = 10;
                    break;
                default:
                    cout << "Unknown gamestate during run: " << main_vars.gamestate << "\n";
                    main_vars.gamestate = 999;
                    break;
            }
        }
        catch (string e) {
            cout << "Exception during run: " << e << "\n";
            cout << "Game state: " << main_vars.gamestate << ", progress " << main_vars.zone_progress << "\n";
            main_vars.gamestate = 999;
        }
        catch (int e) {
            cout << "Exception during run: " << e << "\n";
            cout << "Game state: " << main_vars.gamestate << ", progress " << main_vars.zone_progress << "\n";
            main_vars.gamestate = 999;
        }
    }

    // Wrap-up all objects
    // Disconnect controller gracefully.
    std::cout << "\nDisconnecting Xbox controller.\n";
    xbox.disconnect();
    std::cout << "Xbox disconnected.\n";

    // Disconnect memory
    std::cout << "\nDisconnecting from memory.\n";
    for (int i = 0; i < 5; i++) {
        std::cout << std::dec << memory.frame_pos() << std::endl;
        Sleep(500);

    }
    memory_end();
    std::cout << "Memory disconnected.\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu