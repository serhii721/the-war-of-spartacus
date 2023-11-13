#pragma once
#include "Statistics.h"

class Fighter :
	public Statistics
{
protected:
	int hp, fullHP;

	Weapon* rightHand;
	Weapon* leftHand;
	Armour* armour;

	int getDefense() const;
public:
	// TODO: move AttackResult over here

	Fighter();
	Fighter(const Statistics&, int, int, const Weapon*, const Weapon*, const Armour*);
	Fighter(const Fighter&);
	Fighter& operator=(const Fighter&);
	virtual ~Fighter();

	void setHP(int);

	bool equipWeapon(const Weapon&);
	void attack(Fighter&, AttackResult&, int&);

	bool isAlive() const;
	bool isRightHandOccupied() const;
	bool isLeftHandOccupied() const;
	bool isArmourEquipped() const;

	int getHP() const;
	int getFullHP() const;
	const Weapon& getRightHand() const;
	const Weapon& getLeftHand() const;
	const Armour& getArmour() const;
};
