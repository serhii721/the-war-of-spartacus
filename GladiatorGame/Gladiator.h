#pragma once

struct Gladiator
{
	string name;
	unsigned age;
	int health, fame, fatigue;
	int strength, constitution, dexterity, intelligence, wisdom, charisma;
	Weapon* weapon;

	Gladiator();
	Gladiator(string, unsigned, int, int, int, int, int, int, int, int, int, Weapon*);

	bool isAlive();
	void attack(Gladiator&, AttackResult&, int&);

	~Gladiator();
};
