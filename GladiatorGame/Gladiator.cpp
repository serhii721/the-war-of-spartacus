#include "pch.h"
#include "Gladiator.h"

Gladiator::Gladiator() :
	name(""),
	age(MIN_AGE),
	health(BASIC_HEALTH),
	fame(BASIC_FAME),
	fatigue(BASIC_FATIGUE),
	strength(MIN_STRENGTH),
	constitution(MIN_CONSTITUTION),
	dexterity(MIN_DEXTERITY),
	intelligence(MIN_INTELLIGENCE),
	wisdom(MIN_WISDOM),
	charisma(MIN_CHARISMA),
	weapon(nullptr)
{ }

Gladiator::Gladiator(
	string nname,
	unsigned aage,
	int hhealth,
	int ffame,
	int ffatigue,
	int sstrength,
	int cconstitution,
	int ddexterity,
	int iintelligence,
	int wwisdom,
	int ccharisma,
	Weapon* wweapon
) :
	name(nname),
	age(aage),
	health(hhealth),
	fame(ffame),
	fatigue(ffatigue),
	strength(sstrength),
	constitution(cconstitution),
	dexterity(ddexterity),
	intelligence(iintelligence),
	wisdom(wwisdom),
	charisma(ccharisma),
	weapon(wweapon)
{ }

Gladiator::~Gladiator()
{
	if (weapon)
		delete weapon;
}
