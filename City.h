#pragma once

class City
{
private:
	int nameIndex;
	Arena arena;
	int level;
	HarmlessNPC trader;

public:
	City();
	City(int nameIndex_, const Arena&, int level_, const HarmlessNPC& trader_);
	City(const City&);
	City& operator=(const City&);
	~City();

	friend ostream& operator<<(ostream&, const City&);
	friend istream& operator>>(istream&, City&);

	void setName(int);
	void setLevel(int);

	int getNameIndex() const;
	Arena& getArena();
	int getLevel() const;
	HarmlessNPC& getTrader();
};
