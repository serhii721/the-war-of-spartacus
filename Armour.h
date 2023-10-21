#pragma once

class Armour
{
public:
	int defense;

	ArmourType type;
	int defAddition;
	int strAdditionPerc, dexAdditionPerc;
	int evasionProbAddition, stunProbSubtraction;

	Armour();
	Armour(int, ArmourType, int, int, int, int, int);

	void update(int, int);
	int getTotalDefense() const;
};
