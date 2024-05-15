#pragma once

class City
{
private:
	int nameIndex;
	Arena arena;
	int level;
	//HarmlessNPC trader;

public:
	City();
	City(int nameIndex_, const Arena&, int level_);
	City(const City&);
	City& operator=(const City&);
	~City();

	void setName(int);
	void setLevel(int);

	int getNameIndex() const;
	Arena& getArena();
	int getLevel() const;
};
