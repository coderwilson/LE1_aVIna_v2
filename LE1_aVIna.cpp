/********************************************************************************************
* 
* 
* 
* 
* 
********************************************************************************************/
#include <iostream>
#include "area/eden_prime.h"
#include "config/state.h"
using namespace std;
using std::string;


int main()
{
    tas_vars main_vars;
    try {
        // Initialize logging
        // Initialize controller
        // Connect to memory
        // Connect to config
        
        main_vars.gamestate = 10;
        main_vars.zone_progress = 0;
        cout << "Check gamestate: " << main_vars.gamestate << "\n";

    }
    catch (string e) {
        cout << "Initialization error: " << e << "\n";
    }

    // Opening statements as appropriate
    cout << "Hello World!\n";
    cout << "TEST\n";

    // Run TAS based on game state. Initially from configs, updating as the TAS completes each section.
    while (main_vars.gamestate != 999) {
        try {
            switch (main_vars.gamestate) {
                case 5:
                    cout << "Section not yet programmed." << main_vars.gamestate_name() << "\n";
                    cout << "Ready for Eden Prime.";
                    main_vars.gamestate = 10;
                    break;
                case 10:
                    eden_prime_main();
                    cout << "End of Eden Prime section.";
                    main_vars.gamestate = 999;
                    break;
                case 20:
                    cout << "Section not yet programmed." << main_vars.gamestate_name() << "\n";
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
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu