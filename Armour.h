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

	Armour();
	Armour(int, Type, int, int, int, int, int);
	Armour(const Armour&);
	Armour& operator=(const Armour&);
	virtual ~Armour();

	void update(int, int);
	int getTotalDefense() const;

	int getDefense() const;
	Type getType() const;
	int getDefAddition() const;
	int getEvasionProbAddition() const;
	int getStunProbSubtraction() const;
private:
	int defense;

	Type type;
	int defAddition;
	int strAdditionPerc, dexAdditionPerc;
	int evasionProbAddition, stunProbSubtraction;
};
