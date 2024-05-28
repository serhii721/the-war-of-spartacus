#include "stdafx.h"
#include "Armour.h"

Armour::Armour() :
	Item(ItemType::ARMOUR),
	tier(MIN_ARMOUR_TIER),
	defense(0),
	type(BASIC_ARMOUR_TYPE),
	defAddition(0),
	strAdditionPerc(0),
	dexAdditionPerc(0),
	evasionProbAddition(0),
	stunProbSubtraction(0)
{ }

Armour::Armour(
	const Item& rItem,
	int ttier,
	int ddefense,
	ArmourType ttype,
	int ddefAddition,
	int sstrAdditionPerc,
	int ddexAdditionPerc,
	int eevasionProbAddition,
	int sstunProbSubstraction
) :
	Item(rItem),
	tier(ttier),
	defense(ddefense),
	type(ttype),
	defAddition(ddefAddition),
	strAdditionPerc(sstrAdditionPerc),
	dexAdditionPerc(ddexAdditionPerc),
	evasionProbAddition(eevasionProbAddition),
	stunProbSubtraction(sstunProbSubstraction)
{ }

Armour::Armour(const Armour& A) :
	Item(A),
	tier(A.tier),
	defense(A.defense),
	type(A.type),
	defAddition(A.defAddition),
	strAdditionPerc(A.strAdditionPerc),
	dexAdditionPerc(A.dexAdditionPerc),
	evasionProbAddition(A.evasionProbAddition),
	stunProbSubtraction(A.stunProbSubtraction)
{ }

Armour& Armour::operator=(const Armour& A)
{
	if (&A == this) return *this;

	Item::operator=(A);
	tier = A.tier;
	defense = A.defense;
	type = A.type;
	defAddition = A.defAddition;
	strAdditionPerc = A.strAdditionPerc;
	dexAdditionPerc = A.dexAdditionPerc;
	evasionProbAddition = A.evasionProbAddition;
	stunProbSubtraction = A.stunProbSubtraction;

	return *this;
}

Armour::~Armour() { }

unique_ptr<Item> Armour::clone() const
{
	return make_unique<Armour>(*this);
}

void Armour::update(int sstrength, int ddexterity)
{
	defAddition = ddexterity * dexAdditionPerc / 100 + sstrength * strAdditionPerc / 100;
}

int Armour::getTotalDefense() const { return defense + defAddition; }

int Armour::getTier() const { return tier; }

int Armour::getDefense() const { return defense; }

Armour::ArmourType Armour::getArmourType() const { return type; }

int Armour::getDefAddition() const { return defAddition; }

int Armour::getStrengthAdditionPercentage() const
{
	return strAdditionPerc;
}

int Armour::getDexterityAdditionPercentage() const
{
	return dexAdditionPerc;
}

int Armour::getEvasionProbAddition() const { return evasionProbAddition; }

int Armour::getStunProbSubtraction() const { return stunProbSubtraction; }
