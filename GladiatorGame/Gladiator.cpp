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

bool Gladiator::isAlive()
{
	return health > 0;
}

void Gladiator::attack(Gladiator& rOpponent, AttackResult& rResult, int& rDamage)
{
	// TODO
	// dodge = 
	// damage = (weapon_damage + strength) / 10 * (1 - ) * (1 - dodge) * (1 - 0.7) * (1 + 0.3) * (1 - )
	// 
}

Gladiator::~Gladiator()
{
	if (weapon)
		delete weapon;
}
