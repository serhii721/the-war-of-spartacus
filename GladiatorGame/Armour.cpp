#include "pch.h"
#include "Armour.h"

Armour::Armour() :
	defense(0),
	type(BASIC_ARMOUR_TYPE),
	evasionProb(0),
	stunSubtractionProb(0)
{ }

Armour::Armour(int ddefense, ArmourType ttype) :
	defense(ddefense),
	type(ttype),
	evasionProb(0),
	stunSubtractionProb(0)
{ }

void Armour::update(int sstrength, int ddexterity)
{
	switch (type)
	{
	case ArmourType::LIGHT:
		// Defense scale: 75% dexterity + 25% strength
		defAddition = ddexterity * 3 / 4 + sstrength / 4;
		// Additional evasion: 10% dexterity
		evasionProb = ddexterity / 10;
		break;
	case ArmourType::HEAVY:
		// Defense scale: 25% dexterity + 75% strength
		defAddition = ddexterity / 4 + sstrength * 3 / 4;
		// Stun subtraction: 4% strength
		stunSubtractionProb = sstrength / 25;
		break;
	default:
		outputError("Unknown armour type!\n");
		break;
	}
}

int Armour::getTotalDefense() const
{
	return defense + defAddition;
}
