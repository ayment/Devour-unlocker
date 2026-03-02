#include <MinHook.h>
#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <string>
#include <psapi.h>
#include "Il2cpp_Resolver/il2cpp_resolver.hpp"
#include "Lists.hpp"
#include "Functions.h"
#include "sdk.h"

bool DevourE()
{
    wchar_t exeName[MAX_PATH];
    DWORD size = MAX_PATH;
    if (GetModuleBaseNameW(GetCurrentProcess(), NULL, exeName, size))
    {
        return _wcsicmp(exeName, L"DEVOUR.exe") == 0;
    }
    return false;
}


bool WaitForIl2Cpp(int maxAttempts = 50, int delayMs = 100)
{
    for (int i = 0; i < maxAttempts; i++)
    {
        if (IL2CPP::Initialize(true))
        {
            return true;
        }
        Sleep(delayMs);
    }
    printf("[-] Failed to initialize Il2Cpp after %d attempts\n", maxAttempts);
    return false;
}

bool find_sigs()
{
    Unity::il2cppClass* optionsHelpers = IL2CPP::Class::Find("OptionsHelpers");
    if (!optionsHelpers)
    {
        printf("[-] Class 'OptionsHelpers' not found\n");
        return false;
    }

    Offsets::UnlockedRop = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(optionsHelpers, "IsRobeUnlocked");
    Offsets::UnlockedCharacters = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(optionsHelpers, "IsCharacterUnlocked");

    return (Offsets::UnlockedRop && Offsets::UnlockedCharacters);
}

void EnableHooks()
{
    if (Offsets::UnlockedRop)
    {
        if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::UnlockedRop),
            &Functions::UnlockedRop_h, (LPVOID*)&Functions::UnlockedRop_org) == MH_OK)
        {
            MH_EnableHook(reinterpret_cast<LPVOID*>(Offsets::UnlockedRop));
        }
        else
        {
            printf("[-] Failed to create hook for UnlockedRop\n");
        }
    }

    if (Offsets::UnlockedCharacters)
    {
        if (MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::UnlockedCharacters),
            &Functions::UnlockedCharacter_h, (LPVOID*)&Functions::UnlockedCharacter_org) == MH_OK)
        {
            MH_EnableHook(reinterpret_cast<LPVOID*>(Offsets::UnlockedCharacters));
        }
        else
        {
            printf("[-] Failed to create hook for UnlockedCharacter\n");
        }
    }
}


void MainThread()
{
    if (!DevourE())
    {
        return;
    }
    HWND hGameWnd = FindWindowA("UnityWndClass", NULL);
    if (!hGameWnd) {
        hGameWnd = GetForegroundWindow();
    }
    MessageBoxA(hGameWnd, "Robs and Characters are unlocked. Created by moon", "Success", MB_OK | MB_TOPMOST);
    if (MH_Initialize() != MH_OK)
    {
        printf("[-] MinHook initialization failed\n");
        return;
    }
    sdk::Base = (uintptr_t)GetModuleHandleA(NULL);
    sdk::GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
    sdk::UnityPlayer = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");
    if (!WaitForIl2Cpp())
    {
        printf("[-] Exiting due to Il2Cpp initialization failure\n");
        return;
    }
    if (!find_sigs())
    {
        printf("[-] Failed to find offsets, hooks will not be enabled\n");
        return;
    }
    EnableHooks();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, nullptr, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}