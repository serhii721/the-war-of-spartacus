#include "stdafx.h"
#include "Item.h"

Item::Item() : itemType(GOLD), value(BASIC_ITEM_VALUE) { }

Item::Item(ItemType iitemType, int vvalue) : itemType(iitemType), value(vvalue) { }

Item::Item(const Item& I) : itemType(I.itemType), value(I.value) { }

Item& Item::operator=(const Item& I)
{
	if (&I == this) return *this;

	itemType = I.itemType;
	value = I.value;

	return *this;
}

Item::~Item() { }

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
