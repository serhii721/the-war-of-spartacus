#include "stdafx.h"
#include "HarmlessNPC.h"

HarmlessNPC::HarmlessNPC() : Statistics(), NamedNPC() { }

HarmlessNPC::HarmlessNPC(const Statistics& S, const NamedNPC& N) : Statistics(S), NamedNPC(N) { }

HarmlessNPC::HarmlessNPC(const HarmlessNPC& N) : Statistics(N), NamedNPC(N) { }

HarmlessNPC& HarmlessNPC::operator=(const HarmlessNPC& N)
{
	strength = N.strength;
	constitution = N.constitution;
	dexterity = N.dexterity;
	intelligence = N.intelligence;
	wisdom = N.wisdom;
	charisma = N.charisma;
	age = N.age;
	fame = N.fame;

	firstNameIndex = N.firstNameIndex;
	lastNameIndex = N.lastNameIndex;

	return *this;
}

HarmlessNPC::~HarmlessNPC() { }
