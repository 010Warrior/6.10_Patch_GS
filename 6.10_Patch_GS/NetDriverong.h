#pragma once
#include "ue.h"

namespace NetDriver
{
	void (*TickFlush)(UNetDriver* NetDriver);
	void TickFlushHook(UNetDriver* NetDriver)
	{
		if (NetDriver && NetDriver->ClientConnections.Num() > 0)
		{
			if (NetDriver->ReplicationDriver)
				ServerReplicateActors(NetDriver->ReplicationDriver);
		}

		return TickFlush(NetDriver);
	}


	void Init()
	{
		LOG_INFO("TickFlush Called!");
		Hook(__int64(GetModuleHandleW(0)) + 0x2709580, TickFlushHook, (void**)&TickFlush); 
	}
}