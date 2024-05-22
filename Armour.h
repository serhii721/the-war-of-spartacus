#pragma once
#include "Item.h"

class Armour :
	public Item
{
public:
	enum ArmourType { LIGHT, HEAVY, NUMBER };
	enum Stat
	{
		DEFENSE,
		STR_ADDITION_PERC,
		DEX_ADDITION_PERC,
		EVASION_PROB_ADDITION,
		STUN_PROB_SUBSTRACTION
	};

	Armour();
	Armour(const Item&, int, ArmourType, int, int, int, int, int);
	Armour(const Armour&);
	Armour& operator=(const Armour&);
	virtual ~Armour();

	void update(int, int);
	int getTotalDefense() const;

	int getDefense() const;
	ArmourType getArmourType() const;
	int getDefAddition() const;
	int getStrengthAdditionPercentage() const;
	int getDexterityAdditionPercentage() const;
	int getEvasionProbAddition() const;
	int getStunProbSubtraction() const;
private:
	int defense;

	ArmourType type;
	int defAddition;
	int strAdditionPerc, dexAdditionPerc;
	int evasionProbAddition, stunProbSubtraction;
};
