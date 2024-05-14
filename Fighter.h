#pragma once
#include "Statistics.h"

class Fighter :
	public Statistics
{
protected:
	int hp, fullHP;

	unique_ptr<Weapon> rightHand;
	unique_ptr<Weapon> leftHand;
	unique_ptr<Armour> armour;

	int getDefense() const;
public:
	// TODO: move AttackResult over here

	Fighter();
	Fighter(const Statistics&, int, int, const unique_ptr<Weapon> rightHand_, const unique_ptr<Weapon> leftHand_, const unique_ptr<Armour> armour_);
	Fighter(const Fighter&);
	Fighter& operator=(const Fighter&);
	virtual ~Fighter();

	void setHP(int);
	void setFullHP(int);

	void updateMaxHP();
	bool equipWeapon(const Weapon&);
	void attack(Fighter&, AttackResult&, int&);

	bool isAlive() const;
	bool isRightHandOccupied() const;
	bool isLeftHandOccupied() const;
	bool isArmourEquipped() const;

	int getHP() const;
	int getFullHP() const;
	const unique_ptr<Weapon>& getRightHand() const;
	const unique_ptr<Weapon>& getLeftHand() const;
	const unique_ptr<Armour>& getArmour() const;
};
