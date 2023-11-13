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

Weapon::Weapon(const Weapon& W) :
	damage(W.damage),
	type(W.type),
	damageAddition(W.damageAddition),
	strAdditionPerc(W.strAdditionPerc),
	dexAdditionPerc(W.dexAdditionPerc),
	shieldProbAddition(W.shieldProbAddition),
	shieldDefPercentAddition(W.shieldDefPercentAddition),
	name(W.name)
{ }

Weapon& Weapon::operator=(const Weapon& W)
{
	if (this == &W) return *this;
	damage = W.damage;
	type = W.type;
	damageAddition = W.damageAddition;
	strAdditionPerc = W.strAdditionPerc;
	dexAdditionPerc = W.dexAdditionPerc;
	shieldProbAddition = W.shieldProbAddition;
	shieldDefPercentAddition = W.shieldDefPercentAddition;
	name = W.name;
	return *this;
}

Weapon::~Weapon() { }

void Weapon::update(int sstrength, int ddexterity)
{
	damageAddition = sstrength * strAdditionPerc / 100 + ddexterity * dexAdditionPerc / 100;
}

void Weapon::setName(const string& newName) { name = newName; }

int Weapon::getTotalDamage() const { return damage + damageAddition; }

bool Weapon::isCompatibleWith(Type ttype) const
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
		//outputError("Unknown weapon type!\n");
		break;
	}
	return true;
}

int Weapon::getDamage() const { return damage; }

Weapon::Type Weapon::getType() const { return type; }

int Weapon::getDamageAddition() const { return damageAddition; }

int Weapon::getShieldProbAddition() const { return shieldProbAddition; }

int Weapon::getShieldDefPercentAddition() const { return shieldDefPercentAddition; }

const string& Weapon::getName() const { return name; }
