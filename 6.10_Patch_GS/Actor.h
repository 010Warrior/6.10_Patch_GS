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
		Hook(__int64(GetModuleHandleW(0)) + 0xAE774B, GetNetModeHook); //prob wrong thats why it crashes :skull:
	}
}