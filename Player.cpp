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
	if (this == &P) return *this;

	Fighter::operator=(P);
	Leveling::operator=(P);

	name = P.name;

	return *this;
}

Player::~Player() { }

// TODO: think about regex
void Player::setName(const string& rName) { name = rName; }

const string& Player::getName() const { return name; }
