#include "stdafx.h"
#include "Armour.h"

Armour::Armour() :
	defense(0),
	type(BASIC_ARMOUR_TYPE),
	defAddition(0),
	strAdditionPerc(0),
	dexAdditionPerc(0),
	evasionProbAddition(0),
	stunProbSubtraction(0)
{ }

Armour::Armour(
	int ddefense,
	ArmourType ttype,
	int ddefAddition,
	int sstrAdditionPerc,
	int ddexAdditionPerc,
	int eevasionProbAddition,
	int sstunProbSubstraction
) :
	defense(ddefense),
	type(ttype),
	defAddition(ddefAddition),
	strAdditionPerc(sstrAdditionPerc),
	dexAdditionPerc(ddexAdditionPerc),
	evasionProbAddition(eevasionProbAddition),
	stunProbSubtraction(sstunProbSubstraction)
{ }

void Armour::update(int sstrength, int ddexterity)
{
	defAddition = ddexterity * dexAdditionPerc / 100 + sstrength * strAdditionPerc / 100;
}

int Armour::getTotalDefense() const
{
	return defense + defAddition;
}
