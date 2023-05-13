#pragma once

struct Weapon {
	string name;
	int durability;
	int damage, length, weigth, speed;

	Weapon();
	Weapon(string, int, int, int, int, int);
};
