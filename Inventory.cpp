#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory() : items() { }

Inventory::Inventory(const map<int, pair<unique_ptr<Item>, int>>& iitems)
{
	for (const auto& pair : iitems)
	{
		// Create new unique_ptr and move it into pair
		unique_ptr<Item> newItem = pair.second.first ? pair.second.first->clone() : nullptr;
		items.emplace(newItem->getID(), make_pair(move(newItem), pair.second.second));
	}
}

Inventory::Inventory(const Inventory& I)
{
	for (const auto& pair : I.items)
	{
		// Create new unique_ptr and move it into pair
		unique_ptr<Item> newItem = pair.second.first ? pair.second.first->clone() : nullptr;
		items.emplace(newItem->getID(), make_pair(move(newItem), pair.second.second));
	}
}

Inventory& Inventory::operator=(const Inventory& I)
{
	if (&I == this) return *this;

	items.clear();

	for (const auto& pair : I.items)
	{
		// Create new unique_ptr and move it into pair
		unique_ptr<Item> newItem = pair.second.first ? pair.second.first->clone() : nullptr;
		items.emplace(newItem->getID(), make_pair(move(newItem), pair.second.second));
	}

	return *this;
}

Inventory::~Inventory() { }

PairRef<unique_ptr<Item>&, int&> Inventory::operator[](int index)
{
	if (index >= items.size())
		throw out_of_range("Index out of range");

	auto it = items.begin();
	advance(it, index);
	return { it->second.first, it->second.second };
}

PairRef<const unique_ptr<Item>&, const int&> Inventory::operator[](int index) const
{
	if (index >= items.size())
		throw out_of_range("Index out of range");

	auto it = items.begin();
	advance(it, index);
	return { it->second.first, it->second.second };
}

int Inventory::size() const
{
	return items.size();
}

const unique_ptr<Item>& Inventory::getItem(int id) const
{
	auto it = items.find(id);
	if (it != items.end())
		return it->second.first; // Return item
	return nullptr; // Item not found - return nullptr
}

int Inventory::getItemQuantity(int id) const
{
	auto it = items.find(id);
	if (it != items.end())
		return it->second.second; // Return quantity
	return 0; // Item not found - return 0
}

Item::ItemType Inventory::getItemType(int id) const
{
	auto it = items.find(id);
	if (it != items.end())
		return it->second.first->getItemType(); // Return item type
	throw out_of_range("Item is not in inventory");
}

void Inventory::addItem(unique_ptr<Item> item, int quantity)
{
	// Get item unique ID
	int id = item->getID();

	// Search for this id in inventory
	auto it = items.find(id);

	if (it != items.end()) // If item is already in inventory - increase its quantity
		it->second.second += quantity;
	else if (items.size() >= MAX_INVENTORY_SIZE) // Check space in inventory
		throw out_of_range("Cannot add item. Inventory is full.");
	else // If item is not in inventory - add it
		items.emplace(id, make_pair(move(item), quantity));
}

void Inventory::removeItem(int id, int quantity)
{
	auto it = items.find(id);

	// Check if item is in inventory and there's enough to remove
	if (it != items.end() && it->second.second >= quantity)
	{
		// Remove specified quantity
		it->second.second -= quantity;
		if (it->second.second == 0) // If there's 0 left - remove item
			items.erase(it);
	}
}
