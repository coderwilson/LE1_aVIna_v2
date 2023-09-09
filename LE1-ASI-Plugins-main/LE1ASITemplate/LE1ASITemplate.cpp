
#define GAMELE1
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "../Shared-ASI/Interface.h"
#include "../Shared-ASI/Common.h"
#include "../Shared-ASI/ME3Tweaks/ME3TweaksHeader.h"
#include "../LE1AppearanceTest/AppearanceHUD.h"
#include "TASHelpers.h"
#include <set>
#include "../LE1_aVIna_v2/controls/xbox.h"


#define MYHOOK "HookName_"

SPI_PLUGINSIDE_SUPPORT(L"HookName", L"1.0.0", L"AuthorName", SPI_GAME_LE1, 2);
SPI_PLUGINSIDE_POSTLOAD;
SPI_PLUGINSIDE_ASYNCATTACH;


AppearanceHUD* customHud = new AppearanceHUD;


// ProcessEvent hook
// ======================================================================

typedef void (*tProcessEvent)(UObject* Context, UFunction* Function, void* Parms, void* Result);
tProcessEvent ProcessEvent = nullptr;
tProcessEvent ProcessEvent_orig = nullptr;


void ProcessEvent_hook(UObject* Context, UFunction* Function, void* Parms, void* Result)
{
    static AppearanceHUD* hud = nullptr;
    if (!strcmp(Function->GetFullName(), "Function SFXGame.BioHUD.PostRender"))
    {
        auto biohud = reinterpret_cast<ABioHUD*>(Context);
        
        if (biohud != nullptr)
        {
            auto worldinfo = reinterpret_cast<AWorldInfo*>(Context);
            //_worldInfo = biohud->WorldInfo;
            customHud->Update(((ABioHUD*)Context)->Canvas, biohud);
            customHud->Draw();
        }
    }
    if (!strcmp(Function->GetFullName(), "Function SFXGame.BioWorldInfo.Tick"))
    {
        ABioWorldInfo* bioworldinfo = reinterpret_cast<ABioWorldInfo*>(Context);
        if (bioworldinfo != nullptr)
        {

            //wcout << bioworldinfo->CurrentGame->m_sMapName << "\n";
            //wcout << bioworldinfo->CurrentGame->m_sMapName.Data << "\n";

            //auto worldinfo = reinterpret_cast<AWorldInfo*>(Context);
            //_worldInfo = biohud->WorldInfo;
            //customHud->Update(((ABioHUD*)Context)->Canvas, biohud);
            //customHud->Draw();
        }
    }
    ProcessEvent_orig(Context, Function, Parms, Result);
}


SPI_IMPLEMENT_ATTACH
{

    brrfun();
    Common::OpenConsole();

	INIT_CHECK_SDK()

    INIT_FIND_PATTERN_POSTHOOK(ProcessEvent, /* 40 55 41 56 41 */ "57 48 81 EC 90 00 00 00 48 8D 6C 24 20");
    if (auto rc = InterfacePtr->InstallHook(MYHOOK "ProcessEvent", ProcessEvent, ProcessEvent_hook, (void**)&ProcessEvent_orig);
        rc != SPIReturn::Success)
    {
        writeln(L"Attach - failed to hook ProcessEvent: %d / %s", rc, SPIReturnToString(rc));
        return false;
    }

    return true;
}

SPI_IMPLEMENT_DETACH
{
    Common::CloseConsole();
    return true;
}