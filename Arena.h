#pragma once

class Arena
{
private:
	vector<unique_ptr<NPC>> gladiators;

public:
	Arena();
	Arena(const vector<unique_ptr<NPC>>& gladiators_);
	Arena(const Arena&);
	Arena& operator=(const Arena&);
	~Arena();

	friend ostream& operator<<(ostream&, const Arena&);
	friend istream& operator>>(istream&, Arena&);

	vector<unique_ptr<NPC>>& getGladiators();
	unique_ptr<NPC>& getGladiator(int);

	void changeGladiator(int index_, int level_);
	// TODO: void update();
};
