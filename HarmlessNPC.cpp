#include "stdafx.h"
#include "HarmlessNPC.h"

HarmlessNPC::HarmlessNPC() : level(MIN_TRADER_LEVEL), Statistics(), NamedNPC(), inventory() { }

HarmlessNPC::HarmlessNPC(int llevel, const Statistics& S, const NamedNPC& C, unique_ptr<Inventory> pInventory) : level(llevel), Statistics(S), NamedNPC(C), inventory(move(pInventory)) { }

HarmlessNPC::HarmlessNPC(const HarmlessNPC& C) : level(C.level), Statistics(C), NamedNPC(C)
{
	if (C.inventory)
		inventory = make_unique<Inventory>(*C.inventory);
	else
		inventory = nullptr;
}

HarmlessNPC& HarmlessNPC::operator=(const HarmlessNPC& C)
{
	if (&C == this) return *this;
	level = C.level;
	Statistics::operator=(C);
	NamedNPC::operator=(C);

	if (C.inventory)
	{
		if (!inventory)
			inventory = make_unique<Inventory>(*C.inventory);
		else
			*inventory = *C.inventory;
	}
	else
		inventory = nullptr;

	return *this;
}

HarmlessNPC::~HarmlessNPC() { }

int HarmlessNPC::getLevel() const { return level; }

const unique_ptr<Inventory>& HarmlessNPC::getInventory() const { return inventory; }
