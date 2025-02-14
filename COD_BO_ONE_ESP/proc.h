#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>


// Process 
DWORD GetProcId(const wchar_t* procName);

// Module
uintptr_t GetModuleBaseAddr(DWORD procId, const wchar_t* modName);

// Derefrence Pointer Chain
uintptr_t FinDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int>offsets);

namespace Memory
{

    template <typename T>
    const T RPM(HANDLE hProc, const uintptr_t& address)
    {
        T value = {};
        ReadProcessMemory(hProc, reinterpret_cast<const void*>(address), &value, sizeof(T), 0);
        return value;
    }

    template <typename U>
    const void WPM(HANDLE hProc, const uintptr_t& address, const U& value)
    {
        WriteProcessMemory(hProc, reinterpret_cast<void*>(address), &value, sizeof(U), 0);
    }
}
