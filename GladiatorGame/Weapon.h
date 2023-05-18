#pragma once

struct Weapon
{
	int damage;

	WeaponType type;
	int damageAddition;

	string name;

	Weapon();
	Weapon(int, WeaponType, int, const string&);

	void update(int, int);
	int getTotalDamage();
};
