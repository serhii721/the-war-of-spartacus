#include "stdafx.h"
#include "NPC.h"

NPC::NPC() : Fighter(), NamedNPC(), Leveling(), portraitIndex((rand() % 3) + 1) { } // TODO: change generation of portrait index to support more portraits

NPC::NPC(const Fighter& F, const NamedNPC& C, const Leveling& L, int pportraitIndex) : Fighter(F), NamedNPC(C), Leveling(L), portraitIndex(pportraitIndex) { }

NPC::NPC(const NPC& C) : Fighter(C), NamedNPC(C), Leveling(C), portraitIndex(C.portraitIndex) { }

NPC& NPC::operator=(const NPC& C)
{
	if (&C == this) return *this;

	Fighter::operator=(C);
	NamedNPC::operator=(C);
	Leveling::operator=(C);
	portraitIndex = C.portraitIndex;

	return *this;
}

NPC::~NPC() { }

unique_ptr<NPC> NPC::clone() const
{
	return make_unique<NPC>(*this);
}

void NPC::setPortraitIndex(int i) { portraitIndex = i; }

const int NPC::getPortraitIndex() const { return portraitIndex; }
