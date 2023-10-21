#include "pch.h"
#include "Gladiator.h"

Gladiator::Gladiator() :
	name(""),
	age(MIN_AGE),
	health(BASIC_HEALTH),
	fullHealth(BASIC_HEALTH),
	fame(BASIC_FAME),
	fatigue(BASIC_FATIGUE),
	strength(MIN_STRENGTH),
	constitution(MIN_CONSTITUTION),
	dexterity(MIN_DEXTERITY),
	intelligence(MIN_INTELLIGENCE),
	wisdom(MIN_WISDOM),
	charisma(MIN_CHARISMA),
	rightHand(nullptr),
	leftHand(nullptr),
	armour(nullptr)
{ }

Gladiator::Gladiator(
	string nname,
	unsigned aage,
	int hhealth,
	int ffullHealth,
	int ffame,
	int ffatigue,
	int sstrength,
	int cconstitution,
	int ddexterity,
	int iintelligence,
	int wwisdom,
	int ccharisma,
	Weapon* pRightHand,
	Weapon* pLeftHand,
	Armour* pArmour
) :
	name(nname),
	age(aage),
	health(hhealth),
	fullHealth(ffullHealth),
	fame(ffame),
	fatigue(ffatigue),
	strength(sstrength),
	constitution(cconstitution),
	dexterity(ddexterity),
	intelligence(iintelligence),
	wisdom(wwisdom),
	charisma(ccharisma),
	rightHand(pRightHand),
	armour(pArmour)
{
	if (leftHand->isCompatibleWith(pRightHand->type))
		leftHand = pLeftHand;
	else if (pLeftHand)
		delete pLeftHand;
}

bool Gladiator::equipWeapon(Weapon* pWeapon)
{
	if (!isRightHandOccupied())
	{
		rightHand = pWeapon;
		return true;
	}
	if (!isLeftHandOccupied() && rightHand->isCompatibleWith(pWeapon->type))
	{
		leftHand = pWeapon;
		return true;
	}
	return false;
}

void Gladiator::attack(Gladiator& rOpponent, AttackResult& rResult, int& rDamage)
{
	/*
	 * Determining whether the opponent has a shield:
	 * 0 -- no shield, 1 -- a left hand, 2 -- a right hand.
	 */
	int whereShield = 0;
	if (rOpponent.isLeftHandOccupied() && rOpponent.leftHand->type == WeaponType::SHIELD)
		whereShield = 1;
	else if (rOpponent.isRightHandOccupied() && rOpponent.rightHand->type == WeaponType::SHIELD)
		whereShield = 2;

	// TODO dodged => evaded
	// Is the attack evaded?
	if (
		rand() % 100 < rOpponent.dexterity * 3 / 10 +
		rOpponent.isArmourEquipped() ? rOpponent.armour->evasionProbAddition : 0
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
				(1000 - PERCENT_DAMAGE_REDUCTION) / 1000;

			// Attack
			health -= rDamage;
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
		blockProbAddition = rOpponent.leftHand->shieldProbAddition;
		blockDefPercentAddition = rOpponent.leftHand->shieldDefPercentAddition;
		break;
	case 2: // Right hand
		blockProbAddition = rOpponent.rightHand->shieldProbAddition;
		blockDefPercentAddition = rOpponent.rightHand->shieldDefPercentAddition;
		break;
	}

	// Determining the weapon damage
	int weaponDamage = 0;
	if (isLeftHandOccupied() && leftHand->type != WeaponType::SHIELD)
			weaponDamage += leftHand->getTotalDamage();
	if (isRightHandOccupied() && rightHand->type != WeaponType::SHIELD)
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
			(1000 - PERCENT_DAMAGE_REDUCTION) / 1000;

		// Attack
		rOpponent.health -= rDamage;
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
			(1000 - PERCENT_DAMAGE_REDUCTION) / 1000;

		// Attack
		rOpponent.health -= rDamage;
		return;
	}

	// Basic attack
	rResult = AttackResult::DEALT_DAMAGE;

	// Is the stun attack?
	if (
		rand() % 100 < strength * 15 / 100 -
		(rOpponent.isArmourEquipped() ? rOpponent.armour->stunProbSubtraction : 0)
		)
		rResult = AttackResult::STUNNED;

	// (Player damage - Opponent defense) is reduced to prolong a fight
	rDamage = (weaponDamage + strength + dexterity - rOpponent.getDefense()) *
		(1000 - PERCENT_DAMAGE_REDUCTION) / 1000;

	// Attack
	rOpponent.health -= rDamage;
}

bool Gladiator::isAlive() const
{
	return health > 0;
}

bool Gladiator::isRightHandOccupied() const
{
	return rightHand ? true : false;
}

bool Gladiator::isLeftHandOccupied() const
{
	return leftHand ? true : false;
}

bool Gladiator::isArmourEquipped() const
{
	return armour ? true : false;
}

int Gladiator::getDefense() const
{
	return isArmourEquipped() ? armour->getTotalDefense() : 0;
}

Gladiator::~Gladiator()
{
	if (rightHand)
		delete rightHand;
	if (leftHand)
		delete leftHand;
	if (armour)
		delete armour;
}
