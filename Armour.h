#pragma once

class Armour
{
public:
	enum Type { LIGHT, HEAVY, NUMBER };
	enum Stat
	{
		DEFENSE,
		STR_ADDITION_PERC,
		DEX_ADDITION_PERC,
		EVASION_PROB_ADDITION,
		STUN_PROB_SUBSTRACTION
	};

	int defense;

	Type type;
	int defAddition;
	int strAdditionPerc, dexAdditionPerc;
	int evasionProbAddition, stunProbSubtraction;

	Armour();
	Armour(int, Type, int, int, int, int, int);

	void update(int, int);
	int getTotalDefense() const;
};
