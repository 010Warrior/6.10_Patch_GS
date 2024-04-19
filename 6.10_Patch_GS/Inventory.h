#pragma once
#include "ue.h"

static void Update(AFortPlayerController* PC)
{
	PC->HandleWorldInventoryLocalUpdate();
	PC->WorldInventory->HandleInventoryLocalUpdate();
	PC->ForceNetUpdate();
	PC->WorldInventory->ForceNetUpdate();
	PC->WorldInventory->Inventory;

}