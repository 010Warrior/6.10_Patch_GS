#include "ue.h"

namespace GameModeBase
{
	APawn* SpawnDefaultPawnForHook(AGameModeBase* GameModeBase, AController* Controller, AActor* StartSpot, AFortPlayerControllerAthena* NewPlayer) //from volcano i will make my own one soon this was just to make sure the offset was okay!
	{
		if (!StartSpot || !Controller)
			return 0;

		LOG_INFO("[+] SpawnDefualtPawnFor called");

		FTransform SpawnTransform = StartSpot->GetTransform();
		return GameModeBase->SpawnDefaultPawnAtTransform(Controller, SpawnTransform);

		auto PlayerState = (AFortPlayerStateAthena*)NewPlayer->PlayerState;

		auto FortPawn = UObject::FindObject("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");

		static auto Pickaxe = StaticFindObject<UAthenaPickaxeItemDefinition>("/Game/Athena/Items/Cosmetics/Pickaxes/DefaultPickaxe.DefaultPickaxe");

		static auto HeadPart = UObject::FindObject<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
		static auto BodyPart = UObject::FindObject<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");

		LOG_INFO("HUHHHHHHHH???????");
		Update((AFortPlayerController*)NewPlayer);
		LOG_INFO("Workeddddddddddddd???????");
	}

	void Init()
	{

		Hook(__int64(GetModuleHandleW(0)) + 0xC2FD50, SpawnDefaultPawnForHook);
	}
}