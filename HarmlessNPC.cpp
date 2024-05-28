#include "stdafx.h"
#include "HarmlessNPC.h"

HarmlessNPC::HarmlessNPC() : Statistics(), NamedNPC(), inventory() { }

HarmlessNPC::HarmlessNPC(const Statistics& S, const NamedNPC& C, unique_ptr<Inventory> pInventory) : Statistics(S), NamedNPC(C), inventory(move(pInventory)) { }

HarmlessNPC::HarmlessNPC(const HarmlessNPC& C) : Statistics(C), NamedNPC(C)
{
	if (C.inventory)
		inventory = make_unique<Inventory>(*C.inventory);
	else
		inventory = nullptr;
}

HarmlessNPC& HarmlessNPC::operator=(const HarmlessNPC& C)
{
	if (&C == this) return *this;
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

const unique_ptr<Inventory>& HarmlessNPC::getInventory() const { return inventory; }
