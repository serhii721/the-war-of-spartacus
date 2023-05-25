#pragma once

struct Gladiator
{
	string name;
	unsigned age;
	int health, fame, fatigue;
	int strength, constitution, dexterity, intelligence, wisdom, charisma;

	// Equipment
	Weapon* rightHand;
	Weapon* leftHand;
	// TODO shield 0 1 2
	Armour* armour;

	Gladiator();
	Gladiator(string, unsigned, int, int, int, int, int, int, int, int, int, Weapon*, Weapon*, Armour*);

	bool equipWeapon(Weapon*);
	void attack(Gladiator&, AttackResult&, int&);
	
	bool isAlive() const;
	bool isRightHandOccupied() const;
	bool isLeftHandOccupied() const;
	bool isArmourEquipped() const;

	int getDefense() const;

	~Gladiator();
};
