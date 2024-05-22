#include "stdafx.h"
#include "Player.h"

extern Localization l;

Player::Player() :
	Fighter(),
	Leveling(),
	name(l.getMessage(Localized::GLADIATOR))
{ }

Player::Player(
	const Fighter& F,
	const Leveling& L,
	const string& rName
) :
	Fighter(F),
	Leveling(L),
	name(rName)
{ }

Player::Player(const Player& P) : Fighter(P), Leveling(P), name(P.name) { }

Player& Player::operator=(const Player& P)
{
	if (&P == this) return *this;

	Fighter::operator=(P);
	Leveling::operator=(P);

	name = P.name;

	return *this;
}

Player::~Player() { }

// TODO: think about regex
void Player::setName(const string& rName) { name = rName; }

const string& Player::getName() const { return name; }
