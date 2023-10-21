#pragma once

struct Weapon
{
	int damage;

	WeaponType type;
	int damageAddition;
	int strAdditionPerc, dexAdditionPerc;

	int shieldProbAddition, shieldDefPercentAddition;

	string name;

	Weapon();
	Weapon(int, WeaponType, int, int, int, int, int, const string&);

	void update(int, int);
	int getTotalDamage();

	bool isCompatibleWith(WeaponType);
};
