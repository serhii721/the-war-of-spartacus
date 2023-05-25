#include "pch.h"
#include "Weapon.h"

Weapon::Weapon() :
	damage(BASIC_WEAPON_DAMAGE),
	type(BASIC_WEAPON_TYPE),
	damageAddition(0),
	shieldProbAddition(0),
	shieldDefPercentAddition(0),
	name("")
{ }

Weapon::Weapon(
	int ddamage,
	WeaponType ttype,
	int ddamageAddition,
	int sshieldProbAddition,
	int sshieldDefPercentAddition,
	const string& nname
) :
	damage(ddamage),
	type(ttype),
	damageAddition(ddamageAddition),
	shieldProbAddition(sshieldProbAddition),
	shieldDefPercentAddition(sshieldDefPercentAddition),
	name(nname)
{ }

void Weapon::update(int sstrength, int ddexterity)
{
	switch (type)
	{
	case WeaponType::SHIELD:
		break;
	case WeaponType::SWORD:
		// 50% strength + 50% dexterity
		damageAddition = sstrength / 2 + ddexterity / 2;
		break;
	case WeaponType::SPEAR:
		// 25% strength + 75% dexterity
		damageAddition = sstrength / 4 + ddexterity * 3 / 4;
		break;
	case WeaponType::DAGGER:
		// 15% strength + 85% dexterity
		damageAddition = sstrength * 3 / 20 + ddexterity * 17 / 20;
		break;
	case WeaponType::AXE:
		// 85% strength + 15% dexterity
		damageAddition = sstrength * 17 / 20 + ddexterity * 3 / 20;
		break;
	case WeaponType::MACE:
		// 75% strength + 25% dexterity
		damageAddition = sstrength * 3 / 4 + ddexterity / 4;
		break;
	default:
		outputError("Unknown weapon type!\n");
		break;
	}
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
	default:
		outputError("Unknown weapon type!\n");
		break;
	}
	return true;
}
