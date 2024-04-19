#pragma once
#include "ue.h"
#include "SDK.hpp"

using namespace SDK;

void (*ServerLoadingScreenDroppedHook)(UObject* Context, void* Ret);
void ServerLoadingScreenDropped(UObject* Context, void* Ret)
{
	auto PlayerController = (AFortPlayerController*)Context;

	auto PlayerState = (AFortPlayerState*)PlayerController->PlayerState;
	auto Pawn = (AFortPlayerPawn*)PlayerController->Pawn;

	PlayerController->ForceNetUpdate();
	PlayerState->ForceNetUpdate();
	Pawn->ForceNetUpdate();

	return ServerLoadingScreenDropped(Context, Ret);
}

void ServerAcknowlegePossessionHook(APlayerController* Controller, APawn* Pawn)
{
	Controller->AcknowledgedPawn = Pawn;
}