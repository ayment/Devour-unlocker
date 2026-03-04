#pragma once
#include <vector>
#include <utility> 

namespace sdk
{
	uintptr_t Base;
	uintptr_t GameAssembly;
	uintptr_t UnityPlayer;
}

namespace Offsets
{
	uintptr_t UnlockedRop = 0x0;
	uintptr_t UnlockedCharacters = 0x0;
	uintptr_t Menu_SetupPerk = 0x0;
	uintptr_t Menu_SetupOutfit = 0x0;
	uintptr_t Menu_SetupFlashlight = 0x0;
	uintptr_t Menu_SetupEmote = 0x0;
	uintptr_t Menu_SetupPet = 0x0;
}
