#pragma once

class Item
{
public:
	enum Type { ARMOUR, WEAPON };

	Item();
	// TODO: ctors, operator=()
	virtual ~Item();

	// TODO: getType()
private:
	Type type;
};
