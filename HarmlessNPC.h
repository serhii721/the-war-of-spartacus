#pragma once
#include "Statistics.h"
#include "NamedNPC.h"

class HarmlessNPC :
	public Statistics,
	public NamedNPC
{
private:
	unique_ptr<Inventory> inventory;
public:
	HarmlessNPC();
	HarmlessNPC(const Statistics&, const NamedNPC&, unique_ptr<Inventory>);
	HarmlessNPC(const HarmlessNPC&);
	HarmlessNPC& operator=(const HarmlessNPC&);
	virtual ~HarmlessNPC();

	const unique_ptr<Inventory>& getInventory() const;
};
