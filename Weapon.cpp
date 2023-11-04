#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon() :
	damage(MIN_WEAPON_DAMAGE),
	type(BASIC_WEAPON_TYPE),
	damageAddition(0),
	strAdditionPerc(0),
	dexAdditionPerc(0),
	shieldProbAddition(0),
	shieldDefPercentAddition(0),
	name("")
{ }

Weapon::Weapon(
	int ddamage,
	Type ttype,
	int ddamageAddition,
	int sstrAdditionPerc,
	int ddexAdditionPerc,
	int sshieldProbAddition,
	int sshieldDefPercentAddition,
	const string& nname
) :
	damage(ddamage),
	type(ttype),
	damageAddition(ddamageAddition),
	strAdditionPerc(sstrAdditionPerc),
	dexAdditionPerc(ddexAdditionPerc),
	shieldProbAddition(sshieldProbAddition),
	shieldDefPercentAddition(sshieldDefPercentAddition),
	name(nname)
{ }

void Weapon::update(int sstrength, int ddexterity)
{
	damageAddition = sstrength * strAdditionPerc / 100 + ddexterity * dexAdditionPerc / 100;
}

int Weapon::getTotalDamage()
{
	return damage + damageAddition;
}

bool Weapon::isCompatibleWith(Type ttype)
{
	switch (type)
	{
	case Weapon::SWORD:
		if (ttype == Weapon::SPEAR || ttype == Weapon::AXE)
			return false;
		break;
	case Weapon::SPEAR:
		return false;
	case Weapon::DAGGER:
		if (ttype == Weapon::SPEAR || ttype == Weapon::AXE)
			return false;
		break;
	case Weapon::AXE:
		return false;
	case Weapon::MACE:
		if (ttype == Weapon::SPEAR || ttype == Weapon::AXE)
			return false;
		break;
	case Weapon::SHIELD:
		if (ttype == Weapon::SPEAR || ttype == Weapon::AXE || ttype == Weapon::SHIELD)
			return false;
		break;
	default:
		outputError("Unknown weapon type!\n");
		break;
	}
	return true;
}
