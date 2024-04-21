#include "stdafx.h"
#include "HarmlessNPC.h"

HarmlessNPC::HarmlessNPC() : Statistics(), NamedNPC() { }

HarmlessNPC::HarmlessNPC(const Statistics& S, const NamedNPC& C) : Statistics(S), NamedNPC(C) { }

HarmlessNPC::HarmlessNPC(const HarmlessNPC& C) : Statistics(C), NamedNPC(C) { }

HarmlessNPC& HarmlessNPC::operator=(const HarmlessNPC& C)
{
	if (&C == this) return *this;
	Statistics::operator=(C);
	NamedNPC::operator=(C);
	return *this;
}

HarmlessNPC::~HarmlessNPC() { }
