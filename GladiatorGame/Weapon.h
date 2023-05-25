#pragma once

struct Weapon
{
	int damage;

	WeaponType type;
	int damageAddition;
	int shieldProbAddition, shieldDefPercentAddition;

	string name;

	Weapon();
	Weapon(int, WeaponType, int, int, int, const string&);

	void update(int, int);
	int getTotalDamage();

	bool isCompatibleWith(WeaponType);
};
