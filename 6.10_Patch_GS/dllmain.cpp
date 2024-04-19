#pragma once
#include <Windows.h>
#include <functional>
#include <vector>
#include <format>
#include <iostream>
#include "ue.h"
#include "UnrealContainers.hpp"


DWORD Main(LPVOID)
{
    AllocConsole();
    FILE* streamPtr;
    freopen_s(&streamPtr, "CONOUT$", "w", stdout);

    SetConsoleTitleA("6_1-SDK");
    MH_Initialize();
    SetupLogger();

    LOG_INFO("Welcome to WarriorGS");
    LOG_INFO("[+] Currently In Beta On Version: 6.10 Unreal Engine: Ver:UE4.21 ");

    Sleep(5000);

    LOG_INFO("[+] Entering Athena_Terrain \n");
    GetDefaultObject<UKismetSystemLibrary>()->ExecuteConsoleCommand(GetWorld(), L"open Athena_Terrain", nullptr);
    GetEngine()->GameInstance->LocalPlayers.Remove(0);

    CreateNetDriver = decltype(CreateNetDriver)(Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC 30 48 8B 81 ? ? ? ? 49 8B D8 4C 63 81 ? ? ? ? 4C 8B D2 48 8B F9 4E 8D 0C C0 49 3B C1 74 1B").Get()); //ue421
    LOG_INFO("[+] Found NetDriver From: 48 89 5C 24 ? 57 48 83 EC 30 48 8B 81 ? ? ? ? 49 8B D8 4C 63 81 ? ? ? ? 4C 8B D2 48 8B F9 4E 8D 0C C0 49 3B C1 74 1B");

    // InitHost = decltype(InitHost)(Memcury::Scanner::FindPattern("48 8B C4 48 81 EC ? ? ? ? 48 89 58 18 4C 8D 05 ? ? ? ?").Get()); //ue421A

    LOG_INFO("[+] Hooks");
    Misc::Init();

   // Actor::Init();
    //LOG_INFO("[+] ActorGetNetMode: 0x239A4F0")


    LOG_INFO("[+] KickPlayer: 0x25C98B0");
    NetDriver::Init();

    LOG_INFO("[+] TickFlush: 0x2709580");

    GameMode::Init();
    GameModeBase::Init();
    HandeNewPlayer::init();

    LOG_INFO("[+] Finished hooks");

    



    return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        // Creates the main theread!
        CreateThread(0, 0, Main, 0, 0, 0);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
