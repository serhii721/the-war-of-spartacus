#include "pch.h"
#include "Weapon.h"

Weapon::Weapon() :
	name(""),
	durability(BASIC_DURABILITY),
	damage(BASIC_DAMAGE),
	length(BASIC_LENGTH),
	weigth(BASIC_WEIGHT),
	speed(BASIC_SPEED)
{ }

Weapon::Weapon(
	string nname,
	int ddurability,
	int ddamage,
	int llength,
	int wweigth,
	int sspeed
) :
	name(nname),
	durability(ddurability),
	damage(ddamage),
	length(llength),
	weigth(wweigth),
	speed(sspeed)
{ }
