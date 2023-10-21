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
	WeaponType ttype,
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

bool Weapon::isCompatibleWith(WeaponType ttype)
{
	switch (type)
	{
	case WeaponType::SWORD:
		if (ttype == WeaponType::SPEAR || ttype == WeaponType::AXE)
			return false;
		break;
	case WeaponType::SPEAR:
		return false;
	case WeaponType::DAGGER:
		if (ttype == WeaponType::SPEAR || ttype == WeaponType::AXE)
			return false;
		break;
	case WeaponType::AXE:
		return false;
	case WeaponType::MACE:
		if (ttype == WeaponType::SPEAR || ttype == WeaponType::AXE)
			return false;
		break;
	case WeaponType::SHIELD:
		if (ttype == WeaponType::SPEAR || ttype == WeaponType::AXE || ttype == WeaponType::SHIELD)
			return false;
		break;
	default:
		outputError("Unknown weapon type!\n");
		break;
	}
	return true;
}
