#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <psapi.h>

class memory_class {
private:
    std::wstring exeName = L"MassEffect1.exe";
    DWORD process_id;
    HANDLE processHandle;
    HANDLE base_addr;

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
                    if (wstrModName.find(wstrModContain) != string::npos)
                    {
                        CloseHandle(pHandle); // Check this later, maybe should be removed.
                        return hMods[i];
                    }
                }
            }
        }
        return nullptr;
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
        base_addr = (HANDLE)mi.lpBaseOfDll;
        /*
        unsigned long long int base_addr_temp = (unsigned long long int)&processHandle;
        LPVOID buffer;
        SIZE_T len;
        ReadProcessMemory(processHandle, &processHandle, &buffer, 8, &len);
        std::cout << "Module base address: " << std::hex << buffer << std::endl;
        // base_addr = (unsigned long long int) & processHandle;
        HANDLE Process_Value = read_bytes(processHandle);
        base_addr = 0x7ff7d87c0000;
        */
        std::cout << "Module base address: " << std::hex << &base_addr << std::endl;
        Sleep(2000);
        return 0;
    }

    HANDLE read_bytes(HANDLE base, int offset=0, int bytes=4) {
        LPVOID buffer;
        SIZE_T len = 0;
        int value = (int) base + offset;

        ReadProcessMemory(base, &value, &buffer, bytes, &len);
        return (HANDLE) buffer;
    }
    unsigned long long int follow_pointer(HANDLE base) {
        LPVOID buffer;
        SIZE_T len = 0;

        ReadProcessMemory(base, &base, &buffer, 8, &len);
        return (unsigned long long int) buffer;
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
        int frames = (int)read_bytes((HANDLE) &base_addr, 0x16C88A8);
        std::cout << frames << std::endl;
        return frames;
    }

};
/*
int main() {


    uintptr_t baseAddress = 0x1000000; // Replace with the base address of the data
    size_t offsetForInt = 0x1234;      // Replace with the offset for int
    size_t offsetForFloat = 0x5678;    // Replace with the offset for float
    size_t offsetForLong = 0x9ABC;     // Replace with the offset for long
    size_t offsetForString = 0xCDEF;   // Replace with the offset for string

    int intValue = 0;
    float floatValue = 0.0f;
    long longValue = 0;
    std::wstring stringValue;

    // Read int value
    if (ReadDataFromProcess(processHandle, baseAddress, offsetForInt, intValue)) {
        std::cout << "Read int value: " << intValue << std::endl;
    }

    // Read float value
    if (ReadDataFromProcess(processHandle, baseAddress, offsetForFloat, floatValue)) {
        std::cout << "Read float value: " << floatValue << std::endl;
    }

    // Read long value
    if (ReadDataFromProcess(processHandle, baseAddress, offsetForLong, longValue)) {
        std::cout << "Read long value: " << longValue << std::endl;
    }

    // Read string value
    stringValue = ReadWStringFromProcess(processHandle, baseAddress + offsetForString);
    std::wcout << L"Read string value: " << stringValue << std::endl;

}
*/