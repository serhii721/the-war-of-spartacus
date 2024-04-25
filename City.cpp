#include "stdafx.h"
#include "City.h"

City::City() : nameIndex(Cities::ROMA), arena() { }

City::City(int nname, const Arena& A) : nameIndex(nname), arena(A) { }

City::City(const City& C) : nameIndex(C.nameIndex), arena(C.arena) { }

City& City::operator=(const City& C)
{
	if (&C == this) return *this;

	nameIndex = C.nameIndex;
	arena = C.arena;

	return *this;
}

City::~City() { }

void City::setName(int s)
{
	nameIndex = s;
}

int City::getName() const
{
	return nameIndex;
}

Arena& City::getArena()
{
	return arena;
}
