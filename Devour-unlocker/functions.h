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

	void(UNITY_CALLING_CONVENTION  Menu_SetupPerk_org)(DWORD*, void*);
	void __stdcall Menu_SetupPerk_hk(DWORD* __this, void* perk)
	{
		if (perk)
		{
			IL2CPP::CClass* p = reinterpret_cast<IL2CPP::CClass*>(perk);
			p->SetMemberValue<int>("cost", 0);
			p->SetMemberValue<bool>("isOwned", true);
			p->SetMemberValue<bool>("isHidden", false);
		}
		return Menu_SetupPerk_org(__this, perk);
	}

	// Returns UIOutfitSelectionType* - capture it and force SetLocked(false)
	// so even if the game's own logic still thinks it's locked, the UI shows unlocked.
	void* (UNITY_CALLING_CONVENTION  Menu_SetupOutfit_org)(DWORD*, void*);
	void* __stdcall Menu_SetupOutfit_hk(DWORD* __this, void* outfit)
	{
		if (outfit)
		{
			IL2CPP::CClass* o = reinterpret_cast<IL2CPP::CClass*>(outfit);
			o->SetMemberValue<bool>("isOwned", true);
			o->SetMemberValue<bool>("isHidden", false);
		}
		void* result = Menu_SetupOutfit_org(__this, outfit);
		if (result)
			reinterpret_cast<IL2CPP::CClass*>(result)->CallMethodSafe<void*>("SetLocked", false);
		return result;
	}

	void* (UNITY_CALLING_CONVENTION  Menu_SetupEmote_org)(DWORD*, void*);
	void* __stdcall Menu_SetupEmote_hk(DWORD* __this, void* emote)
	{
		if (emote)
		{
			IL2CPP::CClass* e = reinterpret_cast<IL2CPP::CClass*>(emote);
			e->SetMemberValue<bool>("isOwned", true);
			e->SetMemberValue<bool>("isHidden", false);
			e->SetMemberValue<bool>("requiresPurchase", false);
			e->SetMemberValue<int>("cost", 0);
		}
		void* result = Menu_SetupEmote_org(__this, emote);
		if (result)
			reinterpret_cast<IL2CPP::CClass*>(result)->CallMethodSafe<void*>("SetLocked", false, 0);
		return result;
	}

	void* (UNITY_CALLING_CONVENTION  Menu_SetupPet_org)(DWORD*, void*);
	void* __stdcall Menu_SetupPet_hk(DWORD* __this, void* pet)
	{
		if (pet)
		{
			IL2CPP::CClass* p = reinterpret_cast<IL2CPP::CClass*>(pet);
			p->SetMemberValue<bool>("isOwned", true);
			p->SetMemberValue<bool>("isHidden", false);
		}
		void* result = Menu_SetupPet_org(__this, pet);
		if (result)
			reinterpret_cast<IL2CPP::CClass*>(result)->CallMethodSafe<void*>("SetLocked", false);
		return result;
	}

	void(UNITY_CALLING_CONVENTION  Menu_SetupFlashlight_org)(DWORD*, void*);
	void __stdcall Menu_SetupFlashlight_hk(DWORD* __this, void* fl)
	{
		if (fl)
		{
			IL2CPP::CClass* f = reinterpret_cast<IL2CPP::CClass*>(fl);
			f->SetMemberValue<bool>("isOwned", true);
			f->SetMemberValue<bool>("isHidden", false);
			f->SetMemberValue<bool>("requiresPurchase", false);
			f->SetMemberValue<int>("cost", 0);
		}
		return Menu_SetupFlashlight_org(__this, fl);
	}

}
