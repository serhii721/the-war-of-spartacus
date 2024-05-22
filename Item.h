#pragma once

class Item
{
public:
	enum ItemType
	{
		ARMOUR,
		WEAPON,
		GOLD
	};

	Item();
	Item(ItemType, int value_ = 1);
	Item(const Item&);
	Item& operator=(const Item&);
	virtual ~Item();

private:
	ItemType itemType;
	int value;

public:
	ItemType getItemType() const;
	int getValue() const;

	void setValue(int);
};
