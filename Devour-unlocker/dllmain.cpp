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
    MessageBoxA(NULL, "Failed to initialize Il2Cpp", "error", MB_OK | MB_TOPMOST);
    return false;
}

bool find_sigs()
{
    Unity::il2cppClass* optionsHelpers = IL2CPP::Class::Find("OptionsHelpers");
    Unity::il2cppClass* CSMenu = IL2CPP::Class::Find("Horror.Menu");
    if (!optionsHelpers)
    {
        MessageBoxA(NULL, "[-] Class 'OptionsHelpers' not found", "error", MB_OK | MB_TOPMOST);
        return false;
    }
    if (!CSMenu)
    {
        MessageBoxA(NULL, "[-] Class 'Horror.Menu' not found", "error", MB_OK | MB_TOPMOST);
        return false;
    }
    Offsets::UnlockedRop = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(optionsHelpers, "IsRobeUnlocked");
    Offsets::UnlockedCharacters = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(optionsHelpers, "IsCharacterUnlocked");
    Offsets::Menu_SetupPerk = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(CSMenu, "SetupPerk");
    Offsets::Menu_SetupOutfit = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(CSMenu, "SetupOutfit");
    Offsets::Menu_SetupFlashlight = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(CSMenu, "SetupFlashlight");
    Offsets::Menu_SetupEmote = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(CSMenu, "SetupEmote");
    Offsets::Menu_SetupPet = (uintptr_t)IL2CPP::Class::Utils::GetMethodPointer(CSMenu, "SetupPet");
}

void EnableHooks()
{
    auto TryHook = [](uintptr_t target, LPVOID detour, LPVOID* orig, const char* name) {
        if (!target) {
            MessageBoxA(NULL, "[ WARN ] Skipping %s (not resolved)", "error", MB_OK | MB_TOPMOST);; return; }
        if (MH_CreateHook(reinterpret_cast<LPVOID>(target), detour, orig) == MH_OK)
            MH_EnableHook(reinterpret_cast<LPVOID>(target));
        else
            MessageBoxA(NULL, "Hook failed: %s", "error", MB_OK | MB_TOPMOST);
        };
    TryHook(Offsets::UnlockedRop, &Functions::UnlockedRop_h, (LPVOID*)&Functions::UnlockedRop_org, "UnlockedRop");
    TryHook(Offsets::UnlockedCharacters, &Functions::UnlockedCharacter_h, (LPVOID*)&Functions::UnlockedCharacter_org, "UnlockedCharacter");
    TryHook(Offsets::Menu_SetupPerk,&Functions::Menu_SetupPerk_hk, (LPVOID*)&Functions::Menu_SetupPerk_org, "SetupPerk");
    TryHook(Offsets::Menu_SetupOutfit,&Functions::Menu_SetupOutfit_hk, (LPVOID*)&Functions::Menu_SetupOutfit_org, "SetupOutfit");
    TryHook(Offsets::Menu_SetupFlashlight,&Functions::Menu_SetupFlashlight_hk, (LPVOID*)&Functions::Menu_SetupFlashlight_org, "SetupFlashlight");
    TryHook(Offsets::Menu_SetupEmote,&Functions::Menu_SetupEmote_hk, (LPVOID*)&Functions::Menu_SetupEmote_org, "SetupEmote");
    TryHook(Offsets::Menu_SetupPet,&Functions::Menu_SetupPet_hk, (LPVOID*)&Functions::Menu_SetupPet_org, "SetupPet");
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
    MessageBoxA(hGameWnd, "All unlocked Enjoy. Created by moon", "Success", MB_OK | MB_TOPMOST);
    if (MH_Initialize() != MH_OK)
    {
        MessageBoxA(hGameWnd, "MinHook initialization failed", "error", MB_OK | MB_TOPMOST);

        return;
    }
    sdk::Base = (uintptr_t)GetModuleHandleA(NULL);
    sdk::GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
    sdk::UnityPlayer = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");
    if (!WaitForIl2Cpp())
    {
        MessageBoxA(hGameWnd, "Exiting due to Il2Cpp initialization failure", "error", MB_OK | MB_TOPMOST);
        return;
    }
    if (!find_sigs())
    {
        MessageBoxA(hGameWnd, "Failed to find offsets, hooks will not be enabled", "error", MB_OK | MB_TOPMOST);
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
