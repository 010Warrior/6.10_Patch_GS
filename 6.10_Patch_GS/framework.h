#pragma once
#include <windows.h>
#include <cstdio>
#include <format>
#include <iostream>
#include "ue.h"
#include "MH/MinHook.h"
#include "LOGINFO.h"
#include "SDK.hpp"

#pragma comment(lib, "MH/minhook.lib")

using namespace SDK;

inline SDK::UNetDriver* (*CreateNetDriver)(void*, void*, SDK::FName);
//static UNetDriver* (*CreateNetDriver)(UEngine* a1, UWorld* a2, FName a3) = decltype(CreateNetDriver)(__int64(GetModuleHandleW(0)) + 0x455C570); 
static void (*SetWorld)(UNetDriver* a1, UWorld* a2) = decltype(SetWorld)(__int64(GetModuleHandleW(0)) + 0x27080E0);
static bool (*InitListen)(UNetDriver* a1, UWorld* a2, FURL a3, bool a4, FString& a5) = decltype(InitListen)(__int64(GetModuleHandleW(0)) + 0x40C9D0);
static bool (*InitHost)(void*);
static int32(*ServerReplicateActors)(void* a1);
static SDK::FGameplayAbilitySpecHandle* (*GiveAbilityOG)(SDK::UAbilitySystemComponent*, SDK::FGameplayAbilitySpecHandle*, SDK::FGameplayAbilitySpec);
//static void (*SpecsTestOG)(void* OutSpecIdk, void* Ability, int Level, int InputID, void* a5);



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

static SDK::UObject* (*o_StaticFindObject)(SDK::UClass* Class, SDK::UObject* Package, const TCHAR* Name, bool ExactClass) = decltype(o_StaticFindObject)(GetOffsetSDK(0x13DE630)); //should be correct fr
template<typename T>
T* StaticFindObject(std::string Name, SDK::UClass* ObjectClass = SDK::UObject::StaticClass())
{
	auto OrigInName = std::wstring(Name.begin(), Name.end()).c_str();
	return (T*)o_StaticFindObject(ObjectClass, nullptr, OrigInName, false);
}

/*static SDK::UObject* (*o_StaticLoadObject)(SDK::UClass* Class, SDK::UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, uint32_t LoadFlags, SDK::UObject* Sandbox, bool bAllowObjectReconciliation, void*) = decltype(o_StaticLoadObject)(GetOffsetSDK(0x190E555)); //100% wrong
template<typename T>
T* StaticLoadObject(std::string name)
{
	T* Object = StaticFindObject<T>(name);

	if (!Object)
	{
		auto Name = std::wstring(name.begin(), name.end()).c_str();
		Object = (T*)o_StaticLoadObject(T::StaticClass(), nullptr, Name, nullptr, 0, nullptr, false, nullptr);
	}

	return Object;
}*/

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

