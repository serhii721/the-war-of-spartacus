#pragma once

class City
{
private:
	string name;
	Arena arena;
	//HarmlessNPC trader;

public:
	City();
	City(string name_, const Arena&);
	City(const City&);
	City& operator=(const City&);
	~City();

	void setName(string);
	const string& getName() const;
};
