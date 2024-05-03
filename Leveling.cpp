#include "stdafx.h"
#include "Leveling.h"

Leveling::Leveling() : level(MIN_LEVEL), experience(0) { }

Leveling::Leveling(int l, int e, int a) : level(l), experience(e), unnassignedAttributes(a) { }

Leveling::Leveling(const Leveling& L) :
	level(L.level),
	experience(L.experience),
	unnassignedAttributes(L.unnassignedAttributes)
{ }

Leveling& Leveling::operator=(const Leveling& L)
{
	if (&L == this) return *this;
	level = L.level;
	experience = L.experience;
	unnassignedAttributes = L.unnassignedAttributes;
	return *this;
}

Leveling::~Leveling() { }

int Leveling::getLevel() const { return level; }

int Leveling::getExperience() const
{
	return experience;
}

int Leveling::getUnnassignedAttributes()
{
	return unnassignedAttributes;
}

void Leveling::setLevel(int l)
{
	level = l;
}

void Leveling::setUnnassignedAttributes(int n)
{
	unnassignedAttributes = n;
}

void Leveling::gainExperience(int e)
{
	experience += e;
	while (experience >= EXPERIENCE_PER_LEVEL)
	{
		experience -= EXPERIENCE_PER_LEVEL;
		level++;
		unnassignedAttributes += ATTRIBUTES_PER_LEVEL;
		// Additional 1 attributes point for first 10 level ups
		if (level < 11)
			unnassignedAttributes++;
	}
}
