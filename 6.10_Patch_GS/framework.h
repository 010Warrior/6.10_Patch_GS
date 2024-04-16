#pragma once
#include <windows.h>
#include <cstdio>
#include <format>
#include <iostream>
#include "ue.h"
#include "MH/MinHook.h"
#include "SDK.hpp"
#include "LOGINFO.h"

#pragma comment(lib, "MH/minhook.lib")

using namespace SDK;
using namespace Params;

inline SDK::UNetDriver* (*CreateNetDriver)(void*, void*, SDK::FName);
//static UNetDriver* (*CreateNetDriver)(UEngine* a1, UWorld* a2, FName a3) = decltype(CreateNetDriver)(__int64(GetModuleHandleW(0)) + 0x455C570); 
static void (*SetWorld)(UNetDriver* a1, UWorld* a2) = decltype(SetWorld)(__int64(GetModuleHandleW(0)) + 0x27080E0);
static bool (*InitListen)(UNetDriver* a1, UWorld* a2, FURL a3, bool a4, FString& a5) = decltype(InitListen)(__int64(GetModuleHandleW(0)) + 0x40C9D0);
static bool (*InitHost)(void*);
static int32(*ServerReplicateActors)(void* a1);



__int64 GetOffsetSDK(__int64  Offset)
{
	return __int64(GetModuleHandleW(0)) + Offset;
}


void Hook(__int64 Address, void* Detour, void** Original = nullptr)
{
	if (Address)
	{
		MH_CreateHook((LPVOID)Address, Detour, Original);
		MH_EnableHook((LPVOID)Address);
	}
}

void Hook(UObject* Object, int Index, void* Detour, void** Original = nullptr)
{
	if (!Object)
		return;

	void** VTable = *(void***)Object;
	if (VTable && VTable[Index])
	{
		if (Original)
			*Original = VTable[Index];

		DWORD dwOldProtection;
		VirtualProtect(&VTable[Index], 8, PAGE_EXECUTE_READWRITE, &dwOldProtection);

		VTable[Index] = Detour;

		DWORD dwTempProtection;
		VirtualProtect(&VTable[Index], 8, dwOldProtection, &dwTempProtection);
	}
}

static SDK::UObject* (*o_StaticFindObject)(SDK::UClass* Class, SDK::UObject* Package, const TCHAR* Name, bool ExactClass) = decltype(o_StaticFindObject)(GetOffsetSDK(0x13DE630));
template<typename T>
T* StaticFindObject(std::string Name, SDK::UClass* ObjectClass = SDK::UObject::StaticClass())
{
	auto OrigInName = std::wstring(Name.begin(), Name.end()).c_str();
	return (T*)o_StaticFindObject(ObjectClass, nullptr, OrigInName, false);
}

static inline std::vector<std::pair<SDK::UFunction*, std::function<bool(SDK::UObject*, void*)>>> FunctionsToHook;
static bool AddHook(SDK::UFunction* Func, std::function<bool(SDK::UObject*, void*)> callback)
{
	if (!Func)
		return false;

	FunctionsToHook.push_back({ Func, callback });
   	LOG_INFO("Hooked Function: {}", Func->GetName());

	return true;
}

static bool AddHook(const std::string& Func, std::function<bool(SDK::UObject*, void*)> callback)
{
	SDK::UFunction* Function = StaticFindObject<SDK::UFunction>(Func);
	if (!Function)
		Function = SDK::UObject::FindObject<SDK::UFunction>(Func);
	return AddHook(Function, callback);
}

template<typename T>
T* Cast(UObject* Object, bool bForceCheck = true)
{
	if (Object)
	{
		if (bForceCheck)
		{
			if (Object->IsA(T::StaticClass()))
			{
				return (T*)Object;
			}
		}
		else
		{
			return (T*)Object;
		}
	}
	return nullptr;
}

template<typename T>
T* GetDefaultObject()
{
	return (T*)T::StaticClass()->DefaultObject;
}

UFortEngine* GetEngine()
{
	static auto Engine = UObject::FindObject<UFortEngine>("FortEngine_");
	return Engine;
}

UWorld* GetWorld()
{
	return GetEngine()->GameViewport->World;
}

AFortGameModeAthena* GetGameMode()
{
	return (AFortGameModeAthena*)GetWorld()->AuthorityGameMode;
}

AFortGameStateAthena* GetGameState()
{
	return (AFortGameStateAthena*)GetWorld()->GameState;
}

UGameplayStatics* GetStatics()
{
	return GetDefaultObject<UGameplayStatics>();
}

template<typename T>
T* SpawnActor(UClass* ActorClass = T::StaticClass(), FVector Location = {}, FRotator Rotation = {}, AActor* Owner = nullptr)
{
	struct FActorSpawnParameters
	{
		FName Name;
		AActor* Template;
		AActor* Owner;
		APawn* Instigator;
		ULevel* OverrideLevel;
		ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;
		uint8 bRemoteOwned : 1;
		uint8 bNoFail : 1;
		uint8 bDeferConstruction : 1;
		uint8 bAllowDuringConstructionScript : 1;
		int32 ObjectFlags;
	};

	FActorSpawnParameters Params{};
	Params.Owner = Owner;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	static void* (*SpawnActorOG)(void*, void*, void*, void*, void*) = decltype(SpawnActorOG)(__int64(GetModuleHandleW(0)) + 0x38065C8);
	return (T*)SpawnActorOG(GetWorld(), ActorClass, &Location, &Rotation, &Params);
}

void (*o_ProcessEvent)(void*, void*, void*);
void h_ProcessEvent(void* Object, void* Function, void* Params)
{
	for (auto& ToHook : FunctionsToHook)
	{
		if (ToHook.first == Function)
		{
			if (ToHook.second((SDK::UObject*)Object, Params))
				return;
			return o_ProcessEvent(Object, Function, Params);
		}
	}

	return o_ProcessEvent(Object, Function, Params);
}