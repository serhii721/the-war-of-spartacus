#pragma once
class Inventory
{
private:
	unordered_map<int, pair<unique_ptr<Item>, int>> items;

public:
	Inventory();
	Inventory(const unordered_map<int, pair<unique_ptr<Item>, int>> items_);
	Inventory(const Inventory&);
	Inventory& operator=(const Inventory&);
	~Inventory();

	const unique_ptr<Item>& getItem(int id_) const;
	int getItemQuantity(int id_) const;
	Item::ItemType getItemType(int id_) const;

	void addItem(unique_ptr<Item>, int quantity_ = 1);
	void removeItem(int id_, int quantity_ = 1);
};
