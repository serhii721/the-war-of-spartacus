#include "stdafx.h"
#include "HarmlessNPC.h"

HarmlessNPC::HarmlessNPC() : level(MIN_TRADER_LEVEL), Statistics(), NamedNPC(), inventory() { }

HarmlessNPC::HarmlessNPC(int llevel, const Statistics& S, const NamedNPC& C, unique_ptr<Inventory> pInventory) : level(llevel), Statistics(S), NamedNPC(C), inventory(move(pInventory)) { }

HarmlessNPC::HarmlessNPC(const HarmlessNPC& C) : level(C.level), Statistics(C), NamedNPC(C)
{
	if (C.inventory)
		inventory = make_unique<Inventory>(*C.inventory);
	else
		inventory = nullptr;
}

HarmlessNPC& HarmlessNPC::operator=(const HarmlessNPC& C)
{
	if (&C == this) return *this;
	level = C.level;
	Statistics::operator=(C);
	NamedNPC::operator=(C);

	if (C.inventory)
	{
		if (!inventory)
			inventory = make_unique<Inventory>(*C.inventory);
		else
			*inventory = *C.inventory;
	}
	else
		inventory = nullptr;

	return *this;
}

HarmlessNPC::~HarmlessNPC() { }

int HarmlessNPC::getLevel() const { return level; }

const unique_ptr<Inventory>& HarmlessNPC::getInventory() const { return inventory; }

void HarmlessNPC::saveToFile(const string& directory) const
{
	const string PATH = directory,
		FILE_TRADER = "Trader",
		FOLDER_INVENTORY = "Inventory/",
		FORMAT = ".sav";

	// Opening file for saving
	ofstream fout(PATH + FILE_TRADER + FORMAT, ios::binary);

	if (!fout)
		throw new exception("Error: Couldn't open file for trader's saving");

	fout << level << " ";

	// Inventory
	if (inventory)
	{
		fout << "1 ";
		// Creating the folder for inventory if it doesn't exist
		BOOL success = CreateDirectory((PATH + FOLDER_INVENTORY).c_str(), NULL);
		if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
			throw new exception("Error: Couldn't create directory for save");

		inventory->saveToFile(PATH + FOLDER_INVENTORY);
	}
	else
		fout << "0 ";

	fout.close();
}

void HarmlessNPC::loadFromFile(const string& directory)
{
	const string PATH = directory,
		FILE_TRADER = "Trader",
		FOLDER_INVENTORY = "Inventory/",
		FORMAT = ".sav";

	// Opening file for loading
	ifstream fin(PATH + FILE_TRADER + FORMAT, ios::binary);

	if (!fin)
		throw new exception("Error: Couldn't open file for trader's loading");

	fin >> level;

	// Inventory
	inventory = make_unique<Inventory>();
	int hasInventory;
	fin >> hasInventory;
	if (hasInventory)
		inventory->loadFromFile(PATH + FOLDER_INVENTORY);
	else
		inventory.reset();

	fin.close();
}
