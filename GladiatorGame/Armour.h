#pragma once

struct Armour
{
	int defense;

	ArmourType type;
	int defAddition;
	int evasionProbAddition, stunProbSubtraction;

	Armour();
	Armour(int, ArmourType);

	void update(int, int);
	int getTotalDefense() const;
};
