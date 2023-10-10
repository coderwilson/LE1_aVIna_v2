#pragma once
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <psapi.h>
#include <iomanip>
#include "LE1-ASI-Plugins-main/LE1-SDK/SdkHeaders.h"

class memory_class {
private:
    // For some reason, the two different places this is used need it in different string types.
    std::wstring exeName = L"MassEffect1.exe";
    wchar_t* exeName2 = L"MassEffect1.exe";

    DWORD process_id;
    HANDLE processHandle;
    unsigned long long int base_addr;

    DWORD GetProcessIdFromExeName(const std::wstring& exeName) {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        // Create a snapshot of the running processes
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshot == INVALID_HANDLE_VALUE) {
            std::cerr << "Error creating process snapshot. Error code: " << GetLastError() << std::endl;
            return 0;
        }

        // Iterate through the processes to find a match
        if (Process32First(snapshot, &processEntry)) {
            do {
                if (_wcsicmp(processEntry.szExeFile, exeName.c_str()) == 0) {
                    // Found a process with a matching executable name
                    CloseHandle(snapshot);
                    return processEntry.th32ProcessID;
                }
            } while (Process32Next(snapshot, &processEntry));
        }

        // No process with the given name was found
        CloseHandle(snapshot);
        return 0;
    }
    template <typename T>
    bool ReadDataFromProcess(HANDLE processHandle, uintptr_t baseAddress, size_t offset, T& value) {
        uintptr_t addressToRead = baseAddress + offset;
        return ReadProcessMemory(processHandle, (LPCVOID)addressToRead, &value, sizeof(value), NULL);
    }
    HMODULE find_module(HANDLE pHandle) {
        HMODULE hMods[1024];
        DWORD cbNeeded;
        unsigned int i;

        if (EnumProcessModules(pHandle, hMods, sizeof(hMods), &cbNeeded))
        {
            for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
                TCHAR szModName[MAX_PATH];
                if (GetModuleFileNameEx(pHandle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
                {
                    std::wstring wstrModName = szModName;
                    //you will need to change this to the name of the exe of the foreign process
                    std::wstring wstrModContain = exeName;
                    if (wstrModName.find(wstrModContain) != std::string::npos)
                    {
                        CloseHandle(pHandle); // Check this later, maybe should be removed.
                        return hMods[i];
                    }
                }
            }
        }
        return nullptr;
    }
    unsigned long long int GetModuleBase() {
        const wchar_t* ModuleName = exeName2;
        DWORD ProcessId = process_id;
        // This structure contains lots of goodies about a module
        MODULEENTRY32 ModuleEntry = { 0 };
        // Grab a snapshot of all the modules in the specified process
        HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessId);

        if (!SnapShot)
            return NULL;

        // You have to initialize the size, otherwise it will not work
        ModuleEntry.dwSize = sizeof(ModuleEntry);

        // Get the first module in the process
        if (!Module32First(SnapShot, &ModuleEntry))
            return NULL;

        do {
            // Check if the module name matches the one we're looking for
            if (!wcscmp(ModuleEntry.szModule, ModuleName)) {
                // If it does, close the snapshot handle and return the base address
                CloseHandle(SnapShot);
                std::cout << "\n|| " << std::hex << (unsigned long long int)ModuleEntry.modBaseAddr << " ||\n";
                return (unsigned long long int)ModuleEntry.modBaseAddr;
            }
            // Grab the next module in the snapshot
        } while (Module32Next(SnapShot, &ModuleEntry));

        // We couldn't find the specified module, so return NULL
        CloseHandle(SnapShot);
        return NULL;
    }

public:
    int connect() {
        process_id = GetProcessIdFromExeName(exeName);
        processHandle = OpenProcess(PROCESS_VM_READ, FALSE, process_id);
        if (processHandle == NULL) {
            std::cerr << "Failed to open process. Error code: " << GetLastError() << std::endl;
            return -1;
        }
        std::cout << "Process launched successfully.";
        std::cout << "PROCESS: " << &processHandle << std::endl;
        HMODULE temp_module = find_module(processHandle);
        MODULEINFO mi;
        GetModuleInformation(processHandle, temp_module, &mi, sizeof(mi));
        base_addr = GetModuleBase();
        std::cout << "Module base address: " << std::hex << &base_addr << std::endl;
        std::cout << "Next to connect to SdkHeaders\n";
        Sleep(500);
        SDKInitializer::Instance();
        /*
        auto names = SDKInitializer::Instance()->GetBioNamePools();
        std::cout << "Names initialized: " << typeid(names).name() << std::endl;
        auto objects = SDKInitializer::Instance()->GetObjects();
        std::cout << "Objects initialized: " << objects->Count  << " | " << typeid(objects).name() << std::endl;
        std::cout << "SdkHeaders success: \n";
        
        for (int i = 0; i < objects->Count; i++)
        {
            auto obj = objects->Data[i];
            std::cout << obj->GetFullName() << std::endl;
            if (obj && obj->Outer)
            {
                // Must not be null and must have an outer (otherwise it's already an object at the root of the hierarchy, which should already be rooted... in theory...)
                UObject* outerMost = obj;
                while (outerMost->Outer != nullptr)
                {
                    outerMost = outerMost->Outer; // Go to it's nullptr
                }
                if (outerMost == package)
                {
                    // It's a child of this package!
                    //writeln(L"Rooting startup package object %hs in package %hs", obj->GetFullName(), package->Name.GetName());
                    //RootObject(obj);
                    
                }
            }
        }
        */
        //int hp_test = objects->Health;
        //std::cout << hp_test;
        
        Sleep(5000);
        return 0;
    }

    unsigned long long int read_bytes(unsigned long long int value, int bytes=4) {
        unsigned long long int buffer;
        SIZE_T len = 0;

        ReadProcessMemory(processHandle, (void*)value, &buffer, bytes, &len);
        return buffer;
    }
    float read_float(unsigned long long int value) {
        unsigned long long int x = read_bytes(value, 4);
        //std::cout << std::setprecision(4) << "Raw Float: " << x << std::endl;
        float f;
        *(__int64*)&f = x;
        // memcpy(&f, (const void*)x, 4);
        // std::cout << std::setprecision(4) << "x: " << x << " | f: " << f << std::endl;
        return f;
    }
    int disconnect() {
        try {
            CloseHandle(processHandle);
            return 0;
        }
        catch (int e) {
            (void)e;
            return -1;
        }
    }
    int frame_pos() {
        unsigned long long int frame_addr = base_addr + 0x16C88A8;
        int frames = (int)read_bytes(frame_addr);
        return frames;
    }
    void wait_frames(int wait_amount) {
        int end_frame = frame_pos() + wait_amount;
        while (frame_pos() < end_frame) {}
        return;
    }
    ABioPawn* connect_shep() {
        unsigned long long int key = base_addr + 0x1782300;
        unsigned long long int ptr1 = read_bytes(key, 8);
        unsigned long long int ptr2 = read_bytes(ptr1 + 0x0, 8);
        unsigned long long int ptr3 = read_bytes(ptr2 + 0x40, 8);
        std::cout << "PTR3 test: " << ptr3 << std::endl; // At least we know ptr3 is correct.
        ABioPawn* Shep = reinterpret_cast<ABioPawn*>(ptr3); // Not pulling the class out of memory... yet
        std::cout << "IsA test: " << Shep->IsA(ABioPawn::StaticClass()) << std::endl; // Crashes here.
        //std::cout << "Test: " << Shep.WalkSpeed << std::endl;
        return Shep;
    }
    float* shep_coords() {
        float retval[3];
        unsigned long long int key = base_addr + 0x1782300;
        unsigned long long int ptr1 = read_bytes(key, 8);
        unsigned long long int ptr2 = read_bytes(ptr1 + 0x0, 8);
        unsigned long long int ptr3 = read_bytes(ptr2 + 0x40, 8);
        retval[0] = read_float(ptr3 + 0x108);
        retval[1] = read_float(ptr3 + 0x10c);
        retval[2] = read_float(ptr3 + 0x110);
        return retval;
    }
    float* cam_coords() {
        float retval[6];  // x, y, z, north, east, vertical.
        retval[0] = read_float(base_addr + 0x1782260);
        retval[1] = read_float(base_addr + 0x1782264);
        retval[2] = read_float(base_addr + 0x1782268);
        retval[3] = read_float(base_addr + 0x1649898);
        retval[4] = read_float(base_addr + 0x16498a0);
        retval[5] = read_float(base_addr + 0x164989C);
        return retval;
    }
    bool user_control() {
        unsigned long long int key = base_addr + 0x167EE30;
        int var_check = read_bytes(key, 1);
        // std::cout << var_check << " | " << read_bytes(key, 1) << std::endl;
        std::cout << var_check << std::endl;
        if (var_check == 7) return true;
        return false;
    }
};
