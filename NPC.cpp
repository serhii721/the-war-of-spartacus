#include "stdafx.h"
#include "NPC.h"

NPC::NPC() : Fighter(), NamedNPC(), Leveling() { }

NPC::NPC(const Fighter& F, const NamedNPC& C, int l) : Fighter(F), NamedNPC(C), Leveling(l) { }

NPC::NPC(const NPC& C) : Fighter(C), NamedNPC(C), Leveling(C) { }

NPC& NPC::operator=(const NPC& C)
{
	if (this == &C) return *this;
	Fighter::operator=(C);
	NamedNPC::operator=(C);
	Leveling::operator=(C);
	return *this;
}

NPC::~NPC() { }
