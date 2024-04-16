#pragma once
#include "framework.h"
using namespace SDK;
using namespace Params;

namespace GameMode
{
	bool (*ReadyToStartMatch)(AGameMode* GameMode);
	bool ReadyToStartMatchHook(AGameMode* GameMode)
	{
		auto GameModeAthena = Cast<AFortGameModeAthena>(GameMode);
		auto GameStateAthena = Cast<AFortGameStateAthena>(GameMode->GameState);

		if (!GameModeAthena || !GameStateAthena)
			return false;

		static bool bHasSetupPlaylist = false;
		if (!bHasSetupPlaylist)
		{
			bHasSetupPlaylist = true;
			auto Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DefaultSolo.Playlist_DefaultSolo");
			if (!Playlist)
			{
				bHasSetupPlaylist = false;
				LOG_INFO("[-] AGameMode::ReadyToStartMatch: Invalid Playlist");

				return false;
			}

			static bool bHasStartedListening = false;
			if (!bHasStartedListening)
			{
				bHasStartedListening = true;

				GetWorld()->NetDriver = CreateNetDriver(GetEngine(), GetWorld(), FName(282));

				GetWorld()->NetDriver->World = GetWorld();
				GetWorld()->NetDriver->NetDriverName = FName(282);

				FString Error;
				FURL Url = FURL();
				Url.Port = 7777;

				if (!InitListen(GetWorld()->NetDriver, GetWorld(), Url, false, Error))
				{
					LOG_INFO("[-] UIpNetDriver::InitListen failed");
				}

				SetWorld(GetWorld()->NetDriver, GetWorld());
				for (int i = 0; i < GetWorld()->LevelCollections.Num(); ++i)
				{
					GetWorld()->LevelCollections[i].NetDriver = GetWorld()->NetDriver;
				}
				ServerReplicateActors = decltype(ServerReplicateActors)((*(void***)GetWorld()->NetDriver->ReplicationDriver)[0x56]);
				LOG_INFO("[+] ServerReplicateActors: 0x{:x}", __int64(ServerReplicateActors) - __int64(GetModuleHandleW(0)));

				GameStateAthena->OnRep_CurrentPlaylistInfo();
				GameModeAthena->WarmupRequiredPlayerCount = 1;

				GameModeAthena->bWorldIsReady = true;
			}

			return ReadyToStartMatch(GameMode);
		}
	}

	namespace SpawnDefaultPawnFor
	{
		bool SpawnDefaultPawnForHook(UObject* Object, void* Params)
		{
			LOG_INFO("[+] SpawnDefaultPawnFor Called!");
			auto GameMode = (AFortGameModeAthena*)Object;
			AGameModeBase_SpawnDefaultPawnFor_Params* Parameters = (AGameModeBase_SpawnDefaultPawnFor_Params*)Params;
			AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Parameters->NewPlayer;

			if (!PC || !Parameters->StartSpot)
				return false;
			LOG_INFO("StartSpot");

			/*	if (!PC || !Parameters->NewPlayer)
					return true;*/
		}


	void Init()
	{
		__int64 Base = __int64(GetModuleHandleW(0));
		Hook(Base + 0x4435D68, ReadyToStartMatchHook, (void**)&ReadyToStartMatch);

		Sleep(5000);

		AddHook("Engine.GameModeBase.SpawnDefaultPawnFor", SpawnDefaultPawnForHook);
	}

}