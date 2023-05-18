#include "pch.h"
#include "Weapon.h"

Weapon::Weapon() :
	damage(BASIC_WEAPON_DAMAGE),
	type(BASIC_WEAPON_TYPE),
	damageAddition(0),
	name("")
{ }

Weapon::Weapon(
	int ddamage,
	WeaponType ttype,
	int ddamageAddition,
	const string& nname
) :
	damage(ddamage),
	type(ttype),
	damageAddition(ddamageAddition),
	name(nname)
{ }

void Weapon::update(int sstrength, int ddexterity)
{
	switch (type)
	{
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
