#pragma once
#include <iostream>
#include <Windows.h>

extern HANDLE hProc;

namespace mem
{
    // Change Instructions
    void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);

    // Nop Instrcutions
    void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);


    // ReadProcessMemory
    template <typename T>
    const T RPM(const uintptr_t& address)
    {
        T value = {};
        ReadProcessMemory(hProc, reinterpret_cast<const void*>(address), &value, sizeof(T), 0);
        return value;
    }

    // WriteProcessMemory
    template <typename U>
    const void WPM(const uintptr_t& address, const U& value)
    {
        WriteProcessMemory(hProc, reinterpret_cast<void*>(address), &value, sizeof(U), 0);
    }
}
