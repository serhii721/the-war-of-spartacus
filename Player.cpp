#include "stdafx.h"
#include "Player.h"

Player::Player() :
	Fighter(),
	Leveling(),
	name("Nameless gladiator") // TODO: localized
{ }

Player::Player(
	const Fighter& F,
	int l,
	const string& rName
) :
	Fighter(F),
	Leveling(l),
	name(rName)
{ }

Player::Player(const Player& P) : Fighter(P), Leveling(P), name(P.name) { }

Player& Player::operator=(const Player& P)
{
	strength = P.strength;
	constitution = P.constitution;
	dexterity = P.dexterity;
	intelligence = P.intelligence;
	wisdom = P.wisdom;
	charisma = P.charisma;
	age = P.age;
	fame = P.fame;

	hp = P.hp;
	fullHP = P.fullHP;

	if (rightHand)
		delete rightHand;
	rightHand = new Weapon(P.getRightHand());
	if (leftHand)
		delete leftHand;
	leftHand = new Weapon(P.getLeftHand());
	if (armour)
		delete armour;
	armour = new Armour(P.getArmour());

	level = P.level;

	name = P.name;

	return *this;
}

Player::~Player() { }

// TODO: think about regex
void Player::setName(const string& rName) { name = rName; }

const string& Player::getName() const { return name; }
