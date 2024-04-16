#pragma once
#include <iostream>
#include <Windows.h>
#include <functional>
#include <vector>
#include <format>
#include "framework.h"
#include "Memcury.h"
#include "NetDriverong.h"
#include "Game.h"
#include "LOGINFO.h"
#include "Actor.h"

namespace Misc
{

    char KickPlayer()
    {
        LOG_INFO("[+] KickPlayer Called!");
        return 1;
    }

    void Init()
    {
        __int64 Base = __int64(GetModuleHandleW(0));
        Hook(Base + 0x25C98B0, KickPlayer);
    }
}