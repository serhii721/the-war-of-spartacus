#pragma once

class Weapon
{
public:
	enum Type
	{
		SWORD,
		SPEAR,
		DAGGER,
		AXE,
		MACE,
		SHIELD,
		NUMBER
	};

	int damage;

	Type type;
	int damageAddition;
	int strAdditionPerc, dexAdditionPerc;

	int shieldProbAddition, shieldDefPercentAddition;

	string name;

	Weapon();
	Weapon(int, Type, int, int, int, int, int, const string&);

	void update(int, int);
	int getTotalDamage();

	bool isCompatibleWith(Type);
};
