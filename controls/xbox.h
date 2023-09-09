#pragma once
#define WIN32_LEAN_AND_MEAN
#include "../config/global_objects.h"
#include <windows.h>
#include <Xinput.h>
#include <ViGEm/Client.h>
#pragma comment(lib, "setupapi.lib")
using std::string;

// https://github.com/ViGEm/ViGEmClient/blob/master/README.md

class xbox_class {
public:
    PVIGEM_CLIENT client;
    VIGEM_ERROR retval;
    PVIGEM_TARGET pad;
    VIGEM_ERROR pir;
    XINPUT_STATE state;

    int connect() {
        std::cout << "\nConnecting Virtual Xbox controller\n";
        client = vigem_alloc();
        if (client == nullptr)
        {
            std::cerr << "Uh, not enough memory to do that?!" << std::endl;
            return -1;
        }
        else
        {
            std::cout << "Client\n";
            std::cout << client << std::endl;
        }

        retval = vigem_connect(client);

        if (!VIGEM_SUCCESS(retval))
        {
            std::cerr << "ViGEm Bus connection failed with error code: 0x" << std::hex << retval << std::endl;
            return -1;
        }
        else
        {
            std::cout << "Xbox Connection: " << retval << std::endl;
        }
        memory.wait_frames(1);

        pad = vigem_target_x360_alloc();
        pir = vigem_target_add(client, pad);

        if (!VIGEM_SUCCESS(pir))
        {
            std::cerr << "Target plugin failed with error code: 0x" << std::hex << pir << std::endl;
            return -1;
        }
        XInputGetState(0, &state);
        std::cout << "Virtual X-box controller has now connected.\n";
        return 0;
    }
    int neutral() {
        state.Gamepad.sThumbLX = 0;
        state.Gamepad.sThumbLY = 0;
        state.Gamepad.sThumbRX = 0;
        state.Gamepad.sThumbRY = 0;
        state.Gamepad.wButtons = 0x0000;
        state.Gamepad.bLeftTrigger = 0;
        state.Gamepad.bRightTrigger = 0;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        memory.wait_frames(1);
        return 0;
    }
    int disconnect() {
        vigem_target_remove(client, pad);
        vigem_target_free(pad);
        std::cout << "Client disconnect complete.";
        return 0;
    }

    void test() {
        std::cout << "Ready\n";
        /*
        Sleep(100);
        neutral();
        Sleep(1000);
        state.Gamepad.wButtons = 0x0001;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Dpad up";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0x0002;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Dpad down";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0x0004;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Dpad left";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0x0008;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Dpad right";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0x0010;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Start";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0x0020;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Back";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0x0040;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "L-Thumb";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0x0080;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "R-Thumb";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0x0100;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "L-Shoulder";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0x0200;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "R-Shoulder";
        std::cout << std::endl;
        Sleep(1000);
        //state.Gamepad.wButtons = 0x0400;
        //vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        //std::cout << "Guide button";
        //std::cout << std::endl;
        neutral();
        Sleep(1000);
        std::cout << "A";
        std::cout << std::endl;
        a();
        Sleep(1000);
        std::cout << "B";
        std::cout << std::endl;
        b();
        Sleep(1000);
        std::cout << "X";
        std::cout << std::endl;
        x();
        Sleep(1000);
        std::cout << "Y";
        std::cout << std::endl;
        y();
        Sleep(1000);
        state.Gamepad.wButtons = 0x3003;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Multiple buttons 1";
        std::cout << std::endl;
        Sleep(1000);
        state.Gamepad.wButtons = 0xC005;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Multiple buttons 2";
        std::cout << std::endl;
        Sleep(1000);
        neutral();
        zoom_in();
        std::cout << "Left Trigger";
        std::cout << std::endl;
        Sleep(1000);
        zoom_out();
        state.Gamepad.bRightTrigger = 0xff;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Right Trigger";
        std::cout << std::endl;
        */
        Sleep(1000);
        state.Gamepad.bRightTrigger = 0x0;
        state.Gamepad.sThumbLX = 32767;
        state.Gamepad.sThumbLY = 32767;
        state.Gamepad.sThumbRX = -32767;
        state.Gamepad.sThumbRY = 32767;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Stick movement 1";
        std::cout << std::endl;
        Sleep(500);
        neutral();
        Sleep(500);
        state.Gamepad.sThumbLX = -32767;
        state.Gamepad.sThumbLY = -32767;
        state.Gamepad.sThumbRX = 32767;
        state.Gamepad.sThumbRY = -32767;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Stick movement 2";
        std::cout << std::endl;
        Sleep(500);
        neutral();
        Sleep(500);
        state.Gamepad.sThumbLX = -32767;
        state.Gamepad.sThumbLY = 32767;
        state.Gamepad.sThumbRX = -32767;
        state.Gamepad.sThumbRY = -32767;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Stick movement 3";
        std::cout << std::endl;
        Sleep(500);
        neutral();
        Sleep(500);
        state.Gamepad.sThumbLX = 32767;
        state.Gamepad.sThumbLY = -32767;
        state.Gamepad.sThumbRX = 32767;
        state.Gamepad.sThumbRY = 32767;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        std::cout << "Stick movement 4";
        std::cout << std::endl;
        Sleep(500);
        neutral();
        std::cout << "Test complete.";
        Sleep(2000);
    }

    void tap_a() {
        state.Gamepad.wButtons = 0x1000;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        memory.wait_frames(1);
        neutral();
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        memory.wait_frames(1);
    }
    void tap_b() {
        state.Gamepad.wButtons = 0x2000;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        memory.wait_frames(1);
        neutral();
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        memory.wait_frames(1);
    }
    void tap_x() {
        state.Gamepad.wButtons = 0x4000;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        memory.wait_frames(1);
        neutral();
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        memory.wait_frames(1);
    }
    void tap_y() {
        state.Gamepad.wButtons = 0x8000;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        memory.wait_frames(1);
        neutral();
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        memory.wait_frames(1);
    }
    void set_movement(float x, float y) {
        // Expectation is that x and y are from -1 to 1, representing the
        // percentage of stick push in a given direction.
        // Example, fully up and right would be 1, 1.
        int x_val = (32767 * x);
        int y_val = (32767 * y);
        state.Gamepad.sThumbLX = x_val;
        state.Gamepad.sThumbLY = y_val;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
    }
    void set_aim(float x, float y) {
        // Expectation is that x and y are from -1 to 1, representing the
        // percentage of stick push in a given direction.
        // Example, fully up and right would be 1, 1.
        int x_val = (32767 * x);
        int y_val = (32767 * y);
        state.Gamepad.sThumbRX = x_val;
        state.Gamepad.sThumbRY = y_val;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
    }
    void zoom_in() {
        state.Gamepad.bLeftTrigger = 0xff;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
    }
    void zoom_out() {
        state.Gamepad.bLeftTrigger = 0;
        vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
    }
    void run() {
        // std::cout << (state.Gamepad.wButtons & (1 << 12)) << std::endl;
        if ((state.Gamepad.wButtons & (1 << 12)) == 0) {
            std::cout << "Running\n";
            state.Gamepad.wButtons += 0x1000;
            vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        }
    }
    void walk() {
        // std::cout << (state.Gamepad.wButtons & (1 << 12)) << std::endl;
        if ((state.Gamepad.wButtons & (1 << 12)) == 0x1000) {
            std::cout << "Walking\n";
            state.Gamepad.wButtons -= 0x1000;
            vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));
        }
    }
    void grenade() {
        tap_x();
    }
    void dialog(int alignment = 0, int hemisphere = 1) {
        // alignment 1 is up, 0 is neutral, -1 is down. AKA for paragon/renegade.
        // hemisphere 1 for right, 0 or -1 for left.
        int x = 1;
        if (hemisphere != 1) x = -1;
        set_movement(x, alignment);
        tap_a();
        tap_x();

    }
};
