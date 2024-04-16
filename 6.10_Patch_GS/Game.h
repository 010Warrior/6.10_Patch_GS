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

			//	GameStateAthena->CurrentPlaylistId = Playlist->PlaylistId;
				//GameStateAthena->OnRep_CurrentPlaylistId();
				//GameStateAthena->CurrentPlaylistInfo.BasePlaylist = Playlist;
			//	GameStateAthena->OnRep_CurrentPlaylistInfo();
				//GameStateAthena->CurrentPlaylistData = Playlist;
			   //	GameStateAthena->OnRep_CurrentPlaylistData();
		 		GameStateAthena->CurrentPlaylistInfo.PlaylistReplicationKey++;
		 		GameStateAthena->CurrentPlaylistInfo.OverridePlaylist = Playlist;
				GameStateAthena->CurrentPlaylistInfo.MarkArrayDirty();

				GameModeAthena->bWorldIsReady = true;

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

	bool HandleStartingNewPlayerHook(UObject* Object, void* Parms)
	{
		LOG_INFO("HandleStartingNewPlayerHook!");
		auto NewPlayer = (AFortPlayerControllerAthena*)((AGameModeBase_HandleStartingNewPlayer_Params*)Parms)->NewPlayer;

		if (!NewPlayer)
			return false;

		FTransform Transform{};
	 //   Transform.Translation = GetSpawn()->K2_GetActorLocation();
		Transform.Scale3D = FVector(1, 1, 1);

		LOG_INFO("Spawning Pawn!");
		APawn* NewPawn = (APawn*)SpawnActor<APlayerPawn_Athena_C>(APlayerPawn_Athena_C::StaticClass(), Transform.Translation);
		NewPlayer->Possess(NewPawn);

		AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)NewPlayer->PlayerState;

		PlayerState->bHasFinishedLoading = true;
		PlayerState->bHasStartedPlaying = true;
		NewPlayer->bHasClientFinishedLoading = true;
		NewPlayer->bHasServerFinishedLoading = true;

		return false;
	}


	void Init()
	{
		Hook(__int64(GetModuleHandleW(0)) + 0x25D9500, ReadyToStartMatchHook, (void**)&ReadyToStartMatch);
		Hook(__int64(GetModuleHandleW(0)) + 0xC2FD50, SpawnDefaultPawnForHook, (void**)&SpawnDefaultPawnFor);
		Sleep(10000);
		Hook(__int64(GetModuleHandleW(0)) + 0x239A4F0, HandleStartingNewPlayerHook);
	}
}