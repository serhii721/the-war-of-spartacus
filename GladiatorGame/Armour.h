#pragma once

struct Armour
{
	int defense;

	ArmourType type;
	int defAddition;
	int evasionProb, stunSubtractionProb;

	Armour();
	Armour(int, ArmourType);

	void update(int, int);
	int getTotalDefense() const;
};
