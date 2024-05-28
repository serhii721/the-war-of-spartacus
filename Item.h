#pragma once

class Item
{
protected:
	static int currentID;
	int id;
public:
	enum ItemType
	{
		ARMOUR,
		WEAPON,
		GOLD,
		ITEM
	};

	Item();
	Item(ItemType, int value_ = 1);
	Item(const Item&);
	Item& operator=(const Item&);
	virtual ~Item();
	virtual unique_ptr<Item> clone() const;

	bool operator<(const Item&) const;
private:
	ItemType itemType;
	int value;

public:
	int getID() const;
	ItemType getItemType() const;
	int getValue() const;

	void setValue(int);
};
