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
	// Method returns experience which was not used to level up
	int currentLevel = level,
		expForLevel = EXPERIENCE_PER_LEVEL * (100 + EXPERIENCE_PER_LEVEL_INCREASE_PERC) / 100,
		exp = expForLevel;
	while (currentLevel > 1)
	{
		// Increasing needed experience for every next level by 5%
		expForLevel = expForLevel * (100 + EXPERIENCE_PER_LEVEL_INCREASE_PERC) / 100;
		exp += expForLevel;
		currentLevel--;
	}
	return experience - (exp - EXPERIENCE_PER_LEVEL);
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
	int calculatedLevel = calculateLevelForExperience(experience);

	while (level < calculatedLevel)
	{
		level++;
		unnassignedAttributes += ATTRIBUTES_PER_LEVEL;
		// Additional 1 attributes point for first 10 level ups
		if (level < 11)
			unnassignedAttributes++;
	}
}

int Leveling::calculateExperienceForLevel(int i)
{
	if (i == 1)
		return EXPERIENCE_PER_LEVEL;
	return calculateExperienceForLevel(i - 1) * (100 + EXPERIENCE_PER_LEVEL_INCREASE_PERC) / 100;
}

int Leveling::calculateLevelForExperience(int exp)
{
	int levelsGained = 1;
	int requiredExp = EXPERIENCE_PER_LEVEL;

	while (exp >= requiredExp)
	{
		exp -= requiredExp;
		levelsGained++;
		// Increasing needed experience for every next level by 5%
		requiredExp = requiredExp * (100 + EXPERIENCE_PER_LEVEL_INCREASE_PERC) / 100;
	}

	return levelsGained;
}
