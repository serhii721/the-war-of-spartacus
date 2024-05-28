#include "stdafx.h"
#include "Item.h"

int Item::currentID = 1; // Initializing static variable
// All gold should have id of 0 and value of 1

Item::Item() : id(currentID++), itemType(ITEM), value(BASIC_ITEM_VALUE) { }

Item::Item(ItemType iitemType, int vvalue) : itemType(iitemType)
{
	if (itemType == GOLD)
	{
		id = 0;
		value = 1;
	}
	else
	{
		id = currentID++;
		value = vvalue;
	}
}

Item::Item(const Item& I) : itemType(I.itemType)
{
	if (itemType == GOLD)
	{
		id = 0;
		value = 1;
	}
	else
	{
		id = I.id;
		value = I.value;
	}
}

Item& Item::operator=(const Item& I)
{
	if (&I == this) return *this;

	itemType = I.itemType;
	if (I.itemType == GOLD)
	{
		id = 0;
		value = 1;
	}
	else
	{
		id = I.id;
		value = I.value;
	}

	return *this;
}

Item::~Item() { }

unique_ptr<Item> Item::clone() const
{
	return make_unique<Item>(*this);
}

bool Item::operator<(const Item& other) const
{
	if (itemType != other.itemType)
		return itemType < other.itemType;
	return value < other.value;
}

int Item::getID() const
{
	return id;
}

Item::ItemType Item::getItemType() const
{
	return itemType;
}

int Item::getValue() const
{
	return value;
}

void Item::setValue(int n)
{
	value = n;
}
