#pragma once
#include "framework.h"

namespace GameMode
{
	bool (*ReadyToStartMatch)(AGameMode* GameMode);
	bool ReadyToStartMatchHook(AGameMode* GameMode)
	{
		auto GameModeAthena = Cast<AFortGameModeAthena>(GameMode);
		auto GameStateAthena = Cast<AFortGameStateAthena>(GameMode->GameState);

		if (!GameModeAthena || !GameStateAthena)
			return false;

		static bool BSetup = false;
		if (!BSetup)
		{
			BSetup = true;
			auto Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DefaultSolo.Playlist_DefaultSolo");

			if (!Playlist)
			{
				LOG_INFO("Playlist Wrong Retard!");
			}
		   /*GameStateAthena->GamePhase = EAthenaGamePhase::Warmup;
		   GameStateAthena->OnRep_GamePhase(EAthenaGamePhase::Setup);*/

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

				GameStateAthena->CurrentPlaylistId = Playlist->PlaylistId;
				GameStateAthena->CurrentPlaylistInfo.BasePlaylist = Playlist;
				GameStateAthena->CurrentPlaylistInfo.PlaylistReplicationKey++;
				//	GameStateAthena->CurrentPlaylistInfo.MarkArrayDirt; // i need to add MarkArrayDirt WOwie!
				GameStateAthena->CurrentPlaylistInfo.OverridePlaylist = Playlist;
				GameStateAthena->CurrentPlaylistInfo.MarkArrayDirty();

				GameStateAthena->OnRep_CurrentPlaylistInfo();
				GameModeAthena->WarmupRequiredPlayerCount = 1;

				GameModeAthena->bWorldIsReady = true;
			}

			return ReadyToStartMatch(GameMode);
		}
	}

	bool (*SpawnDefaultPawnFor)(UObject* Object, void* Params);
	bool SpawnDefaultPawnForHook(UObject* Object, void* Params)
	{
		LOG_INFO("[+] SpawnDefaultPawnFor Called!");
		auto GameMode = (AFortGameModeAthena*)Object;
		AGameModeBase_SpawnDefaultPawnFor_Params* Parameters = (AGameModeBase_SpawnDefaultPawnFor_Params*)Params;
		AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Parameters->NewPlayer;

		if (!PC || !Parameters->StartSpot)
			return false;

		/*	if (!PC || !Parameters->NewPlayer)
				return true;*/
	}

	void Init()
	{
		Hook(__int64(GetModuleHandleW(0)) + 0x25D9500, ReadyToStartMatchHook, (void**)&ReadyToStartMatch);
		Hook(__int64(GetModuleHandleW(0)) + 0xC2FD50, SpawnDefaultPawnForHook, (void**)&SpawnDefaultPawnFor);
	}
}