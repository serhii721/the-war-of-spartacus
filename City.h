#pragma once

class City
{
private:
	int nameIndex;
	Arena arena;
	//HarmlessNPC trader;

public:
	City();
	City(int nameIndex_, const Arena&);
	City(const City&);
	City& operator=(const City&);
	~City();

	void setName(int);

	int getNameIndex() const;
	Arena& getArena();
};
