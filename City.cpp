#include "stdafx.h"
#include "City.h"

City::City() : name("Nameless City"), arena() { }

City::City(string nname, const Arena& A) : name(nname), arena(A) { }

City::City(const City& C) : name(C.name), arena(C.arena) { }

City& City::operator=(const City& C)
{
	if (&C == this) return *this;

	name = C.name;
	arena = C.arena;

	return *this;
}

City::~City() { }

void City::setName(string s)
{
	name = s;
}

const string& City::getName() const
{
	return name;
}
