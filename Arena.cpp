#include "stdafx.h"
#include "Arena.h"

Arena::Arena() : gladiators() { }

Arena::Arena(const vector<unique_ptr<NPC>>& G)
{
	for (const auto& gladiator : G)
		gladiators.push_back(gladiator->clone());
}

Arena::Arena(const Arena& A)
{
	for (const auto& gladiator : A.gladiators)
		gladiators.push_back(gladiator->clone());
}

Arena& Arena::operator=(const Arena& A)
{
	if (&A == this) return *this;

	gladiators.clear();
	for (const auto& gladiator : A.gladiators)
		gladiators.push_back(gladiator->clone());

	return *this;
}

Arena::~Arena() { }

vector<unique_ptr<NPC>>& Arena::getGladiators()
{
	return gladiators;
}

unique_ptr<NPC>& Arena::getGladiator(int n)
{
	return gladiators[n];
}

void Arena::changeGladiator(int index, int level)
{
	gladiators[index] = generateNPC(level);
}
