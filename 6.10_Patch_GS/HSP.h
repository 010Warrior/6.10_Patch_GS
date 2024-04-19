#pragma once
#include "ue.h"

/*namespace HandeNewPlayer
{
	static void (*HandleStartingNewPlayerA)(AFortGameModeAthena*, AFortPlayerControllerAthena*); //offset wrong + its skunk jyzo said 
	void HandleStartingNewPlayerHook(AFortGameModeAthena* GM, AFortPlayerControllerAthena* NewPlayer)
	{
		LOG_INFO("HandleStartingNewPlayerHook")
		auto FortPlayerState = (AFortPlayerStateAthena*)NewPlayer->PlayerState;

		NewPlayer->bHasServerFinishedLoading = true;
		NewPlayer->OnRep_bHasServerFinishedLoading();


		FortPlayerState->bHasStartedPlaying = true;
		FortPlayerState->OnRep_bHasStartedPlaying();

		NewPlayer->OverriddenBackpackSize = 5;

		return HandleStartingNewPlayerA(GM, NewPlayer);

	}

	void init()
	{
		Hook(__int64(GetModuleHandleW(0)) + 0x2A466C0, HandeNewPlayer::HandleStartingNewPlayerHook, (void**)&HandeNewPlayer::HandleStartingNewPlayerA); //wrong offset thats why it aint working lol
	}
}*/