#include "stdafx.h"
#include "Arena.h"

Arena::Arena() : gladiators() { }

Arena::Arena(const vector<NPC>& G) : gladiators(G) { }

Arena::Arena(const Arena& A) : gladiators(A.gladiators) { }

Arena& Arena::operator=(const Arena& A)
{
	if (&A == this) return *this;
	gladiators = A.gladiators;
	return *this;
}

Arena::~Arena() { }
