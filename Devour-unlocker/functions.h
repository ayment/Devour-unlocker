#pragma once
#include <string>
#include "Il2cpp_Resolver/il2cpp_resolver.hpp"
#include "Lists.hpp"
#include <intrin.h>

namespace Functions
{
#pragma region Hooks

	bool(UNITY_CALLING_CONVENTION  UnlockedRop_org)(DWORD*, Unity::System_String);
	bool __stdcall UnlockedRop_h(DWORD* __this, Unity::System_String robe)
	{
		return true;
	}
	bool(UNITY_CALLING_CONVENTION  UnlockedCharacter_org)(DWORD*, Unity::System_String);
	bool __stdcall UnlockedCharacter_h(DWORD* __this, Unity::System_String prefab)
	{
		return true;
	}

}