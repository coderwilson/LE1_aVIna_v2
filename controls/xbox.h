#include <Xinput.h>
#include <ViGEm/Client.h>

// https://github.com/ViGEm/ViGEmClient/blob/master/README.md

const auto client = vigem_alloc();

if (client == nullptr)
{
    std::cerr << "Uh, not enough memory to do that?!" << std::endl;
    return -1;
}

const auto retval = vigem_connect(client);

if (!VIGEM_SUCCESS(retval))
{
    std::cerr << "ViGEm Bus connection failed with error code: 0x" << std::hex << retval << std::endl;
    return -1;
}

const auto pad = vigem_target_x360_alloc();
const auto pir = vigem_target_add(client, pad);

if (!VIGEM_SUCCESS(pir))
{
    std::cerr << "Target plugin failed with error code: 0x" << std::hex << pir << std::endl;
    return -1;
}

XINPUT_STATE state;

XInputGetState(0, &state);

vigem_target_x360_update(client, pad, *reinterpret_cast<XUSB_REPORT*>(&state.Gamepad));

vigem_target_remove(client, pad);
vigem_target_free(pad);