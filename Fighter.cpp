#include "stdafx.h"
#include "Fighter.h"

int Fighter::getDefense() const
{
	return isArmourEquipped() ? armour->getTotalDefense() : 0;
}

Fighter::Fighter() :
	Statistics(),
	hp(BASIC_HP),
	fullHP(BASIC_HP),
	rightHand(nullptr),
	leftHand(nullptr),
	armour(nullptr)
{ }

Fighter::Fighter(
	const Statistics& rStats,
	int hhp,
	int ffullHP,
	const Weapon* pRightHand,
	const Weapon* pLeftHand,
	const Armour* pArmour
) :
	Statistics(rStats),
	hp(hhp),
	fullHP(ffullHP)
{		
	if (pRightHand && pRightHand != nullptr)
		rightHand = new Weapon(*pRightHand);
	else
		rightHand = nullptr;

	if (pLeftHand && pLeftHand != nullptr)
		leftHand = new Weapon(*pLeftHand);
	else
		leftHand = nullptr;

	if (pArmour && pArmour != nullptr)
		armour = new Armour(*pArmour);
	else
		armour = nullptr;

	// TODO: think about processing exceptions
}

Fighter::Fighter(const Fighter& F) :
	Statistics(F),
	hp(F.hp),
	fullHP(F.fullHP),
	rightHand(new Weapon(F.getRightHand())),
	leftHand(new Weapon(F.getLeftHand())),
	armour(new Armour(F.getArmour()))
{ }

Fighter& Fighter::operator=(const Fighter& F)
{
	if (this == &F) return *this;

	Statistics::operator=(F);

	hp = F.hp;
	fullHP = F.fullHP;

	if (rightHand)
		delete rightHand;
	rightHand = new Weapon(F.getRightHand());
	if (leftHand)
		delete leftHand;
	leftHand = new Weapon(F.getLeftHand());
	if (armour)
		delete armour;
	armour = new Armour(F.getArmour());

	return *this;
}

Fighter::~Fighter()
{
	if (rightHand)
		delete rightHand;
	if (leftHand)
		delete leftHand;
	if (armour)
		delete armour;
}

void Fighter::setHP(int hhp) { hp = hhp; }

bool Fighter::equipWeapon(const Weapon& rWeapon)
{
	if (!isRightHandOccupied())
	{
		rightHand = new Weapon(rWeapon);
		return true;
	}
	if (!isLeftHandOccupied() && rightHand->isCompatibleWith(rWeapon.getType()))
	{
		leftHand = new Weapon(rWeapon);
		return true;
	}
	return false;
	// TODO: process using the inventory; move a shield to the left hand
}

void Fighter::attack(Fighter& rOpponent, AttackResult& rResult, int& rDamage)
{
	/*
	 * Determining whether the opponent has a shield:
	 * 0 -- no shield, 1 -- a left hand, 2 -- a right hand.
	 */
	int whereShield = 0;
	if (rOpponent.isLeftHandOccupied() && rOpponent.leftHand->getType() == Weapon::SHIELD)
		whereShield = 1;
	else if (rOpponent.isRightHandOccupied() && rOpponent.rightHand->getType() == Weapon::SHIELD)
		whereShield = 2;

	// TODO: dodged => evaded
	// Is the attack evaded?
	if (
		rand() % 100 < rOpponent.dexterity * 3 / 10 +
		rOpponent.isArmourEquipped() ? rOpponent.armour->getEvasionProbAddition() : 0
		)
	{
		// Does the opponent counterattack?
		if (rand() % 100 < rOpponent.dexterity / 5)
		{
			rResult = AttackResult::WERE_COUNTERATTAKED;

			// Determining the opponent's weapon damage
			int opponentWeaponDamage = 0;
			switch (whereShield)
			{
			case 0: // No shield
				if (rOpponent.isRightHandOccupied())
					opponentWeaponDamage += rOpponent.rightHand->getTotalDamage();
				// The left hand is checked when determining whether a shield
				opponentWeaponDamage += rOpponent.leftHand->getTotalDamage();
				break;
			case 1: // Left hand
				if (rOpponent.isRightHandOccupied())
					opponentWeaponDamage = rOpponent.rightHand->getTotalDamage();
				break;
			case 2: // Right hand
				// The left hand is checked when determining whether a shield
				opponentWeaponDamage = rOpponent.leftHand->getTotalDamage();
				break;
			}

			// (Opponent damage - Player defense) is reduced to prolong a fight
			rDamage = (opponentWeaponDamage + rOpponent.strength + rOpponent.dexterity - getDefense()) *
				(ONE_HUNDRED_PERCENT - DAMAGE_REDUCTION_PERCENT) / ONE_HUNDRED_PERCENT;

			// Attack
			hp -= rDamage;
			return;
		}

		rResult = AttackResult::WERE_DODGED;
		rDamage = 0;
		return;
	}

	// Determining the opponent's block bonus if a shield is equipped
	int blockProbAddition = 0, blockDefPercentAddition = 0;
	switch (whereShield)
	{
	case 0: // No shield
		break;
	case 1: // Left hand
		blockProbAddition = rOpponent.leftHand->getShieldProbAddition();
		blockDefPercentAddition = rOpponent.leftHand->getShieldDefPercentAddition();
		break;
	case 2: // Right hand
		blockProbAddition = rOpponent.rightHand->getShieldProbAddition();
		blockDefPercentAddition = rOpponent.rightHand->getShieldDefPercentAddition();
		break;
	}

	// Determining the weapon damage
	int weaponDamage = 0;
	if (isLeftHandOccupied() && leftHand->getType() != Weapon::SHIELD)
		weaponDamage += leftHand->getTotalDamage();
	if (isRightHandOccupied() && rightHand->getType() != Weapon::SHIELD)
		weaponDamage += rightHand->getTotalDamage();

	/*
	 * Is the attack blocked?
	 *
	 * <= 35% probability + <= 30% reduction from the constitution and
	 * <= 15% probability + <= 60% reduction from a shield.
	 */
	if (rand() % 100 < rOpponent.constitution * 35 / 100 + blockProbAddition)
	{
		rResult = AttackResult::WERE_BLOCKED;
		// (Damage - Defense) is reduced by the block defense and by the percent to prolong fight
		rDamage = (weaponDamage + strength + dexterity - rOpponent.getDefense()) *
			(100 - rOpponent.constitution * 3 / 10 + blockDefPercentAddition) / 100 *
			(ONE_HUNDRED_PERCENT - DAMAGE_REDUCTION_PERCENT) / ONE_HUNDRED_PERCENT;

		// Attack
		rOpponent.hp -= rDamage;
		return;
	}

	// Is the attack critical?
	if (rand() % 100 < dexterity * 3 / 10)
	{
		rResult = AttackResult::DEALT_CRIT_DAMAGE;
		/*
		 * (Player damage - Opponent defense) is increased by the crit (30% + <=40% from the dexterity) and
		 * reduced to prolong fight.
		 */
		rDamage = (weaponDamage + strength + dexterity - rOpponent.getDefense()) *
			(130 + dexterity * 4 / 10) / 100 *
			(ONE_HUNDRED_PERCENT - DAMAGE_REDUCTION_PERCENT) / ONE_HUNDRED_PERCENT;

		// Attack
		rOpponent.hp -= rDamage;
		return;
	}

	// Basic attack
	rResult = AttackResult::DEALT_DAMAGE;

	// Is the stun attack?
	if (
		rand() % 100 < strength * 15 / 100 -
		(rOpponent.isArmourEquipped() ? rOpponent.armour->getStunProbSubtraction() : 0)
		)
		rResult = AttackResult::STUNNED;

	// (Player damage - Opponent defense) is reduced to prolong a fight
	rDamage = (weaponDamage + strength + dexterity - rOpponent.getDefense()) *
		(ONE_HUNDRED_PERCENT - DAMAGE_REDUCTION_PERCENT) / ONE_HUNDRED_PERCENT;

	// Attack
	rOpponent.hp -= rDamage;
}

bool Fighter::isAlive() const { return hp > 0; }

bool Fighter::isRightHandOccupied() const { return rightHand != nullptr; }

bool Fighter::isLeftHandOccupied() const { return leftHand != nullptr; }

bool Fighter::isArmourEquipped() const { return armour != nullptr; }

int Fighter::getHP() const { return hp; }

int Fighter::getFullHP() const { return fullHP; }

const Weapon& Fighter::getRightHand() const { return *rightHand; }

const Weapon& Fighter::getLeftHand() const { return *leftHand; }

const Armour& Fighter::getArmour() const { return *armour; }
