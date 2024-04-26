#pragma once
#include "framework.h"
#include "SDK/Engine_parameters.hpp"
#include "Inventory.h"

namespace GameMode
{
	bool (*ReadyToStartMatch)(AGameMode* GameMode);
	bool ReadyToStartMatchHook(AGameMode* GameMode)
	{
		auto GameModeAthena = Cast<AFortGameModeAthena>(GameMode);
		auto GameStateAthena = Cast<AFortGameStateAthena>(GameMode->GameState);
		//auto PlayerState = Cast<AFortPlayerStateAthena>(PlayerController->)

		if (!GameModeAthena || !GameStateAthena)
			return false;

		static bool bHasSetupPlaylist = false;
		if (!bHasSetupPlaylist)
		{
			bHasSetupPlaylist = true;

		//	GameStateAthena->bGameModeWillSkipAircraft = true;
		//	GameStateAthena->AircraftStartTime = 99999.0f;
		//	GameStateAthena->WarmupCountdownEndTime = 99999.0f;

			GameStateAthena->GamePhase = EAthenaGamePhase::Warmup;
			GameStateAthena->OnRep_GamePhase(EAthenaGamePhase::Setup);

		   auto Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DefaultSolo.Playlist_DefaultSolo");

			GameStateAthena->CurrentPlaylistId = Playlist->PlaylistId;
			GameStateAthena->OnRep_CurrentPlaylistId();
			GameStateAthena->CurrentPlaylistInfo.BasePlaylist = Playlist;
			GameStateAthena->CurrentPlaylistInfo.PlaylistReplicationKey++;
			GameStateAthena->CurrentPlaylistInfo.OverridePlaylist = Playlist;
			GameStateAthena->OnRep_CurrentPlaylistInfo();
			//GameStateAthena->CurrentPlaylistInfo.MarkArrayDirty.(); //no markarraydirt implementation rn


			auto LF_Athena_StreamingTest13 = StaticFindObject<ABuildingFoundation>("/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Athena_StreamingTest13");
		  	if (LF_Athena_StreamingTest13)
			{
				LOG_INFO("[+] Succsesfully Loaded: LF_Athena_StreamingTest13");

				LF_Athena_StreamingTest13->DynamicFoundationType = EDynamicFoundationType::Static;
				LF_Athena_StreamingTest13->bServerStreamedInLevel = true;
				LF_Athena_StreamingTest13->OnRep_ServerStreamedInLevel();
			}

		   /* auto Athena_POI_LakeIsland = StaticFindObject<ABuildingFoundation>("/Game/Athena/Maps/POI/Athena_POI_LakeIsland");
			if (Athena_POI_LakeIsland)
			{
				LOG_INFO("[+] Succsesfully Loaded: Athena_POI_LakeIsland");

				Athena_POI_LakeIsland->DynamicFoundationType = EDynamicFoundationType::Static;
				Athena_POI_LakeIsland->bServerStreamedInLevel = true;
				Athena_POI_LakeIsland->OnRep_ServerStreamedInLevel();
			}

			auto Lake1= StaticFindObject<ABuildingFoundation>("/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Lake1");
			if (Lake1)
			{
				LOG_INFO("[+] Succsesfully Loaded: LF_Lake1");

				Lake1->DynamicFoundationType = EDynamicFoundationType::Static;
				Lake1->bServerStreamedInLevel = true;
				Lake1->OnRep_ServerStreamedInLevel();
			}

			auto Lake2 = StaticFindObject<ABuildingFoundation>("/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Lake2");
			if (Lake2)
			{
				LOG_INFO("[+] Succsesfully Loaded: LF_Lake2");

				Lake2->DynamicFoundationType = EDynamicFoundationType::Static;
				Lake2->bServerStreamedInLevel = true;
				Lake2->OnRep_ServerStreamedInLevel();
			}

			auto LF_FloatingIsland = StaticFindObject<ABuildingFoundation>("/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_FloatingIsland");
			if (LF_FloatingIsland)
			{
				LOG_INFO("[+] Succsesfully Loaded: LF_FloatingIsland!");

				LF_FloatingIsland->DynamicFoundationType = EDynamicFoundationType::Static;
				LF_FloatingIsland->bServerStreamedInLevel = true;
				LF_FloatingIsland->OnRep_ServerStreamedInLevel();
			}*/


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

				GameModeAthena->bWorldIsReady = true;

				GameStateAthena->OnRep_CurrentPlaylistInfo();
				GameModeAthena->WarmupRequiredPlayerCount = 1;

				GameModeAthena->bWorldIsReady = true;

			   	LOG_INFO("Players Can Join!");
				
			}

			return ReadyToStartMatch(GameMode);
		}
	}


		
			void Init()
			{
				Hook(__int64(GetModuleHandleW(0)) + 0x25D9500, ReadyToStartMatchHook, (void**)&ReadyToStartMatch);
			//7FF6E0558A46-7FF6E0310000=0x248A46
				//7FF6E39F2250-7FF6E0310000=0x36E2250
		}
	}

