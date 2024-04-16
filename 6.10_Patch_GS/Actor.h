#pragma once
#include "ue.h"

using namespace SDK;

namespace Actor
{
	__int64 GetNetModeHook(AActor* Actor)
	{
		return 1;
	}

	void Init()
	{
		LOG_INFO("[+] ActorGetNetMode: Caled Wowie")
		Hook(__int64(GetModuleHandleW(0)) + 0x239A4F0, GetNetModeHook);
	}
}