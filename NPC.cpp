#include "stdafx.h"
#include "NPC.h"

NPC::NPC() : Fighter(), NamedNPC(), Leveling() { }

NPC::NPC(const Fighter& F, const NamedNPC& N, int l) : Fighter(F), NamedNPC(N), Leveling(l) { }

NPC::NPC(const NPC& N) : Fighter(N), NamedNPC(N), Leveling(N) { }

NPC& NPC::operator=(const NPC& N)
{
	strength = N.strength;
	constitution = N.constitution;
	dexterity = N.dexterity;
	intelligence = N.intelligence;
	wisdom = N.wisdom;
	charisma = N.charisma;
	age = N.age;
	fame = N.fame;

	hp = N.hp;
	fullHP = N.fullHP;

	if (rightHand)
		delete rightHand;
	rightHand = new Weapon(N.getRightHand());
	if (leftHand)
		delete leftHand;
	leftHand = new Weapon(N.getLeftHand());
	if (armour)
		delete armour;
	armour = new Armour(N.getArmour());

	firstNameIndex = N.firstNameIndex;
	lastNameIndex = N.lastNameIndex;

	level = N.level;

	return *this;
}

NPC::~NPC() { }
