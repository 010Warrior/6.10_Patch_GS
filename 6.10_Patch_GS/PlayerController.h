#pragma once
#include "ue.h"
#include "SDK.hpp"

using namespace SDK;


namespace Possesion
{
	void ServerAcknowlegePossessionHook(APlayerController* Controller, APawn* Pawn, AGameMode* GameMode)
	{
		LOG_INFO("[+] ServerAcknowlegePossession: Called");
		Controller->AcknowledgedPawn = Pawn;

		auto PlayerState = (AFortPlayerStateAthena*)Controller->PlayerState;
	}

	void (*ApplyCharacterCustomization)(FFortAthenaLoadout& Loadout, AFortPlayerState* PlayerState, AFortPlayerPawn* MyFortPawn);
	void ApplyCharacterCustomizationHook(FFortAthenaLoadout& Loadout, AFortPlayerState* PlayerState, AFortPlayerPawn* MyFortPawn)
	{
		LOG_INFO("[+] ApplyCharacterCustomizationHook: Called")
		if (!PlayerState || !MyFortPawn)
			return;
		auto HeroDefinition = Loadout.Character->HeroDefinition;

		if (Loadout.Character && HeroDefinition)
		{
			LOG_INFO("[+] ApplyCharacterCustomization: Found HeroType, Applying!")
				PlayerState->HeroType = HeroDefinition;
			(PlayerState, MyFortPawn);
		}
		else
		{
			static auto BodyPart = StaticLoadObject<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");
			static auto HeadPart = StaticLoadObject<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
			MyFortPawn->ServerChoosePart(EFortCustomPartType::Head, HeadPart);
			MyFortPawn->ServerChoosePart(EFortCustomPartType::Body, BodyPart);
		}
	}



	void Init()
	{
		Hook(__int64(GetModuleHandleW(0)) + 0x2AE2330, ServerAcknowlegePossessionHook); //gets called so correct
		Hook(__int64(GetModuleHandleW(0)) + 0x1266E80, ApplyCharacterCustomizationHook, (void**)&ApplyCharacterCustomization);

	}
}
