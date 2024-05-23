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
		id = currentID++;
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
		id = currentID++;
		value = I.value;
	}

	return *this;
}

Item::~Item() { }

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
